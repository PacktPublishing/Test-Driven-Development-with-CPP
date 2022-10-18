#include "../Test.h"

using namespace MereTDD;

template <typename T>
T calculateFraction (T input)
{
    T denominator {10};
    return input / denominator;
}

template <typename T>
T accumulateError (T input)
{
    // First add many small amounts.
    T partialAmount {0.1};
    for (int i = 0; i < 10; ++i)
    {
        input += partialAmount;
    }
    // Then subtract to get back to the original.
    T wholeAmount {1};
    input -= wholeAmount;
    return input;
}

template <typename T>
int performComparisons (int totalCount)
{
    int passCount {0};
    for (int i = 0; i < totalCount; ++i)
    {
        T expected = static_cast<T>(i);
        expected = calculateFraction(expected);
        T actual = accumulateError(expected);
        if (compareEq(actual, expected))
        {
            ++passCount;
        }
    }
    return passCount;
}

TEST("Test can use hamcrest style confirm")
{
    int ten = 10;
    CONFIRM_THAT(ten, Equals(10));
}

TEST("Test hamcrest style confirm failure")
{
    std::string reason = "    Expected: 9\n";
    reason += "    Actual  : 10";
    setExpectedFailureReason(reason);

    int ten = 10;
    CONFIRM_THAT(ten, Equals(9));
}

TEST("Test other hamcrest style integer confirms")
{
    char c1 = 'A';
    char c2 = 'A';
    CONFIRM_THAT(c1, Equals(c2));
    CONFIRM_THAT(c1, Equals('A'));

    short s1 = 10;
    short s2 = 10;
    CONFIRM_THAT(s1, Equals(s2));
    CONFIRM_THAT(s1, Equals(10));

    unsigned int ui1 = 3'000'000'000;
    unsigned int ui2 = 3'000'000'000;
    CONFIRM_THAT(ui1, Equals(ui2));
    CONFIRM_THAT(ui1, Equals(3'000'000'000));

    long long ll1 = 5'000'000'000'000LL;
    long long ll2 = 5'000'000'000'000LL;
    CONFIRM_THAT(ll1, Equals(ll2));
    CONFIRM_THAT(ll1, Equals(5'000'000'000'000LL));
}

TEST("Test hamcrest style bool confirms")
{
    bool b1 = true;
    bool b2 = true;
    CONFIRM_THAT(b1, Equals(b2));

    // This works but probably won't be used much.
    CONFIRM_THAT(b1, Equals(true));

    // When checking a bool variable for a known value,
    // the classic style is probably better.
    CONFIRM_TRUE(b1);
}

TEST("Test hamcrest style string confirms")
{
    std::string s1 = "abc";
    std::string s2 = "abc";
    CONFIRM_THAT(s1, Equals(s2));       // string vs. string
    CONFIRM_THAT(s1, Equals("abc"));    // string vs. literal
    CONFIRM_THAT("abc", Equals(s1));    // literal vs. string

    // Probably not needed, but this works too.
    CONFIRM_THAT("abc", Equals("abc")); // literal vs. literal

    std::string s3 = "def";
    CONFIRM_THAT(s1, NotEquals(s3));       // string vs. string
    CONFIRM_THAT(s1, NotEquals("def"));    // string vs. literal
    CONFIRM_THAT("def", NotEquals(s1));    // literal vs. string
}

TEST("Test hamcrest style string pointer confirms")
{
    char const * sp1 = "abc";
    std::string s1 = "abc";
    char const * sp2 = s1.c_str();    // avoid sp1 and sp2 being same
    CONFIRM_THAT(sp1, Equals(sp2));   // pointer vs. pointer
    CONFIRM_THAT(sp2, Equals("abc")); // pointer vs. literal
    CONFIRM_THAT("abc", Equals(sp2)); // literal vs. pointer
    CONFIRM_THAT(sp1, Equals(s1));    // pointer vs. string
    CONFIRM_THAT(s1, Equals(sp1));    // string vs. pointer

    char const * sp3 = "def";
    CONFIRM_THAT(sp1, NotEquals(sp3));   // pointer vs. pointer
    CONFIRM_THAT(sp1, NotEquals("def")); // pointer vs. literal
    CONFIRM_THAT("def", NotEquals(sp1)); // literal vs. pointer
    CONFIRM_THAT(sp3, NotEquals(s1));    // pointer vs. string
    CONFIRM_THAT(s1, NotEquals(sp3));    // string vs. pointer
}

TEST("Test many float comparisons")
{
    int totalCount {1'000};
    int passCount = performComparisons<float>(totalCount);
    CONFIRM_THAT(passCount, Equals(totalCount));
}

TEST("Test many double comparisons")
{
    int totalCount {1'000};
    int passCount = performComparisons<double>(totalCount);
    CONFIRM_THAT(passCount, Equals(totalCount));
}

TEST("Test many long double comparisons")
{
    int totalCount {1'000};
    int passCount = performComparisons<long double>(totalCount);
    CONFIRM_THAT(passCount, Equals(totalCount));
}

TEST("Test small float values")
{
    // Based on float epsilon = 1.1920928955078125e-07
    CONFIRM_THAT(0.000001f, NotEquals(0.000002f));
}

TEST("Test large float values")
{
    // Based on float epsilon = 1.1920928955078125e-07
    CONFIRM_THAT(9'999.0f, Equals(9'999.001f));
}

TEST("Test small double values")
{
    // Based on double epsilon = 2.2204460492503130808e-16
    CONFIRM_THAT(0.000000000000001, NotEquals(0.000000000000002));
}

TEST("Test large double values")
{
    // Based on double epsilon = 2.2204460492503130808e-16
    CONFIRM_THAT(1'500'000'000'000.0, Equals(1'500'000'000'000.0003));
}

TEST("Test small long double values")
{
    // Based on double epsilon = 2.2204460492503130808e-16
    CONFIRM_THAT(0.000000000000001L, NotEquals(0.000000000000002L));
}

TEST("Test large long double values")
{
    // Based on double epsilon = 2.2204460492503130808e-16
    CONFIRM_THAT(1'500'000'000'000.0L, Equals(1'500'000'000'000.0003L));
}

TEST("Test even integral value")
{
    CONFIRM_THAT(10, IsEven<int>());
}

TEST("Test even integral value confirm failure")
{
    std::string reason = "    Expected: is even\n";
    reason += "    Actual  : 11";
    setExpectedFailureReason(reason);

    CONFIRM_THAT(11, IsEven<int>());
}
