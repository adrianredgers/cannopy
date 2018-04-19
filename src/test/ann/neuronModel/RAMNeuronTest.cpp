
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

    // When:
    UIntType actual = ram.lookup(address);

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
    UIntType actual = ram.lookup(address);
    EXPECT_EQ(1, actual) << "Testing RAM(0,1)";
}

TEST(RAMNeuronTest, SetAddressFail) {
    // Given:
    RAMNeuron ram = RAMNeuron(2);
    AddressType address;
    address.push_back(1);
    address.push_back(0);
    address.push_back(0);

    try {
        // When:
        ram.set(address, 1);

        FAIL() << "Should throw exception";
    } catch(invalid_argument e) {
        // Then:
        string expected = string("ERROR : arg (address size) has value (3) that should be (2)");
        EXPECT_EQ(expected, e.what());
    } catch(...) {
        FAIL() << "Should have thrown invalid_argument exception";

    }
}


TEST(RAMNeuronTest, SetContentsFail) {
    // Given:
    RAMNeuron ram = RAMNeuron(2);
    AddressType address;
    address.push_back(1);
    address.push_back(0);

    try {
        // When:
        ram.set(address, 2);

        FAIL() << "Should throw exception";
    } catch(invalid_argument e) {
        // Then:
        string expected = string("ERROR : arg (contents value) has value (2) that should be between (0) and (1) inclusive");
        EXPECT_EQ(expected, e.what());
    } catch(...) {
        FAIL() << "Should have thrown invalid_argument exception";

    }
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
    UIntType actual = ram.lookup(address);
    EXPECT_EQ(0, actual) << "Testing RAM(0,1)";
}
