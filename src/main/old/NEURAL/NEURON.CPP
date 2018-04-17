#include "neural.hpp"
#include <math.h>

Neuron::Neuron(void)
{
    input = 0;
    output = 0;
    error = 0;
}


void Neuron::transfer(void)
{
    //This is the "classic" transfer function.
    output = 1 / (1 + exp(-1 * M_FACTOR * input));
}


float Neuron::derivTransfer(float weightedSumOfErrors)
{
    error = output * (1 - output) * weightedSumOfErrors;
    return error;
}
