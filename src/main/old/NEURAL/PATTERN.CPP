#include "neural.hpp"
#include <iostream.h>
#include <stdlib.h>

Pattern::getMem(int inSize, int outSize)
{
    in = new float[inSize];
    if(!in)
    {
        cerr<<"Error during memory allocation of Pattern.in[]!\n";
        return(ALLOC_ERR);
    }
    out = new int[outSize];
    if(!out)
    {
        cerr<<"Error during memory allocation of Pattern.out[]!\n";
        return(ALLOC_ERR);
    }
    return true;
}
