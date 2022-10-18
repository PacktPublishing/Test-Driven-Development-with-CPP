#include "SetupTeardown.h"

MereTDD::TestSuiteSetupAndTeardown<ServiceSetup>
gService1("Calculation Service", "Service 1");

MereTDD::TestSuiteSetupAndTeardown<TestServiceSetup>
gService2("Calculation Test Service", "Service 2");
