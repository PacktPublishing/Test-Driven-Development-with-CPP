#include "../Log.h"

#include "Util.h"

#include <MereTDD/Test.h>

TEST("Simple message can be logged")
{
    std::string message = "simple ";
    message += Util::randomString();
    MereMemo::log() << message << " with more text.";

    bool result = Util::isTextInFile(message, "application.log");
    CONFIRM_TRUE(result);
}

TEST("Complicated message can be logged")
{
    std::string message = "complicated ";
    message += Util::randomString();
    MereMemo::log() << message
        << " double=" << 3.14
        << " quoted=" << std::quoted("in quotes");

    bool result = Util::isTextInFile(message, "application.log");
    CONFIRM_TRUE(result);
}
