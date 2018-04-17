
// #include <cstdio>
// #include <cstdlib>
#include <iostream>
#include <Adrian.h>
#include <Adrian_p.h>
#include "RAMNeuron_p.h"
#include <ANNLib.h>

using namespace std;
namespace cannopy {

    RAMNeuron::RAMNeuron(IntType numInputs) {
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


    void RAMNeuron::set(AddressRef address, IntType value) {
        mData->set(address, value);
    }

    IntType RAMNeuron::lookup(AddressRef address) {
        mData->lookup(address);
    }


    RAMNeuronPrivate::RAMNeuronPrivate(IntType numInputs) {
        if (numInputs < 0 || numInputs > RAMNeuron::MAX_INPUTS) {
            cerr << "ERROR: Number of inputs " << numInputs << " must be between 0 and " << RAMNeuron::MAX_INPUTS;
            exit(1);
        }

        this->numInputs = numInputs;
        numAddresses = 1 << numInputs;
        contents = MALLOC(IntType, numAddresses);
        if (contents == NULL) {
            cerr << "ERROR: Could not allocate space for " << numAddresses;
            cout << "OUT ERROR: Could not allocate space for " << numAddresses;
            exit(1);
        }

        reset();
    }

    RAMNeuronPrivate::~RAMNeuronPrivate() {
        BURN(contents, numAddresses);
        numAddresses = 0;
        numInputs = 0;

    }

    void RAMNeuronPrivate::reset() {
        if (contents != NULL) {
            reset(contents, numAddresses);
        }
    }

    void RAMNeuronPrivate::reset(IntPtr contents, IntType numAddresses) {
        while (numAddresses-- > 0) {
            *contents++ = 0;
        }
    }

    void RAMNeuronPrivate::set(AddressRef address, IntType value) {
        IntType addressOffset = getOffset(address);
        contents[addressOffset] = value;
    }

    IntType RAMNeuronPrivate::lookup(AddressRef address) {
        IntType addressOffset = getOffset(address);
        return contents[addressOffset];
    }

    IntType RAMNeuronPrivate::getOffset(AddressRef address) {
        IntType addressOffset = 0;
        IntType addressSection = 1;
        for(auto iter = address.begin(); iter != address.end(); ++iter) {
//            cout << "itesr " << *iter << endl;
            addressOffset += *iter * addressSection;
            addressSection <<= 1;

        }
        return addressOffset;
    }


}
