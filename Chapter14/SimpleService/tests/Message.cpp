#include "../Service.h"

#include "SetupTeardown.h"

#include <MereTDD/Test.h>

using namespace MereTDD;

TEST_SUITE("Request can be sent and response received", "Service 1")
{
    std::string user = "123";
    std::string path = "";
    std::string request = "Hello";
    std::string expectedResponse = "Hi, " + user;

    std::string response = gService1.service().handleRequest(
        user, path, request);
    CONFIRM_THAT(response, Equals(expectedResponse));
}
