#ifndef MERETDD_TEST_H
#define MERETDD_TEST_H

#include <concepts>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <ostream>
#include <string_view>
#include <type_traits>
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

constexpr float getMargin (float)
{
    // 4 is choosen to pass a reasonable amount of error.
    return std::numeric_limits<float>::epsilon() * 4;
}

constexpr double getMargin (double)
{
    // 4 is choosen to pass a reasonable amount of error.
    return std::numeric_limits<double>::epsilon() * 4;
}

constexpr long double getMargin (long double)
{
    // Use double epsilon instead of long double epsilon.
    // Double epsilon is already much bigger than
    // long double epsilon so we don't need to multiply it.
    return std::numeric_limits<double>::epsilon();
}

template <typename T>
bool compareEq (T lhs, T rhs)
{
    // Check for an exact match with operator == first.
    if (lhs == rhs)
    {
        return true;
    }

    // Subnormal diffs near zero are treated as equal.
    T diff = std::abs(lhs - rhs);
    if (diff <= std::numeric_limits<T>::min())
    {
        return true;
    }

    // The margin should get bigger with bigger absolute values.
    // We scale the margin up by the larger value or
    // leave the margin unchanged if larger is less than 1.
    lhs = std::abs(lhs);
    rhs = std::abs(rhs);
    T larger = (lhs > rhs) ? lhs : rhs;
    larger = (larger < 1.0) ? 1.0 : larger;
    return diff <= getMargin(lhs) * larger;
}

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

template <std::floating_point T>
void confirm (
    T expected,
    T actual,
    int line)
{
    if (not compareEq(actual, expected))
    {
        throw ActualConfirmException(
            std::to_string(expected),
            std::to_string(actual),
            line);
    }
}

inline std::string to_string (std::string const & str)
{
    return str;
}

template <typename ExpectedT, typename ActualT>
void confirm (
    ExpectedT const & expected,
    ActualT const & actual,
    int line)
{
    using std::to_string;
    using MereTDD::to_string;
    if (actual != expected)
    {
        throw ActualConfirmException(
            to_string(expected),
            to_string(actual),
            line);
    }
}

template <typename ActualT, typename MatcherT>
inline void confirm_that (
    ActualT const & actual,
    MatcherT const & matcher,
    int line)
{
    using std::to_string;
    using MereTDD::to_string;
    if (not matcher.pass(actual))
    {
        throw ActualConfirmException(
            to_string(matcher),
            to_string(actual),
            line);
    }
}

class Matcher
{
public:
    virtual ~Matcher () = default;
    Matcher (Matcher const & other) = delete;
    Matcher (Matcher && other) = delete;

    virtual std::string to_string () const = 0;

    Matcher & operator = (Matcher const & rhs) = delete;
    Matcher & operator = (Matcher && rhs) = delete;

protected:
    Matcher () = default;
};

inline std::string to_string (Matcher const & matcher)
{
    return matcher.to_string();
}

template <typename T>
class Equals : public Matcher
{
public:
    Equals (T const & expected)
    : mExpected(expected)
    { }

    bool pass (T const & actual) const
    {
        return actual == mExpected;
    }

    std::string to_string () const override
    {
        using std::to_string;
        using MereTDD::to_string;
        return to_string(mExpected);
    }

private:
    T mExpected;
};

template <typename T, std::size_t N> requires (
    std::is_same<char, std::remove_const_t<T>>::value)
class Equals<T[N]> : public Matcher
{
public:
    Equals (char const (& expected)[N])
    {
        memcpy(mExpected, expected, N);
    }

    bool pass (std::string const & actual) const
    {
        return actual == mExpected;
    }

    std::string to_string () const override
    {
        return std::string(mExpected);
    }

private:
    char mExpected[N];
};

template <typename T> requires (
    std::is_same<char, std::remove_const_t<T>>::value)
class Equals<T *> : public Matcher
{
public:
    Equals (char const * expected)
    : mExpected(expected)
    { }

    bool pass (std::string const & actual) const
    {
        return actual == mExpected;
    }

    std::string to_string () const override
    {
        return mExpected;
    }

private:
    std::string mExpected;
};

template <std::floating_point T>
class Equals<T> : public Matcher
{
public:
    Equals (T const & expected)
    : mExpected(expected)
    { }

    bool pass (T const & actual) const
    {
        return compareEq(actual, mExpected);
    }

    std::string to_string () const override
    {
        return std::to_string(mExpected);
    }

private:
    T mExpected;
};

