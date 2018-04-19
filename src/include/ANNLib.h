
#ifndef CANNOPY_ANNLIB_H
#define CANNOPY_ANNLIB_H

#include <Adrian.h>

namespace cannopy {

    class RAMNeuronPrivate;

    class RAMNeuron {

    public:
        static const UIntType MAX_INPUTS = 20;

        RAMNeuron(UIntType numInputs);

        ~RAMNeuron();

        void reset();

        void set(AddressRef address, UIntType value);

        UIntType lookup(AddressRef address);

    private:
        RAMNeuronPrivate *mData;
    };


    class WisardPrivate;

    class Wisard {

    public:

        Wisard(UIntType numInputs, UIntType coverage, UIntType arity);

        ~Wisard();

        void reset();

        void train(AddressRef retina);

        UIntType lookup(AddressRef retina);

    private:
        WisardPrivate *mData;
    };

}
#endif //CANNOPY_ANNLIB_H
