#ifndef SIMPLESERVICE_SERVICE_H
#define SIMPLESERVICE_SERVICE_H

#include <condition_variable>
#include <string>
#include <variant>

namespace SimpleService
{

struct CalculateRequest
{
    int mSeed;
};

struct StatusRequest
{
    std::string mToken;
};

using RequestVar = std::variant<
    CalculateRequest,
    StatusRequest
    >;

struct ErrorResponse
{
    std::string mReason;
};

struct CalculateResponse
{
    std::string mToken;
};

struct StatusResponse
{
    bool mComplete;
    int mProgress;
    int mResult;
};

using ResponseVar = std::variant<
    ErrorResponse,
    CalculateResponse,
    StatusResponse
    >;

void normalCalc (int seed, int & progress, int & result);

extern std::mutex service2Mutex;
extern std::condition_variable testCalcCV;
extern std::condition_variable testCV;
extern bool testCalcReady;
extern bool testReady;
void testCalc (int seed, int & progress, int & result);

class Service
{
public:
    using CalcFunc = void (*) (int, int &, int &);

    Service (CalcFunc f = normalCalc)
    : mCalc(f)
    { }

    void start ();

    ResponseVar handleRequest (std::string const & user,
        std::string const & path,
        RequestVar const & request);

private:
    CalcFunc mCalc;
};

} // namespace SimpleService

#endif // SIMPLESERVICE_SERVICE_H
