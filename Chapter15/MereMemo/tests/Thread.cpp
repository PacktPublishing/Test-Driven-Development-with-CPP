#include "../Log.h"

#include "Util.h"

#include <MereTDD/Test.h>
#include <thread>

TEST("log can be called from multiple threads")
{
    // We'll have 3 threads with 50 messages each.
    std::vector<std::string> messages;
    for (int i = 0; i < 150; ++i)
    {
        std::string message = std::to_string(i);
        message += " thread-safe message ";
        message += Util::randomString();
        messages.push_back(message);
    }

    std::vector<std::thread> threads;
    for (int c = 0; c < 3; ++c)
    {
        threads.emplace_back(
            [c, &messages]()
        {
            int indexStart = c * 50;
            for (int i = 0; i < 50; ++i)
            {
                MereMemo::log() << messages[indexStart + i];
            }
        });
    }

    for (auto & t : threads)
    {
        t.join();
    }
    for (auto const & message: messages)
    {
        bool result = Util::isTextInFile(message, "application.log");
        CONFIRM_TRUE(result);
    }
}
