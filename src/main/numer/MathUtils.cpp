
#include <Adrian.h>
#include <cmath>
#include <stdexcept>
#include <iostream>

#include "MathUtils.h"
using namespace std;
namespace cannopy {

    FloatType factorial(IntType n, FloatType acc);

    FloatType factorial(IntType n) {
        FloatType fac = factorial(n, 1.0);
        return fac;
    }

    FloatType factorial(IntType n, FloatType acc) {
        if (n <= 1) {
            return acc;
        } else {
            FloatType fac = factorial(n - 1, n * acc);
            return fac;
        }
    }

    /**
     * Return true if floating point values a and b are close, else false.
     * 'close' means relative distance is within DTOL, (or else both a and b are close to 0).
     */
    bool DEqual(FloatType a, FloatType b) {
        if (a < b) { return DEqual(b, a); }
        // So now we know a >= b, although a could still be negative.
        FloatType c = fabs(a);
        // Guard that abs(a) is non-zero before dividing by it.
        return (c <= DTOL) || ((a - b) / c <= DTOL);
    }


}