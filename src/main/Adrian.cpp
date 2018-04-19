
#include <Adrian.h>
#include "Adrian_p.h"
#include <iostream>

using namespace std;

namespace cannopy {

    unsigned int MAX_UINT = numeric_limits<unsigned int>::max();


    Arg::Arg() {}

    Arg::Arg(const string &varName) {
        this->varName = varName;
    }

    Arg &Arg::min(UIntType minValue) {
        this->minValue = minValue;
        return *this;
    }

    Arg &Arg::exactly(UIntType exactValue) {
        this->minValue = exactValue;
        this->maxValue = exactValue;
        return *this;
    }

    Arg &Arg::max(UIntType maxValue) {
        this->maxValue = maxValue;
        return *this;
    }


    string &Arg::check(UIntType value) {

        if ((minValue != MAX_UINT && value < minValue) ||
            (maxValue != MAX_UINT && value > maxValue)) {

            string *errorMessage = new string("ERROR : arg (");
            errorMessage->append(varName).append(") has value (")
                    .append(to_string(value))
                    .append(") that should be ");
            if (minValue == MAX_UINT) {
                errorMessage->append("less than or equal to (").append(to_string(maxValue)).append(+")");
            } else if (maxValue == MAX_UINT) {
                errorMessage->append("more than or equal to (").append(to_string(minValue)).append(+")");
            } else if (maxValue == minValue) {
                errorMessage->append("(").append(to_string(minValue)).append(+")");
            } else {
                errorMessage->append("between (")
                        .append(to_string(minValue)).append(+") and (").append(to_string(maxValue)).append(
                                +") inclusive");
            }

            throw invalid_argument(*errorMessage);
        }
        string *msg = new string("");
        return *msg;
    }


}