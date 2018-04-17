
#ifndef CANNOPY_ANNLIB_H
#define CANNOPY_ANNLIB_H

#include <Adrian.h>

namespace cannopy {

    class RAMNeuronPrivate;

    class RAMNeuron {

    public:
        static const IntType MAX_INPUTS = 20;

        RAMNeuron(IntType numInputs);

        ~RAMNeuron();

        void reset();

        void set(AddressRef address, IntType value);

        IntType lookup(AddressRef address);

    private:
        RAMNeuronPrivate *mData;
    };

}
#endif //CANNOPY_ANNLIB_H
