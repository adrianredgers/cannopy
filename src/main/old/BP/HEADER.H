/*			header.h			UBS			30/3/94		*/

/*
 *	Inclusions file of essential standard headers and useful extra macro and type definitions.
 */


/*
 *	Simulate #PRAGMA ONCE compiler directive.
 */
#ifndef HEADER_H
#define		HEADER_H	1

#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<malloc.h>
#include	<math.h>

/*
 *	Type definitions so that we can easily change the memory model.
 */
#ifdef	_M_I86HM
#define		MM_HUGE		1
#define		MY_MM		__huge
#else
#define		MY_MM		
#endif

/*
 *	Macro to create a typedef statement - given a type MyType - of the form :
 *
 *	 	typedef MyType __huge * MyTypePtr 
 *
 *	(or leave out the __huge keyword depending on MY_MM).
 *	Note the use of the concatenation operator:  ##  
 *	+++ NOTE +++ requires a semi-colon (see examples).
 */
#define		TYPEDEF_PTR_TO(A)	typedef A MY_MM * A##Ptr

/*
 *	Basic types and their pointers.
 */
typedef		void 				Void;
TYPEDEF_PTR_TO(Void);           /* Typedef's VoidPtr */
TYPEDEF_PTR_TO(VoidPtr);        /* Typedef's VoidPtrPtr */
TYPEDEF_PTR_TO(VoidPtrPtr);     /* Typedef's VoidPtrPtrPtr */

typedef		double 				Double; 
TYPEDEF_PTR_TO(Double);
TYPEDEF_PTR_TO(DoublePtr);
TYPEDEF_PTR_TO(DoublePtrPtr);

typedef		long int 			Int;
TYPEDEF_PTR_TO(Int);
TYPEDEF_PTR_TO(IntPtr);
TYPEDEF_PTR_TO(IntPtrPtr);

typedef 	Int (*CompFunc)(VoidPtr, VoidPtr);                    	

typedef		Double (* ActivFunc) (Double x);
TYPEDEF_PTR_TO(ActivFunc);
TYPEDEF_PTR_TO(ActivFuncPtr);

typedef		Double (* DiffFunc) (Double x, Double y);
TYPEDEF_PTR_TO(DiffFunc);
TYPEDEF_PTR_TO(DiffFuncPtr);

/*
 *	Type definition for Boolean values.
 *	+++ WARNING +++ if(UNKNOWN) has the same effect as if(TRUE)
 *	In the following code fragment b gets set to 1. 
 *
 *					Bool a = UNKNOWN;
 *					if(a)
 *						b = 1;
 *					else
 *						b = 2;  
 */
typedef enum {FALSE=0, TRUE=1, UNKNOWN= -1} Bool;
TYPEDEF_PTR_TO(Bool);
TYPEDEF_PTR_TO(BoolPtr);
#define	PRINT_BOOL(A)	((A) == TRUE ? "TRUE " : ((A) == FALSE ? "FALSE" : "D/K  "))

/*
 *	Operation signal for back-propagation network.
 *	Either test or train.
 */
typedef enum {BP_TEST, BP_TRAIN} BPOpSignal;
TYPEDEF_PTR_TO(BPOpSignal);

/*
 * Macros - DANGER of side-effects in arguments that are evaluated twice.
 */
#define		ABS(A)			((A) > 0 ? (A) : -(A))
#define		TOLERANCE		0.00001  
#define		EQUAL(A,B)		((A) > (B) ? ((A)-(B) < TOLERANCE ? TRUE : FALSE) : ((B)-(A) < TOLERANCE ? TRUE : FALSE))
#define		SIGN(A)			((A) > TOLERANCE ? 1 : ((A) < -TOLERANCE ? -1 : 0))
#define		NO_POS_ZONE		0.02    
#define		MARKET(A,B)		(((A)*(B)) > NO_POS_ZONE ? 1 : (((A)*(B)) < -NO_POS_ZONE ? -1 : 0))
#define		MAX(A,B)		((A)>(B) ? (A) : (B)) 

/*
 *	Automate Malloc statement.
 */
#define 	MAKE_A(A)		(A MY_MM *) My_Malloc(1, sizeof(A))	   
#define 	MAKE_SOME(N,A)	(A MY_MM *) My_Malloc((N), sizeof(A))	   

/*
 *	External global for memory management.
 *	Initail declaration in allocate.c
 */
extern Int MEM_TOTAL;
	
#include	"struct.h"
#include	"prtype.h"

/*
 * #endif from #ifndef HEADER_H
 */
#endif 

