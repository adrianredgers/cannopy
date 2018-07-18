

#include <gtest/gtest.h>
//#include <ctime>
//#include <iostream>
#include <NumerLib.h>

using namespace std;
using namespace cannopy;

typedef unsigned long long LIntType;
typedef LIntType *LIntPtr;
typedef std::vector<UIntType> LIntVec;

TEST(VectorVsArrayTest, Scan) {
    // Given:
    LIntType arraySize = 1 << 20;
    LIntType testSize = 1 << 10;

    LIntVec vector;
    vector.reserve(arraySize);
    LIntType counter = 0;
    cout << endl <<  " setting up "  << endl;
    for(LIntType  counter = 0; counter < arraySize; ++counter) {
        vector.push_back(2*counter);
    }

    LIntType sum = 0;
    auto vEnd = vector.end();
    auto vBegin = vector.begin();
    time_t beforeVec = time(0);
    for(LIntType  t=0; t<testSize; ++t) {
        for (auto it = vBegin; it != vEnd; ++it) {
            sum += *it;
        }
    }
    time_t afterVec = time(0);

    cout << "\nVector scan test: elapsed time=" << (afterVec - beforeVec) << endl;
    LIntType expected = testSize*arraySize*(arraySize-1);
    EXPECT_EQ(sum, expected) << "Summing vector - expected " << arraySize*(arraySize-1) << " actual: " << sum << endl;


    LIntPtr array = (LIntPtr) malloc(arraySize * sizeof(LIntType));
    counter = 0;
    for(LIntPtr pos = array; counter < arraySize; ++counter, ++pos) {
        *pos = 2*counter;
    }

    sum = 0;
    time_t beforeArr = time(0);
    for(LIntType  t=0; t<testSize; ++t) {
        counter = 0;
        for(LIntPtr pos = array; counter < arraySize; ++counter, ++pos) {
            sum += *pos;
        }
    }

    time_t afterArr = time(0);
    cout << "\nArray scan test: elapsed time=" << (afterArr - beforeArr) << endl;
    EXPECT_EQ(sum, expected) << "Summing array - expected " << arraySize*(arraySize-1) << " actual: " << sum << endl;

}


TEST(VectorVsArrayTest, Setup) {
    // Given:
    LIntType arraySize = 1 << 20;
    LIntType testSize = 1 << 10;


    time_t beforeVec = time(0);
    for(LIntType  t=0; t<testSize; ++t) {
        LIntVec vector;
        vector.reserve(arraySize);
        for (LIntType counter = 0; counter < arraySize; ++counter) {
            vector.push_back(2 * counter);
        }
    }
    time_t afterVec = time(0);
    cout << "\nVector setup test: elapsed time=" << (afterVec - beforeVec) << endl;


    time_t beforeArr = time(0);
    for(LIntType  t=0; t<testSize; ++t) {
        LIntPtr array = (LIntPtr) malloc(arraySize * sizeof(LIntType));
        LIntType counter = 0;
        for (LIntPtr pos = array; counter < arraySize; ++counter, ++pos) {
            *pos = 2 * counter;
        }
        free(array);
    }
    time_t afterArr = time(0);
    cout << "\nArray setup test: elapsed time=" << (afterArr - beforeArr) << endl;


}


