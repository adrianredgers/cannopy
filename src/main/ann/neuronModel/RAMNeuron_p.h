#ifndef CANNOPY_RAMNEURON_P_H
#define CANNOPY_RAMNEURON_P_H

namespace cannopy {

    class RAMNeuronPrivate {

    public:
        IntType numInputs;
        IntType numAddresses;
        IntPtr contents;

        explicit RAMNeuronPrivate(IntType numInputs);
        ~RAMNeuronPrivate();

        void reset();
        void reset(IntPtr contents, IntType numAddresses);

        void set(AddressRef address, IntType value);

        IntType lookup(AddressRef address);

        IntType getOffset(AddressRef address);
    };

}
#endif // CANNOPY_RAMNEURON_P_H
