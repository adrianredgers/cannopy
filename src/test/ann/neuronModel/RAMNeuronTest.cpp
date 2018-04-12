
#include <gtest/gtest.h>
#include <Adrian.h>
#include <NumerLib.h>
#include <ANNLib.h>
#include <python2.7/graminit.h>
#include<TestUtils.h>

using namespace std;
using namespace cannopy;

TEST(RAMNeuronTest, EmptyNode) {
    // Given:
    RAMNeuron ram = RAMNeuron(2);
    IntPtr address = MALLOC(IntType, 2);
    address[0] = 0;
    address[1] = 1;

    cout << "sizeof int = " << sizeof(int) << endl;
    cout << "sizeof long = " << sizeof(long) << endl;
    cout << "sizeof float = " << sizeof(float) << endl;
    cout << "sizeof double = " << sizeof(double) << endl;
    cout << "sizeof short = " << sizeof(short) << endl;
    cout << "sizeof char = " << sizeof(char) << endl;
    // When:
    IntType actual = ram.lookup(address);

    // Then:
    EXPECT_EQ(0, actual) << "Testing RAM(0,1)";
}

TEST(RAMNeuronTest, SetAddress) {
    // Given:
    RAMNeuron ram = RAMNeuron(2);
    IntPtr address = MALLOC(IntType, 2);
    address[0] = 0;
    address[1] = 1;

    // When:
    ram.set(address, 1);

    // Then:
    int actual = ram.lookup(address);
    EXPECT_EQ(1, actual) << "Testing RAM(0,1)";
}

TEST(RAMNeuronTest, Reset) {
    // Given:
    RAMNeuron ram = RAMNeuron(2);
    IntPtr address = MALLOC(IntType, 2);
    address[0] = 0;
    address[1] = 1;
    ram.set(address, 1);

    // When:
    ram.reset();

    // Then:
    int actual = ram.lookup(address);
    EXPECT_EQ(0, actual) << "Testing RAM(0,1)";
}
