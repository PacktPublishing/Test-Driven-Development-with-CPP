#include <MereMemo/Log.h>
#include <MereTDD/Test.h>

#include <iostream>

int main ()
{
    MereMemo::FileOutput appFile("logs");
    MereMemo::addLogOutput(appFile);

    return MereTDD::runTests(std::cout);
}
