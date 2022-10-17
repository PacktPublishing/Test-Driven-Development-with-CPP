#include "../Test.h"

TEST("Test can be created")
{
}

// This test should fail because it throws an
// unexpected exception.
TEST("Test that throws unexpectedly can be created")
{
    setExpectedFailureReason(
        "Unexpected exception thrown.");

    throw "Unexpected";
}

// This test should throw an unexpected exception
// but it doesn't. We need to somehow let the user
// know what happened. This will result in a missed failure.
TEST("Test that should throw unexpectedly can be created")
{
    setExpectedFailureReason(
        "Unexpected exception thrown.");
}

TEST_EX("Test with throw can be created", int)
{
    throw 1;
}

// This test should fail because it does not throw
// an exception that it is expecting to be thrown.
TEST_EX("Test that never throws can be created", int)
{
    setExpectedFailureReason(
        "Expected exception type int was not thrown.");
}

// This test should fail because it throws an
// exception that does not match the expected type.
TEST_EX("Test that throws wrong type can be created", int)
{
    setExpectedFailureReason(
        "Unexpected exception thrown.");

    throw "Wrong type";
}
