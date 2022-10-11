#ifndef MERETDD_TEST_H
#define MERETDD_TEST_H

#include <string_view>
#include <vector>

namespace MereTDD
{

class TestInterface
{
public:
    virtual ~TestInterface () = default;

    virtual void run () = 0;
};

inline std::vector<TestInterface *> & getTests ()
{
    static std::vector<TestInterface *> tests;

    return tests;
}

inline void runTests ()
{
    for (auto * test: getTests())
    {
        test->run();
    }
}

} // namespace MereTDD

#define TEST \
class Test : public MereTDD::TestInterface \
{ \
public: \
    Test (std::string_view name) \
    : mName(name), mResult(true) \
    { \
        MereTDD::getTests().push_back(this); \
    } \
    void run () override; \
private: \
    std::string mName; \
    bool mResult; \
}; \
Test test("testCanBeCreated"); \
void Test::run ()

#endif // MERETDD_TEST_H
