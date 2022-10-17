#ifndef MERETDD_TEST_H
#define MERETDD_TEST_H

#include <ostream>
#include <string_view>
#include <vector>

namespace MereTDD
{

class TestBase
{
public:
    TestBase (std::string_view name)
    : mName(name), mPassed(true)
    { }

    virtual ~TestBase () = default;

    virtual void run () = 0;

    std::string_view name () const
    {
        return mName;
    }

    bool passed () const
    {
        return mPassed;
    }

    std::string_view reason () const
    {
        return mReason;
    }

    void setFailed (std::string_view reason)
    {
        mPassed = false;
        mReason = reason;
    }

private:
    std::string mName;
    bool mPassed;
    std::string mReason;
};

inline std::vector<TestBase *> & getTests ()
{
    static std::vector<TestBase *> tests;

    return tests;
}

inline int runTests (std::ostream & output)
{
    output << "Running "
        << getTests().size()
        << " tests\n";

    int numPassed = 0;
    int numFailed = 0;
    for (auto * test: getTests())
    {
        output << "---------------\n"
            << test->name()
            << std::endl;

        try
        {
            test->run();
        }
        catch (...)
        {
            test->setFailed("Unexpected exception thrown.");
        }

        if (test->passed())
        {
            ++numPassed;
            output << "Passed"
                << std::endl;
        }
        else
        {
            ++numFailed;
            output << "Failed\n"
                << test->reason()
                << std::endl;
        }
    }

    output << "---------------\n";
    if (numFailed == 0)
    {
        output << "All tests passed."
            << std::endl;
    }
    else
    {
        output << "Tests passed: " << numPassed
            << "\nTests failed: " << numFailed
            << std::endl;
    }

    return numFailed;
}

} // namespace MereTDD

#define MERETDD_CLASS_FINAL( line ) Test ## line
#define MERETDD_CLASS_RELAY( line ) MERETDD_CLASS_FINAL( line )
#define MERETDD_CLASS MERETDD_CLASS_RELAY( __LINE__ )

#define MERETDD_INSTANCE_FINAL( line ) test ## line
#define MERETDD_INSTANCE_RELAY( line ) MERETDD_INSTANCE_FINAL( line )
#define MERETDD_INSTANCE MERETDD_INSTANCE_RELAY( __LINE__ )

#define TEST( testName ) \
class MERETDD_CLASS : public MereTDD::TestBase \
{ \
public: \
    MERETDD_CLASS (std::string_view name) \
    : TestBase(name) \
    { \
        MereTDD::getTests().push_back(this); \
    } \
    void run () override; \
}; \
MERETDD_CLASS MERETDD_INSTANCE(testName); \
void MERETDD_CLASS::run ()

#endif // MERETDD_TEST_H
