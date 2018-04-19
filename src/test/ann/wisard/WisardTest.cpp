
#include <gtest/gtest.h>
#include <Adrian.h>
#include <NumerLib.h>
#include <ANNLib.h>
#include<TestUtils.h>

using namespace std;
using namespace cannopy;

AddressType makeAddress(UIntPtr retinaPtr, UIntType retinaSize);

UIntType retinaArray[4] = {1,1,0,0};

TEST(WisardTest, EmptyNode) {
    // Given:
    UIntType retinaSize = 4;
    UIntType coverage = 1;
    UIntType arity = 2;
    Wisard wisard = Wisard(retinaSize, coverage, arity);
    AddressType retina = makeAddress(retinaArray, retinaSize);

    // When:
    UIntType actual = wisard.lookup(retina);

    // Then:
    EXPECT_EQ(0, actual) << "Testing empty wisard";
}


TEST(WisardTest, SetRetina) {
    // Given:
    UIntType retinaSize = 4;
    UIntType coverage = 1;
    UIntType arity = 2;
    UIntType numNodes = retinaSize*coverage/arity;
    Wisard wisard = Wisard(retinaSize, coverage, arity);
    AddressType retina = makeAddress(retinaArray, retinaSize);

    // When:
    wisard.train(retina);

    // Then:
    UIntType actual = wisard.lookup(retina);
    EXPECT_EQ(numNodes, actual) << "Testing wisard(1,1,0,0)";
}

TEST(WisardTest, Reset) {
    // Given:
    UIntType retinaSize = 4;
    UIntType coverage = 1;
    UIntType arity = 2;
    Wisard wisard = Wisard(retinaSize, coverage, arity);
    AddressType retina = makeAddress(retinaArray, retinaSize);
    wisard.train(retina);

    // When:
    wisard.reset();

    // Then:
    UIntType actual = wisard.lookup(retina);
    EXPECT_EQ(0, actual) << "Testing wisard.reset()";
}


TEST(WisardTest, Generalize) {
    // Given:
    UIntType retinaArray2[4] = {0,0,1,1};
    UIntType retinaArray3[4] = {0,0,0,0};
    UIntType retinaArray4[4] = {1,0,1,0};
    UIntType retinaSize = 4;
    UIntType coverage = 1;
    UIntType arity = 2;
    UIntType numNodes = retinaSize*coverage/arity;
    Wisard wisard = Wisard(retinaSize, coverage, arity);
    wisard.train(makeAddress(retinaArray, retinaSize));
    wisard.train(makeAddress(retinaArray2, retinaSize));

    // When:
    UIntType actual = wisard.lookup(makeAddress(retinaArray3, retinaSize));


    // Then:
    EXPECT_EQ(numNodes, actual) << "Testing wisard(1,1,0,0)";


    // And when:
    actual = wisard.lookup(makeAddress(retinaArray4, retinaSize));

    // Then:
    EXPECT_EQ(0, actual) << "Testing wisard(1,1,0,0)";
}

AddressType makeAddress(UIntPtr retinaPtr, UIntType retinaSize) {
    AddressType retina;
    retina.reserve(retinaSize);
    while (retinaSize-- > 0) {
        retina.push_back(*retinaPtr++);
    }
    return retina;
}
