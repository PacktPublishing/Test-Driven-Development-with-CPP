#ifndef SIMPLESERVICE_TESTS_SUITES_H
#define SIMPLESERVICE_TESTS_SUITES_H

#include "../Service.h"

#include <MereMemo/Log.h>
#include <MereTDD/Test.h>

class ServiceSetup
{
public:
    void setup ()
    {
        mService.start();
    }

    void teardown ()
    {
    }

    SimpleService::Service & service ()
    {
        return mService;
    }

private:
    SimpleService::Service mService;
};

extern MereTDD::TestSuiteSetupAndTeardown<ServiceSetup>
gService1;

class TestServiceSetup
{
public:
    TestServiceSetup ()
    : mService(SimpleService::testCalc)
    { }

    void setup ()
    {
        mService.start();
    }

    void teardown ()
    {
    }

    SimpleService::Service & service ()
    {
        return mService;
    }

private:
    SimpleService::Service mService;
};

extern MereTDD::TestSuiteSetupAndTeardown<TestServiceSetup>
gService2;

#endif // SIMPLESERVICE_TESTS_SUITES_H
