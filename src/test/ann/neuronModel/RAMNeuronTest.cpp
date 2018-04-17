
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
    AddressType address;
    address.push_back(1);
    address.push_back(0);

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
    AddressType address;
    address.push_back(1);
    address.push_back(0);

    // When:
    ram.set(address, 1);

    // Then:
    IntType actual = ram.lookup(address);
    EXPECT_EQ(1, actual) << "Testing RAM(0,1)";
}

TEST(RAMNeuronTest, Reset) {
    // Given:
    RAMNeuron ram = RAMNeuron(2);
    AddressType address;
    address.push_back(1);
    address.push_back(0);
    ram.set(address, 1);

    // When:
    ram.reset();

    // Then:
    IntType actual = ram.lookup(address);
    EXPECT_EQ(0, actual) << "Testing RAM(0,1)";
}
