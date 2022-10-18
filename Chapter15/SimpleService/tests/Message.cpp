#include "../Service.h"

#include "SetupTeardown.h"

#include <MereTDD/Test.h>

using namespace MereTDD;

TEST_SUITE("Calculate request can be sent", "Service 1")
{
    std::string user = "123";
    std::string path = "";

    SimpleService::RequestVar request =
        SimpleService::CalculateRequest {
            .mSeed = 5
        };
    auto const responseVar = gService1.service().handleRequest(
        user, path, request);
    auto const response =
        std::get_if<SimpleService::CalculateResponse>(&responseVar);
    CONFIRM_TRUE(response != nullptr);
}

TEST_SUITE("Status request generates result", "Service 1")
{
    std::string user = "123";
    std::string path = "";

    SimpleService::RequestVar calcRequest =
        SimpleService::CalculateRequest {
            .mSeed = 5
        };
    auto responseVar = gService1.service().handleRequest(
        user, path, calcRequest);
    auto const calcResponse =
        std::get_if<SimpleService::CalculateResponse>(&responseVar);
    CONFIRM_TRUE(calcResponse != nullptr);

    SimpleService::RequestVar statusRequest =
        SimpleService::StatusRequest {
            .mToken = calcResponse->mToken
        };
    int result {0};
    for (int i = 0; i < 5; ++i)
    {
        responseVar = gService1.service().handleRequest(
            user, path, statusRequest);
        auto const statusResponse =
            std::get_if<SimpleService::StatusResponse>(&responseVar);
        CONFIRM_TRUE(statusResponse != nullptr);

        if (statusResponse->mComplete)
        {
            result = statusResponse->mResult;
            break;
        }
    }
    CONFIRM_THAT(result, Equals(50));
}

TEST_SUITE("Status request to test service generates result", "Service 2")
{
    std::string user = "123";
    std::string path = "";

    SimpleService::RequestVar calcRequest =
        SimpleService::CalculateRequest {
            .mSeed = 5
        };
    auto responseVar = gService2.service().handleRequest(
        user, path, calcRequest);
    auto const calcResponse =
        std::get_if<SimpleService::CalculateResponse>(&responseVar);
    CONFIRM_TRUE(calcResponse != nullptr);

    // Make a status request right away before the service
    // is allowed to do any calculations.
    SimpleService::RequestVar statusRequest =
        SimpleService::StatusRequest {
            .mToken = calcResponse->mToken
        };
    responseVar = gService2.service().handleRequest(
        user, path, statusRequest);
    auto statusResponse =
        std::get_if<SimpleService::StatusResponse>(&responseVar);
    CONFIRM_TRUE(statusResponse != nullptr);
    CONFIRM_FALSE(statusResponse->mComplete);
    CONFIRM_THAT(statusResponse->mProgress, Equals(0));
    CONFIRM_THAT(statusResponse->mResult, Equals(0));

    // Notify the service that the test has completed the first
    // confirmation so that the service can proceed with the
    // calculation.
    {
        std::lock_guard<std::mutex> lock(SimpleService::service2Mutex);
        SimpleService::testReady = true;
    }
    SimpleService::testCV.notify_one();

    // Now wait until the service has completed the calculation.
    {
        std::unique_lock<std::mutex> lock(SimpleService::service2Mutex);
        SimpleService::testCalcCV.wait(lock, []
        {
            return SimpleService::testCalcReady;
        });
    }

    // Make another status request to get the completed result.
    responseVar = gService2.service().handleRequest(
        user, path, statusRequest);
    statusResponse =
        std::get_if<SimpleService::StatusResponse>(&responseVar);
    CONFIRM_TRUE(statusResponse != nullptr);
    CONFIRM_TRUE(statusResponse->mComplete);
    CONFIRM_THAT(statusResponse->mProgress, Equals(100));
    CONFIRM_THAT(statusResponse->mResult, Equals(40));
}
