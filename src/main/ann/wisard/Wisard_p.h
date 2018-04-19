#ifndef CANNOPY_WISARD_P_H
#define CANNOPY_WISARD_P_H
#include <ANNLib.h>

using namespace std;

namespace cannopy {

    class WisardPrivate {

    public:
        UIntType retinaSize;
        UIntType numNodes;
        UIntType arity;
        UIntType coverage;
        vector<RAMNeuron> nodes;
        UIntVec mapping;

        WisardPrivate(UIntType retinaSize, UIntType coverage, UIntType arity);
        ~WisardPrivate();

        void reset();

        void train(AddressRef retina);

        UIntType lookup(AddressRef retina);

    };

}
#endif // CANNOPY_WISARD_P_H
