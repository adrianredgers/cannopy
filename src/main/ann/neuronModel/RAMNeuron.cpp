
// #include <cstdio>
// #include <cstdlib>
#include <iostream>
#include <Adrian.h>

using namespace std;
namespace cannopy {

    class RAMNeuron {

    public:
        static const IntType MAX_INPUTS = 20;

        RAMNeuron(IntType numInputs);

        ~RAMNeuron();

        void reset();

        IntType lookup(IntType *address);

        void set(IntPtr address, IntType value);

    private:
        IntType numInputs;
        IntType numAddresses;
        IntPtr contents;

        void reset(IntPtr contents, IntType numAddresses);

        IntType getOffset(IntPtr address);
    };


    RAMNeuron::RAMNeuron(IntType numInputs) {
        if (numInputs < 0 || numInputs > MAX_INPUTS) {
            cout << "OUT ERROR: Number of inputs " << numInputs << " must be between 0 and " << MAX_INPUTS;
            cerr << "ERROR: Number of inputs " << numInputs << " must be between 0 and " << MAX_INPUTS;
//        exit(1);
        }

        this->numInputs = numInputs;
        numAddresses = 1 << numInputs;
        contents = MALLOC(IntType, numAddresses);
        if (contents == NULL) {
            cerr << "ERROR: Could not allocate space for " << numAddresses;
            cout << "OUT ERROR: Could not allocate space for " << numAddresses;
//        exit(1);
        }

        reset();
    }

    RAMNeuron::~RAMNeuron() {
        BURN(contents, numAddresses);
        numAddresses = 0;
        numInputs = 0;

    }

    void RAMNeuron::reset() {
        if (contents != NULL) {
            reset(contents, numAddresses);
        }
    }

    void RAMNeuron::reset(IntPtr contents, IntType numAddresses) {
        while (numAddresses-- > 0) {
            *contents++ = 0;
        }
    }

    void RAMNeuron::set(IntPtr address, IntType value) {
        IntType addressOffset = getOffset(address);
        contents[addressOffset] = value;
    }

    IntType RAMNeuron::lookup(IntPtr address) {
        IntType addressOffset = getOffset(address);
        return contents[addressOffset];
    }

    IntType RAMNeuron::getOffset(IntPtr address) {
        IntType addressOffset = 0;
        IntType addressSection = 1;
        for (IntType inputLine = 0; inputLine < numInputs; inputLine++, address++) {
            addressOffset += *address * addressSection;
            addressSection <<= 1;
        }
        return addressOffset;
    }


}
