#ifndef CANNOPY_ADRIAN_H
#define CANNOPY_ADRIAN_H

namespace cannopy {

    typedef long IntType;
    typedef long *IntPtr;
    typedef double FloatType;

#define MALLOC(TYPE, NUMBER)    ((TYPE *) malloc((NUMBER) * sizeof(TYPE)))
#define FREE(PTR)               {if((PTR) != NULL) {free(PTR); PTR = NULL;}}
#define BURN(PTR, COUNT)        {if((PTR) != NULL) {for(long i=0;i<(COUNT); i++){(PTR)[i] = 0;} FREE(PTR);}}

}
#endif // CANNOPY_ADRIAN_H