template <typename T>
class NotEquals : public Matcher
{
public:
    NotEquals (T const & expected)
    : mExpected(expected)
    { }

    template <typename U>
    bool pass (U const & actual) const
    {
        return not mExpected.pass(actual);
    }

    std::string to_string () const override
    {
        return "not " + mExpected.to_string();
    }

private:
    Equals<T> mExpected;
};

template <std::integral T>
class IsEven : public Matcher
{
public:
    IsEven ()
    { }

    bool pass (T const & actual) const
    {
        return actual % 2 == 0;
    }

    std::string to_string () const override
    {
        return "is even";
    }
};

class Test;
class TestSuite;

inline std::map<std::string, std::vector<Test *>> & getTests ()
{
    static std::map<std::string, std::vector<Test *>> tests;

    return tests;
}

inline std::map<std::string, std::vector<TestSuite *>> & getTestSuites ()
{
    static std::map<std::string, std::vector<TestSuite *>> suites;

    return suites;
}

inline void addTest (std::string_view suiteName, Test * test)
{
    std::string name(suiteName);
    if (not getTests().contains(name))
    {
        getTests().try_emplace(name, std::vector<Test *>());
    }
    getTests()[name].push_back(test);
}

inline void addTestSuite (std::string_view suiteName, TestSuite * suite)
{
    std::string name(suiteName);
    if (not getTestSuites().contains(name))
    {
        getTestSuites().try_emplace(name, std::vector<TestSuite *>());
    }
    getTestSuites()[name].push_back(suite);
}

class TestBase
{
public:
    TestBase (std::string_view name, std::string_view suiteName)
    : mName(name),
      mSuiteName(suiteName),
      mPassed(true),
      mConfirmLocation(-1)
    { }

    virtual ~TestBase () = default;

    std::string_view name () const
    {
        return mName;
    }

    std::string_view suiteName () const
    {
        return mSuiteName;
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

    void setFailed (std::string_view reason, int confirmLocation = -1)
    {
        mPassed = false;
        mReason = reason;
        mConfirmLocation = confirmLocation;
    }

private:
    std::string mName;
    std::string mSuiteName;
    bool mPassed;
    std::string mReason;
    int mConfirmLocation;
};

class Test : public TestBase
{
public:
    Test (std::string_view name, std::string_view suiteName)
    : TestBase(name, suiteName)
    {
        addTest(suiteName, this);
    }

    virtual void runEx ()
    {
        run();
    }

    virtual void run () = 0;

    std::string_view expectedReason () const
    {
        return mExpectedReason;
    }

    void setExpectedFailureReason (std::string_view reason)
    {
        mExpectedReason = reason;
    }

private:
    std::string mExpectedReason;
};

template <typename ExceptionT>
class TestEx : public Test
{
public:
    TestEx (std::string_view name,
        std::string_view suiteName,
        std::string_view exceptionName)
    : Test(name, suiteName), mExceptionName(exceptionName)
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

class TestSuite : public TestBase
{
public:
    TestSuite (
        std::string_view name,
        std::string_view suiteName)
    : TestBase(name, suiteName)
    {
        addTestSuite(suiteName, this);
    }

    virtual void suiteSetup () = 0;

