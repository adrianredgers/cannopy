#ifndef CANNOPY_RAMNEURON_P_H
#define CANNOPY_RAMNEURON_P_H

namespace cannopy {


    class RAMNeuronPrivate {

    public:
        UIntType numInputs;
        UIntType numAddresses;
        UIntVec contents;

        explicit RAMNeuronPrivate(UIntType numInputs);

        ~RAMNeuronPrivate();

        void reset();

        void set(AddressRef address, UIntType value);

        UIntType lookup(AddressRef address);

        UIntType getOffset(AddressRef address);
    };

}
#endif // CANNOPY_RAMNEURON_P_H
