#include "../Log.h"

#include "LogTags.h"

#include <MereTDD/Test.h>

#include <iostream>

int main ()
{
    MereMemo::addDefaultTag(info);
    MereMemo::addDefaultTag(green);

    return MereTDD::runTests(std::cout);
}
