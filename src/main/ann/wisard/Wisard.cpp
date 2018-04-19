
#include <iostream>
#include <Adrian.h>
#include "Wisard_p.h"

using namespace std;
namespace cannopy {

    Wisard::Wisard(UIntType retinaSize, UIntType coverage, UIntType arity) {
        if (arity < 0 || arity > RAMNeuron::MAX_INPUTS) {
            cerr << "ERROR: Arity = Number of inputs per node " << arity << " must be between 0 and "
                 << RAMNeuron::MAX_INPUTS;
            exit(1);
        }
        mData = new WisardPrivate(retinaSize, coverage, arity);
    }

    Wisard::~Wisard() {
        delete mData;
    }

    void Wisard::reset() {
        if (mData != NULL) {
            mData->reset();
        }
    }


    void Wisard::train(AddressRef retina) {
        mData->train(retina);
    }

    UIntType Wisard::lookup(AddressRef retina) {
        mData->lookup(retina);
    }

////////////////////////////////////////////////////////////////////

    WisardPrivate::WisardPrivate(UIntType retinaSize, UIntType coverage, UIntType arity)
//            : nodes(retinaSize * coverage / arity, RAMNeuron(arity))
    {
        this->arity = arity;
        this->retinaSize = retinaSize;
        this->coverage = coverage;
        numNodes = coverage * retinaSize / arity;
        this->nodes.reserve(numNodes);
        for (UIntType node = 0; node < numNodes; node++) {
            nodes.emplace_back(arity);
        }
        mapping.reserve(retinaSize);
        auto mappingIter = mapping.begin();
        for (UIntType c = 0; c < coverage; c++) {
            for (UIntType retinaPos = 0; retinaPos < retinaSize; retinaPos++, ++mappingIter) {
                *mappingIter = retinaPos;
            }
        }
    }

    WisardPrivate::~WisardPrivate() {
        nodes.clear();
        mapping.clear();
        coverage = 0;
        retinaSize = 0;
        arity = 0;
        numNodes = 0;
    }

    void WisardPrivate::reset() {
        for (auto &node:nodes) {
            node.reset();
        }
    }

    void WisardPrivate::train(AddressRef retina) {
        auto mappingIter = mapping.begin();
        AddressType nodeAddress;
        nodeAddress.reserve(arity);
        for (auto &node: nodes) {
            for (UIntType nodeInput = 0; nodeInput < arity; nodeInput++, ++mappingIter) {
                nodeAddress[nodeInput] = retina[*mappingIter];
            }
            node.set(nodeAddress, 1);

        }

    }

    UIntType WisardPrivate::lookup(AddressRef retina) {
        UIntType response = 0;
        auto mappingIter = mapping.begin();
        AddressType nodeAddress;
        nodeAddress.reserve(arity);
        for (auto &nodeIter: nodes) {
            nodeAddress.clear();
            for (UIntType nodeInput = 0; nodeInput < arity; nodeInput++, ++mappingIter) {
                nodeAddress.push_back(retina[*mappingIter]);
            }
            response += nodeIter.lookup(nodeAddress);

        }
        return response;
    }


}
