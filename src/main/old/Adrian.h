/*      Adrian.h        AGCR        31/1/89                     */
/*                                                              */
/*                  General Inclusions File                     */
/*                                                              */

#include <stdio.h>
#include <math.h>

/* *** CONSTANTS *** */
/* This File is Present */
#define ADRIAN_H        1

/* System Stuff */
#define SCREENWIDTH     80
#define TABSTOP         4
#define MAXENTRY        255
#define BELL            '\a'
#define PRINTING_CHAR   32
#define WORD_SIZE       8           /* one day it will increase... */

/* Stuff for PLN's and GRAM's*/
#define ONE             2
#define ZERO            0
#define U               1
#define HIGHFLAG        16          /* Sets 5th bit high for updating Grams */
#define LO              -1
#define HI              1
#define REWARD          1
#define PUNISH          -1

/* Printing stuff */
#define chOne           '1'
#define chZero          '0'
#define chU             '.'
#define MSB             10000
#define MSB_2           (MSB/2)

/* Numerical Constants */
#define PI              3.14159265385
#define E               2.718281828
#define TOL             0.0000001  /* For comparing float's or double's */
#define BIG             100000000   /* looks pretty big to me... */
#define RANDOMBIT       RandomBit()
                                    /* different each time it's called */


/* *** C ALIASES *** */

#define FOREVER         for(;;)

/* Flags */
#define TRUE            1
#define FALSE           0
#define UNSET           2           		/* BEWARE: "if(UNSET)" is true! */
#define NOWORD          -1          		/* no word found in line by
                                        		subroutine getword() */

/* Comparison Operators - resembling FORTRAN */
#define EQ ==
#define NE !=
#define GT >
#define LT <
#define GE >=
#define LE <=
#define NOT !
#define AND &&
#define OR ||
#define LNOT !              				/* L for LOJIKAL */
#define LAND &&
#define LOR ||
#define BNOT ~								/* B for BIT */
#define BAND &
#define BOR |
#define BXOR ^


/* *** MACROS *** */

/* NOTE:    "###" <- means BEWARE argument(s) may be evaluated twice   */

/* Bit Operations */
/* NOTE:    j = i<<3    =>   j= i*8 (multiply by 8):   a<<b = a*(2^b)  */
/*          A and B start from 0                                       */
#define BitOf(A,B)  (((B)&(1<<(A)))>>(A))
                                    /* ### Returns A'th bit of B ### */
#define FlipBit(A,B)    ((B)^(1<<(A)))
                                    /* Flips A'th bit of B */
#define IsOn(A,B) ((B)&(1<<(A))) 	/* true if A'th bit of B is set */
#define Pow2(A) (1<<(A))            /* 2^A = num locations in a RAM etc */

/* Simple Maths Functions */
#define Max(A,B)        ((A)>(B) ? (A) : (B))
                                    /* ### */
#define Min(A,B)        ((A)<(B) ? (A) : (B))
                                    /* ### */
#define Thresh(A)       ((A)>0 ?  1 : 0)
                                    /* Threshold function */
#define Abs(A)          ((A)>0 ? (A) : -(A))
                                    /* ### Absolute value of A ### */
#define Equal(A,B)  (Abs((A)-(B)) < TOL ? TRUE : FALSE )
#define Sgn(A)          (Equal((A),0) ? 0 : ( (A) > 0 ? 1 : -1 ))
#define DoFunc(A,B,C,D) ((C) ? ((B)((A),(C),(D))) : ((B)((A))))
                                    /* ### Evaluating functions with variable
                                        numbers of arguments ### */
/* eg1 DoFunc(x,Poly,4,params) => (4)? => ((Poly)((x),(4),(params)))
   eg2 DoFunc(x,sin,0,NULL) => (0)? => ((sin)(x))                       */


/* *** Typedef's **** */
typedef unsigned int Bool;  /* for use when we don't want signs
                               getting in the way (e.g with bit operations).
                               Don't forget to use %u in printf() statements. */




/* Discriminator contents type */
typedef int DConts;
/*typedef double DConts;*/


/* *** STRUCTS *** */

struct CharList
{ /* struct for making a linked list of char's (or int's) */
    char ch;                         /* this character */
    struct CharList *next;          /* pointer to next character in the list */
};


typedef struct
{
    double  (*func)();
    int     nump;
    double  *params;
    char    *funcName;
} Function;


/* To save on parameter passing: */
typedef struct
{
    int ntuple;                         /* ntuple size */
    int ramSize;                        /* = 1<<ntuple */
    DConts *nodes;                      /* discriminator contents */
    int discrimSize;                    /* = numRams*ramSize */
    int numRams;                        /* numRams in the discriminator */
    int numDiscrims;                    /* in the network */
} Discriminator;


typedef struct
{
	double (*squash)();					/* pointer to squashing function */
	double lambda;						/* steepness param for squash fn */
    int numActivns;                     /* no. activn states - default 2 */
    double *activnProb;                 /* list of activation probs */
	int middleVal;						/* middle activation value */
} ActivationFunc;


typedef struct
{
    int *list;                       	/* storage for mapping */
    Bool sameMaps;                  	/* if so then can optimize */
    int *address;                       /* optimizing sameMappings */
    int mapSize;                        /* size of mapping */
    double coverage;                    /* default = 1.0  = 100% */
    int numMaps;  		                /* in the network */
} Mapping;


typedef struct
{
    int nRows;                          /* in the image */
    int nCols;                          /*  "  "    "   */
    FILE *outFile;                      /* default output to stdout */
} Display;


typedef struct
{
    int retSize;                        /* size of input image */
	int *inputs;						/* the retina */
} Retina;


typedef struct
{
    double *dRetina;                    /* for reversing */
    int *iRetina;                       /*  "       "    */
    int pixel;                          /* which pixel are we talking about */
    double normal;                      /* normalization const for reversing */
    double th;                          /* threshold for reversing */
} Reverse;


typedef struct
{
    int numEgs;                         /* no. of images to deal with */
    int numToDo;                        /* no. of discrims to deal with */
    DConts  start;
    int offset;
    int *retina;
    int trainSignal;                    /* training signal - reward/punish */
} AnnOp;


typedef struct
{
	Discriminator *discrim;				/* discriminator info */
	Reverse *reverse;					/* reversing */
	Mapping *mapping;					/* the mapping */
	Retina *retina;						/* retina */
	Display *display;					/* the display */
	ActivationFunc *activFunc;			/* activation function */
	AnnOp *opInfo;						/* info about which / how many to do */
    int wisardKind;                    	/* 0 = normal Wisard, ... */
} Wisard;



typedef struct M_T *M_TREE_PTR;         /* MallocT() utilities tree structure */
typedef struct M_T
{ /* binary tree structure for MallocT() */
    void *ptr;                              /* the pointer */
    int nItems;                             /* no. of items in the array */
    int gulp;                             	/* gulp sizeof(type) for arrays */
    int nBytes;                             /* no. of bytes = nItems*gulp */
    char *mesg;                             /* user's comment */
    int mesLen;                             /* length of comment */
    M_TREE_PTR bigger;                      /* ptr's bigger than this one */
    M_TREE_PTR smaller;                     /* ptr's smaller than this one */
    M_TREE_PTR prev;                        /* parent node (NULL for theRoot) */
} M_TREE;


