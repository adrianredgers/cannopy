
#include <Adrian.h>
#include <cmath>

#include "MathUtils.h"
namespace cannopy {

    FloatType factorial(UIntType n, FloatType acc);

    FloatType factorial(UIntType n) {
        FloatType fac = factorial(n, 1.0);
        return fac;
    }

    FloatType factorial(UIntType n, FloatType acc) {
        if (n <= 1) {
            return acc;
        } else {
            FloatType fac = factorial(n - 1, n * acc);
            return fac;
        }
    }


    UIntType DEqual(FloatType a, FloatType b) {
        if (a < b) { return DEqual(b, a); }
        FloatType c = fabs(a);
        return (c <= DTOL) || ((a - b) / c <= DTOL);
    }


}