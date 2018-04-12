
#include <gtest/gtest.h>
#include <NumerLib.h>
#include <Adrian.h>
#include <TestUtils.h>

using namespace cannopy;

TEST(NumericalTest, HappyCase) {
    FloatType fac = factorial(5);
    EXPECT_NEAR(120, fac, DTOL) << "Testing factorial 5";
}

TEST(NumericalTest, Negative) {
    EXPECT_GT(factorial(-10), 0);
}

TEST(NumericalTest, Near) {
    EXPECT_TRUE(DEqual(1.000000001, 1.000000000));
}


