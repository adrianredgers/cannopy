#ifndef CANNOPY_ADRIAN_P_H
#define CANNOPY_ADRIAN_P_H

namespace cannopy {

#define MALLOC(TYPE, NUMBER)    ((TYPE *) malloc((NUMBER) * sizeof(TYPE)))
#define FREE(PTR)               {if((PTR) != NULL) {free(PTR); PTR = NULL;}}
#define BURN(PTR, COUNT)        {if((PTR) != NULL) {for(long i=0;i<(COUNT); i++){(PTR)[i] = 0;} FREE(PTR);}}

}
#endif // CANNOPY_ADRIAN_P_H
