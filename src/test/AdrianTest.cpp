
#include <gtest/gtest.h>
#include <Adrian.h>
#include "../main/Adrian_p.h"
#include <TestUtils.h>
#include <NumerLib.h>

using namespace std;
using namespace cannopy;


TEST(AdrianTest, Happy) {
    try {
        // Given:
        Arg arg = Arg("test").min(1).max(3);

        // When:
        arg.check(2);

        // Then: nothing happens


    } catch (...) {
        FAIL() << "Should not have thrown exception";
    }

}



TEST(AdrianTest, MinArg) {
    try {
        // Given:
        Arg arg = Arg("test").min(2);

        // When:
        arg.check(1);

        FAIL() << "Should have thrown exception";
    } catch (invalid_argument e) {
        // Then:
        string expected = string("ERROR : arg (test) has value (1) that should be more than or equal to (2)");
        EXPECT_EQ(expected, e.what());
    } catch(...) {
        FAIL() << "Should have thrown invalid_argument exception";

    }
}


TEST(AdrianTest, MaxArg) {
    try {
        // Given:
        Arg arg = Arg("test").max(2);

        // When:
        arg.check(3);

        FAIL() << "Should have thrown exception";
    } catch (invalid_argument e) {
        // Then:
        string expected = string("ERROR : arg (test) has value (3) that should be less than or equal to (2)");
        EXPECT_EQ(expected, e.what());
    } catch(...) {
        FAIL() << "Should have thrown invalid_argument exception";

    }
}


TEST(AdrianTest, Exactly) {
    try {
        // Given:
        Arg arg = Arg("test").exactly(2);

        // When:
        arg.check(3);

        FAIL() << "Should have thrown exception";
    } catch (invalid_argument e) {
        // Then:
        string expected = string("ERROR : arg (test) has value (3) that should be (2)");
        EXPECT_EQ(expected, e.what());
    } catch(...) {
        FAIL() << "Should have thrown invalid_argument exception";

    }
}



TEST(AdrianTest, Between) {
    try {
        // Given:
        Arg arg = Arg("test").min(1).max(2);

        // When:
        arg.check(3);

        FAIL() << "Should have thrown exception";
    } catch (invalid_argument e) {
        // Then:
        string expected = string("ERROR : arg (test) has value (3) that should be between (1) and (2) inclusive");
        EXPECT_EQ(expected, e.what());
    } catch(...) {
        FAIL() << "Should have thrown invalid_argument exception";

    }

}