    virtual void suiteTeardown () = 0;
};

inline void runTest (std::ostream & output, Test * test,
    int & numPassed, int & numFailed, int & numMissedFailed)
{
    output << "------- Test: "
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

inline bool runSuite (std::ostream & output,
    bool setup, std::string const & name,
    int & numPassed, int & numFailed)
{
    for (auto & suite: getTestSuites()[name])
    {
        if (setup)
        {
            output << "------- Setup: ";
        }
        else
        {
            output << "------- Teardown: ";
        }
        output << suite->name()
            << std::endl;

        try
        {
            if (setup)
            {
                suite->suiteSetup();
            }
            else
            {
                suite->suiteTeardown();
            }
        }
        catch (ConfirmException const & ex)
        {
            suite->setFailed(ex.reason(), ex.line());
        }
        catch (...)
        {
            suite->setFailed("Unexpected exception thrown.");
        }

        if (suite->passed())
        {
            ++numPassed;
            output << "Passed"
                << std::endl;
        }
        else
        {
            ++numFailed;
            if (suite->confirmLocation() != -1)
            {
                output << "Failed confirm on line "
                    << suite->confirmLocation() << "\n";
            }
            else
            {
                output << "Failed\n";
            }
            output << suite->reason()
                << std::endl;
            return false;
        }
    }
    return true;
}

inline int runTests (std::ostream & output)
{
    output << "Running "
        << getTests().size()
        << " test suites\n";

    int numPassed = 0;
    int numMissedFailed = 0;
    int numFailed = 0;
    for (auto const & [key, value]: getTests())
    {
        std::string suiteDisplayName = "Suite: ";
        if (key.empty())
        {
            suiteDisplayName += "Single Tests";
        }
        else
        {
            suiteDisplayName += key;
        }
        output << "--------------- "
            << suiteDisplayName
            << std::endl;

        if (not key.empty())
        {
            if (not getTestSuites().contains(key))
            {
                output << "Test suite is not found."
                    << " Exiting test application."
                    << std::endl;
                return ++numFailed;
            }

            if (not runSuite(output, true, key,
                numPassed, numFailed))
            {
                output << "Test suite setup failed."
                    << " Skipping tests in suite."
                    << std::endl;
                continue;
            }
        }

        for (auto * test: value)
        {
            runTest(output, test,
                numPassed, numFailed, numMissedFailed);
        }

        if (not key.empty())
        {
            if (not runSuite(output, false, key,
                numPassed, numFailed))
            {
                output << "Test suite teardown failed."
                    << std::endl;
            }
        }
    }

    output << "-----------------------------------\n";
    output << "Tests passed: " << numPassed
        << "\nTests failed: " << numFailed;
    if (numMissedFailed != 0)
    {
        output << "\nTests failures missed: " << numMissedFailed;
    }
    output << std::endl;

    return numFailed;
}

template <typename T>
class SetupAndTeardown : public T
{
public:
    SetupAndTeardown ()
    {
        T::setup();
    }

    ~SetupAndTeardown ()
    {
        T::teardown();
    }
};

template <typename T>
class TestSuiteSetupAndTeardown :
    public T,
    public TestSuite
{
public:
    TestSuiteSetupAndTeardown (
        std::string_view name,
        std::string_view suite)
    : TestSuite(name, suite)
    { }

    void suiteSetup () override
    {
        T::setup();
    }

    void suiteTeardown () override
    {
        T::teardown();
    }
};

} // namespace MereTDD

#define MERETDD_CLASS_FINAL( line ) Test ## line
#define MERETDD_CLASS_RELAY( line ) MERETDD_CLASS_FINAL( line )
#define MERETDD_CLASS MERETDD_CLASS_RELAY( __LINE__ )

#define MERETDD_INSTANCE_FINAL( line ) test ## line
#define MERETDD_INSTANCE_RELAY( line ) MERETDD_INSTANCE_FINAL( line )
#define MERETDD_INSTANCE MERETDD_INSTANCE_RELAY( __LINE__ )

#define TEST_SUITE( testName, suiteName ) \
namespace { \
class MERETDD_CLASS : public MereTDD::Test \
{ \
public: \
    MERETDD_CLASS (std::string_view name, \
      std::string_view suite) \
    : Test(name, suite) \
    { } \
    void run () override; \
}; \
} /* end of unnamed namespace */ \
MERETDD_CLASS MERETDD_INSTANCE(testName, suiteName); \
void MERETDD_CLASS::run ()

#define TEST_SUITE_EX( testName, suiteName, exceptionType ) \
namespace { \
class MERETDD_CLASS : public MereTDD::TestEx<exceptionType> \
{ \
public: \
    MERETDD_CLASS (std::string_view name, \
        std::string_view suite, \
        std::string_view exceptionName) \
    : TestEx(name, suite, exceptionName) \
    { } \
    void run () override; \
}; \
} /* end of unnamed namespace */ \
MERETDD_CLASS MERETDD_INSTANCE(testName, suiteName, #exceptionType); \
void MERETDD_CLASS::run ()

#define TEST( testName ) \
namespace { \
class MERETDD_CLASS : public MereTDD::Test \
{ \
public: \
    MERETDD_CLASS (std::string_view name) \
    : Test(name, "") \
    { } \
    void run () override; \
}; \
} /* end of unnamed namespace */ \
MERETDD_CLASS MERETDD_INSTANCE(testName); \
void MERETDD_CLASS::run ()

#define TEST_EX( testName, exceptionType ) \
namespace { \
class MERETDD_CLASS : public MereTDD::TestEx<exceptionType> \
{ \
public: \
    MERETDD_CLASS (std::string_view name, \
        std::string_view exceptionName) \
    : TestEx(name, "", exceptionName) \
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
#define CONFIRM_THAT( actual, matcher ) \
    MereTDD::confirm_that(actual, matcher, __LINE__)

#endif // MERETDD_TEST_H
