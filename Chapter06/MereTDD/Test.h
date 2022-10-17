#ifndef MERETDD_TEST_H
#define MERETDD_TEST_H

#include <ostream>
#include <string_view>
#include <vector>

namespace MereTDD
{

class ConfirmException
{
public:
    ConfirmException (int line)
    : mLine(line)
    { }

    virtual ~ConfirmException () = default;

    std::string_view reason () const
    {
        return mReason;
    }

    int line () const
    {
        return mLine;
    }

protected:
    std::string mReason;
    int mLine;
};

class BoolConfirmException : public ConfirmException
{
public:
    BoolConfirmException (bool expected, int line)
    : ConfirmException(line)
    {
        mReason += "    Expected: ";
        mReason += expected ? "true" : "false";
    }
};

class ActualConfirmException : public ConfirmException
{
public:
    ActualConfirmException (
        std::string_view expected,
        std::string_view actual,
        int line)
    : ConfirmException(line),
      mExpected(expected),
      mActual(actual)
    {
        formatReason();
    }

private:
    void formatReason ()
    {
        mReason += "    Expected: " + mExpected + "\n";
        mReason += "    Actual  : " + mActual;
    }

    std::string mExpected;
    std::string mActual;
};

class MissingException
{
public:
    MissingException (std::string_view exType)
    : mExType(exType)
    { }

    std::string_view exType () const
    {
        return mExType;
    }

private:
    std::string mExType;
};

inline void confirm (
    bool expected,
    bool actual,
    int line)
{
    if (actual != expected)
    {
        throw BoolConfirmException(expected, line);
    }
}

inline void confirm (
    std::string_view expected,
    std::string_view actual,
    int line)
{
    if (actual != expected)
    {
        throw ActualConfirmException(
            expected,
            actual,
            line);
    }
}

inline void confirm (
    std::string const & expected,
    std::string const & actual,
    int line)
{
    confirm(
        std::string_view(expected),
        std::string_view(actual),
        line);
}

inline void confirm (
    float expected,
    float actual,
    int line)
{
    if (actual < (expected - 0.0001f) ||
        actual > (expected + 0.0001f))
    {
        throw ActualConfirmException(
            std::to_string(expected),
            std::to_string(actual),
            line);
    }
}

inline void confirm (
    double expected,
    double actual,
    int line)
{
    if (actual < (expected - 0.000001) ||
        actual > (expected + 0.000001))
    {
        throw ActualConfirmException(
            std::to_string(expected),
            std::to_string(actual),
            line);
    }
}

inline void confirm (
    long double expected,
    long double actual,
    int line)
{
    if (actual < (expected - 0.000001) ||
        actual > (expected + 0.000001))
    {
        throw ActualConfirmException(
            std::to_string(expected),
            std::to_string(actual),
            line);
    }
}

template <typename T>
void confirm (
    T const & expected,
    T const & actual,
    int line)
{
    if (actual != expected)
    {
        throw ActualConfirmException(
            std::to_string(expected),
            std::to_string(actual),
            line);
    }
}

class TestBase;

inline std::vector<TestBase *> & getTests ()
{
    static std::vector<TestBase *> tests;

    return tests;
}

class TestBase
{
public:
    TestBase (std::string_view name)
    : mName(name), mPassed(true), mConfirmLocation(-1)
    {
        getTests().push_back(this);
    }

    virtual ~TestBase () = default;

    virtual void runEx ()
    {
        run();
    }

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

    int confirmLocation () const
    {
        return mConfirmLocation;
    }

    std::string_view expectedReason () const
    {
        return mExpectedReason;
    }

    void setFailed (std::string_view reason, int confirmLocation = -1)
    {
        mPassed = false;
        mReason = reason;
        mConfirmLocation = confirmLocation;
    }

    void setExpectedFailureReason (std::string_view reason)
    {
        mExpectedReason = reason;
    }

private:
    std::string mName;
    bool mPassed;
    std::string mReason;
    std::string mExpectedReason;
    int mConfirmLocation;
};

template <typename ExceptionT>
class TestExBase : public TestBase
{
public:
    TestExBase (std::string_view name,
        std::string_view exceptionName)
    : TestBase(name), mExceptionName(exceptionName)
    { }

    void runEx () override
    {
        try
        {
            run();
        }
        catch (ExceptionT const &)
        {
            return;
        }

        throw MissingException(mExceptionName);
    }

private:
    std::string mExceptionName;
};

inline int runTests (std::ostream & output)
{
    output << "Running "
        << getTests().size()
        << " tests\n";

    int numPassed = 0;
    int numMissedFailed = 0;
    int numFailed = 0;
    for (auto * test: getTests())
    {
        output << "---------------\n"
            << test->name()
            << std::endl;

        try
        {
            test->runEx();
        }
        catch (ConfirmException const & ex)
        {
            test->setFailed(ex.reason(), ex.line());
        }
        catch (MissingException const & ex)
        {
            std::string message = "Expected exception type ";
            message += ex.exType();
            message += " was not thrown.";
            test->setFailed(message);
        }
        catch (...)
        {
            test->setFailed("Unexpected exception thrown.");
        }

        if (test->passed())
        {
            if (not test->expectedReason().empty())
            {
                // This test passed but it was supposed
                // to have failed.
                ++numMissedFailed;
                output << "Missed expected failure\n"
                    << "Test passed but was expected to fail."
                    << std::endl;
            }
            else
            {
                ++numPassed;
                output << "Passed"
                    << std::endl;
            }
        }
        else if (not test->expectedReason().empty() &&
            test->expectedReason() == test->reason())
        {
            ++numPassed;
            output << "Expected failure\n"
                << test->reason()
                << std::endl;
        }
        else
        {
            ++numFailed;
            if (test->confirmLocation() != -1)
            {
                output << "Failed confirm on line "
                    << test->confirmLocation() << "\n";
            }
            else
            {
                output << "Failed\n";
            }
            output << test->reason()
                << std::endl;
        }
    }

    output << "---------------\n";
    output << "Tests passed: " << numPassed
        << "\nTests failed: " << numFailed;
    if (numMissedFailed != 0)
    {
        output << "\nTests failures missed: " << numMissedFailed;
    }
    output << std::endl;

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
namespace { \
class MERETDD_CLASS : public MereTDD::TestBase \
{ \
public: \
    MERETDD_CLASS (std::string_view name) \
    : TestBase(name) \
    { } \
    void run () override; \
}; \
} /* end of unnamed namespace */ \
MERETDD_CLASS MERETDD_INSTANCE(testName); \
void MERETDD_CLASS::run ()

#define TEST_EX( testName, exceptionType ) \
namespace { \
class MERETDD_CLASS : public MereTDD::TestExBase<exceptionType> \
{ \
public: \
    MERETDD_CLASS (std::string_view name, \
        std::string_view exceptionName) \
    : TestExBase(name, exceptionName) \
    { } \
    void run () override; \
}; \
} /* end of unnamed namespace */ \
MERETDD_CLASS MERETDD_INSTANCE(testName, #exceptionType); \
void MERETDD_CLASS::run ()

#define CONFIRM_FALSE( actual ) \
    MereTDD::confirm(false, actual, __LINE__)
#define CONFIRM_TRUE( actual ) \
    MereTDD::confirm(true, actual, __LINE__)
#define CONFIRM( expected, actual ) \
    MereTDD::confirm(expected, actual, __LINE__)

#endif // MERETDD_TEST_H
