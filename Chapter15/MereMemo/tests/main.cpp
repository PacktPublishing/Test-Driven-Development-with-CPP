#include "../Log.h"

#include "LogTags.h"

#include <MereTDD/Test.h>

#include <iostream>

int main ()
{
    MereMemo::FileOutput appFile("logs");
    //appFile.namePattern() = "application-{}.log";
    //appFile.maxSize() = 10'000'000;
    //appFile.rolloverCount() = 5;
    MereMemo::addLogOutput(appFile);

    MereMemo::StreamOutput consoleStream(std::cout);
    MereMemo::addLogOutput(consoleStream);

    MereMemo::addDefaultTag(info);
    MereMemo::addDefaultTag(green);

    return MereTDD::runTests(std::cout);
}
