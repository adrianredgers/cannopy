
#ifndef CANNOPY_ANNLIB_H
#define CANNOPY_ANNLIB_H

#include <Adrian.h>
namespace cannopy {

    class RAMNeuron {

    public:
        IntType MAX_INPUTS = 20;

        RAMNeuron(IntType numInputs);

        ~RAMNeuron();

        void reset();

        void set(IntPtr address, IntType value);

        IntType lookup(IntPtr address);

    };

}
#endif //CANNOPY_ANNLIB_H
