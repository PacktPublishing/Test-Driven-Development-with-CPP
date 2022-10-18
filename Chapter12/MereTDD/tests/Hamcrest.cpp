#include "../Test.h"

using namespace MereTDD;

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
}
