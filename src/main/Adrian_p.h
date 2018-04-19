#ifndef CANNOPY_ADRIAN_P_H
#define CANNOPY_ADRIAN_P_H

#include <iostream>

namespace cannopy {

#define MALLOC(TYPE, NUMBER)    ((TYPE *) malloc((NUMBER) * sizeof(TYPE)))
#define FREE(PTR)               {if((PTR) != NULL) {free(PTR); (PTR) = NULL;}}
#define BURN(PTR, COUNT)        {if((PTR) != NULL) {for(long i=0;i<(COUNT); i++){(PTR)[i] = 0;} FREE(PTR);}}


    extern unsigned int MAX_UINT;

    class Arg {
    public:
        std::string varName = (" ");
        UIntType minValue = MAX_UINT;
        UIntType maxValue = MAX_UINT;

        Arg();

        Arg(const std::string  &varName);

        Arg &min(UIntType minValue);

        Arg &exactly(UIntType minValue);

        Arg &max(UIntType maxValue);

        std::string &check(UIntType value);
    };

}
#endif // CANNOPY_ADRIAN_P_H
