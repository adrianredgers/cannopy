#include "neural.hpp"
#include <stdlib.h>
#include <iostream.h>

void Connection::adjust(void)
{
    //If samad Coefficient == 0 this is "classic" backprop
    //If samad Coefficient == 1 this is original "fast backprop"
    //If samad Coefficient is something else, you will usually get significant 
    //changes in learning rate.  The range 0.5 - 2.0 seems to work well 
    delta = learningConstant * n2->error * (n1->output + samadCoefficient * n1->error) + delta * momentum;
    weight += delta;
}

void Connection::displaySelf(void)
{
    cout<<"Connection delta: "<<delta<<" weight: "<<weight<<"\n";
}


void Connection::feedForward(void)
{
    n2->input += n1->output * weight;
}

void Connection::set(Neuron* nLow, Neuron* nHigh)
{
    n1 = nLow;
    n2 = nHigh;
}

void Connection::setRandom(float lC, float m, float sC)
{
    weight = (((float)rand()/RAND_MAX) - 0.5) / 5;
    delta = 0;
    learningConstant = lC;
    momentum = m;
    samadCoefficient = sC;
}


int Connection::firstNeuronIs(Neuron* n)
{

    if(n == n1)
        return 1;
    else
        return 0;
}

int Connection::secondNeuronIs(Neuron* n)
{
    if(n == n2)
        return 1;
    else
        return 0;
}
