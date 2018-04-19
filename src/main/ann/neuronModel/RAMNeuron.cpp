
// #include <cstdio>
// #include <cstdlib>
#include <iostream>
#include <Adrian.h>
#include <Adrian_p.h>
#include "RAMNeuron_p.h"
#include <ANNLib.h>

using namespace std;
namespace cannopy {

    RAMNeuron::RAMNeuron(UIntType numInputs) {
        mData = new RAMNeuronPrivate(numInputs);
    }

    RAMNeuron::~RAMNeuron() {
        delete mData;
    }

    void RAMNeuron::reset() {
        if (mData != NULL) {
            mData->reset();
        }
    }

    void RAMNeuron::set(AddressRef address, UIntType value) {
        mData->set(address, value);
    }

    UIntType RAMNeuron::lookup(AddressRef address) {
        mData->lookup(address);
    }

    ////////////////////////////////////////////////////////////////////

    RAMNeuronPrivate::RAMNeuronPrivate(UIntType numInputs) {
        Arg("numInputs").min(0).max(RAMNeuron::MAX_INPUTS).check(numInputs);

        this->numInputs = numInputs;
        numAddresses = 1u << numInputs;
        contents.reserve(numAddresses);

        reset();
    }

    RAMNeuronPrivate::~RAMNeuronPrivate() {
        reset();
        numAddresses = 0;
        numInputs = 0;

    }

    void RAMNeuronPrivate::reset() {
        for(UIntType addr = 0; addr<numAddresses;addr++) {
            contents[addr] = 0;
        }
    }

    void RAMNeuronPrivate::set(AddressRef address, UIntType value) {
        Arg("address size").exactly(numInputs).check(address.size());
        Arg("contents value").min(0).max(1).check(value);
        UIntType addressOffset = getOffset(address);
        contents[addressOffset] = value;
    }

    UIntType RAMNeuronPrivate::lookup(AddressRef address) {
        UIntType addressOffset = getOffset(address);
        return contents[addressOffset];
    }

    UIntType RAMNeuronPrivate::getOffset(AddressRef address) {
        UIntType addressOffset = 0;
        UIntType addressSection = 1;
        for (UIntType inputNum=0; inputNum<numInputs; inputNum++) {
            addressOffset += address[inputNum] * addressSection;
            addressSection <<= 1;

        }
        return addressOffset;
    }


}
