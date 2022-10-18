#include "Service.h"

#include "LogTags.h"

#include <MereMemo/Log.h>
#include <mutex>
#include <thread>
#include <vector>

namespace
{
    std::mutex & getCalcMutex ()
    {
        static std::mutex m;
        return m;
    }

    class CalcRecord
    {
    public:
        CalcRecord ()
        { }

        CalcRecord (CalcRecord const & src)
        {
            const std::lock_guard<std::mutex> lock(getCalcMutex());
            mComplete = src.mComplete;
            mProgress = src.mProgress;
            mResult = src.mResult;
        }

        void getData (bool & complete, int & progress, int & result)
        {
            const std::lock_guard<std::mutex> lock(getCalcMutex());
            complete = mComplete;
            progress = mProgress;
            result = mResult;
        }

        void setData (bool complete, int progress, int result)
        {
            const std::lock_guard<std::mutex> lock(getCalcMutex());
            mComplete = complete;
            mProgress = progress;
            mResult = result;
        }

        CalcRecord &
        operator = (CalcRecord const & rhs) = delete;

    private:
        bool mComplete {false};
        int mProgress {0};
        int mResult {0};
    };

    std::vector<CalcRecord> calculations;
}

void SimpleService::normalCalc (
    int seed, int & progress, int & result)
{
    progress = 100;
    result = seed * 10;
}

std::mutex SimpleService::service2Mutex;
std::condition_variable SimpleService::testCalcCV;
std::condition_variable SimpleService::testCV;
bool SimpleService::testCalcReady {false};
bool SimpleService::testReady {false};

void SimpleService::testCalc (
    int seed, int & progress, int & result)
{
    // Wait until the test has completed the first status request.
    {
        std::unique_lock<std::mutex> lock(service2Mutex);
        testCV.wait(lock, []
        {
            return testReady;
        });
    }

    progress = 100;
    result = seed * 8;

    // Notify the test that the calculation is ready.
    {
        std::lock_guard<std::mutex> lock(service2Mutex);
        testCalcReady = true;
    }
    testCalcCV.notify_one();
}

void SimpleService::Service::start ()
{
    MereMemo::log(info) << "Service is starting.";
}

SimpleService::ResponseVar SimpleService::Service::handleRequest (
    std::string const & user,
    std::string const & path,
    RequestVar const & request)
{
    ResponseVar response;
    if (auto const * req = std::get_if<CalculateRequest>(&request))
    {
        MereMemo::log(debug, User(user), LogPath(path))
            << "Received Calculate request for: "
            << std::to_string(req->mSeed);

        calculations.emplace_back();
        int calcIndex = calculations.size() - 1;
        int seed = req->mSeed;
        std::thread calcThread([this, calcIndex, seed] ()
        {
            int progress {0};
            int result {0};
            while (true)
            {
                mCalc(seed, progress, result);
                if (progress == 100)
                {
                    calculations[calcIndex].setData(true, progress, result);
                    break;
                }
                else
                {
                    calculations[calcIndex].setData(false, progress, result);
                }
            }
        });
        calcThread.detach();
        response = SimpleService::CalculateResponse {
            .mToken = std::to_string(calcIndex)
        };
    }
    else if (auto const * req = std::get_if<StatusRequest>(&request))
    {
        MereMemo::log(debug, User(user), LogPath(path))
            << "Received Status request for: "
            << req->mToken;

        int calcIndex = std::stoi(req->mToken);
        bool complete;
        int progress;
        int result;
        calculations[calcIndex].getData(complete, progress, result);
        response = SimpleService::StatusResponse {
            .mComplete = complete,
            .mProgress = progress,
            .mResult = result
        };
    }
    else
    {
        response = SimpleService::ErrorResponse {
            .mReason = "Unrecognized request."
        };
    }

    return response;
}
