#include "../Test.h"

#include <atomic>
#include <thread>

using namespace MereTDD;

TEST("Test can use additional threads")
{
    std::vector<ThreadConfirmException> threadExs(2);
    std::atomic<int> count {0};
    std::vector<std::thread> threads;
    for (int c = 0; c < 2; ++c)
    {
        threads.emplace_back(
            [&threadEx = threadExs[c], &count]()
        {
            try
            {
                for (int i = 0; i < 100'000; ++i)
                {
                    ++count;
                }
                CONFIRM_THAT(count, NotEquals(200'001));
            }
            catch (ConfirmException const & ex)
            {
                threadEx.setFailure(ex.line(), ex.reason());
            }
        });
    }

    for (auto & t : threads)
    {
        t.join();
    }
    for (auto const & ex: threadExs)
    {
        ex.checkFailure();
    }
    CONFIRM_THAT(count, Equals(200'000));
}
