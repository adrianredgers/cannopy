/*			types.h			UBS			30/3/94		*/

/*
 *	Inclusions file of essential standard headers and useful extra macro and type definitions.
 */


/*
 *	Simulate #PRAGMA ONCE compiler directive.
 */
#ifndef G_TYPE_H
#define		G_TYPE_H	1

#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<malloc.h>
#include	<math.h>
#include	<limits.h> 
#include 	<float.h>
#include	<ctype.h>

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

typedef		unsigned long int 	UInt;
TYPEDEF_PTR_TO(UInt);
TYPEDEF_PTR_TO(UIntPtr);
TYPEDEF_PTR_TO(UIntPtrPtr);

/*
 *	Number of bits in an unsigned long int - typically 32.  
 */  
#define		BITS_PER_BYTE	8
#define		BITS_PER_UINT 	(BITS_PER_BYTE*sizeof(UInt))

typedef 	Int 				(*CompFunc)(VoidPtr, VoidPtr);                    	

typedef		Double 				(* ActivFunc) (Double x);
TYPEDEF_PTR_TO(ActivFunc);
TYPEDEF_PTR_TO(ActivFuncPtr);

typedef		Double 				(* DiffFunc) (Double x, Double y);
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
/*              
#define	FALSE 	0 
#define TRUE 	1 
#define	UNKNOWN 2
*/		
typedef enum {FALSE = 0, TRUE = 1, UNKNOWN = 2} Bool;
TYPEDEF_PTR_TO(Bool);
TYPEDEF_PTR_TO(BoolPtr);

#define	PRINT_BOOL(A)	((A) == TRUE ? "TRUE " : ((A) == FALSE ? "FALSE" : "D/K  "))

typedef	char					String[255];
TYPEDEF_PTR_TO(String);
TYPEDEF_PTR_TO(StringPtr);

/*
 * Macros - DANGER of side-effects in arguments that are evaluated twice.
 */
#define		ABS(A)			((A) > 0 ? (A) : -(A))
#define		TOLERANCE		0.000001  
#define		SIGN(A)			((A) > TOLERANCE ? 1 : ((A) < -TOLERANCE ? -1 : 0)) 

#define		EQUAL(A,B)		((A) > (B) ? (((A)-(B)) < TOLERANCE ? TRUE : FALSE) : (((B)-(A)) < TOLERANCE ? TRUE : FALSE))
#define		NO_POS_ZONE		0.02
#define		MARKET(A,B)		(((A)*(B)) > NO_POS_ZONE ? 1 : (((A)*(B)) < -NO_POS_ZONE ? -1 : 0))
#define		MAX(A,B)		((A)>(B) ? (A) : (B)) 
#define		UINT_ROUND(A)	(((A) - ((UInt) (A))) < 0.5 ? ((UInt) (A)) : ((UInt) ((A)+1)))
/*
 *	Automate Malloc statement.
 */
#define 	MAKE_A(A)		(A MY_MM *) My_Malloc(1, sizeof(A))	   
#define 	MAKE_SOME(N,A)	(A MY_MM *) My_Malloc((N), sizeof(A))	   

/*
 *	Global for memory accounting.
 *	Decalred in allocate.c
 */
extern	Int MEM_TOTAL;	         

/*
 *	 Types for genetic algorithm. 
 */                                   
typedef Bool 				Bit; 	/* Individual Bit */ 
TYPEDEF_PTR_TO(Bit);

typedef struct
{
	UIntPtrPtr	chrom;				/* UInt/Bit chromosome strings of individuals */   
	BoolPtr		changed;			/* Flags whether we have to recalculate fitness. */
	Int			pop_size;           /* number of individuals */
	Int			chrom_len;          /* size of an individual = num_weights */
	DoublePtr	rmse;				/* RMSE value */ 
	DoublePtr	fitness;			/* Fitness function value */ 
	DoublePtr	normal_fitness;		/* Normalized fitness function value */ 
	Double		sum_fitness;		/* Sum of the (un-normalized) fitnesses. */
	Double		max_fitness;
	Double		min_fitness;
	Double		min_rmse;			/* Minimum rmse for current population.	*/
	IntPtr		parent1;            /* parent's index */
	IntPtr		parent2;            
	IntPtr		x_site;             /* crossover sites */  
	IntPtr		ordering;			/* Numbering of the organisms by fitness -
										first item is the number of the fittest,
										second item is the no. of the second fittest etc.*/
} Population;
TYPEDEF_PTR_TO(Population);



typedef enum {FO_ACCURACY = 0, FO_P_AND_L = 1, FO_AVG_PL = 2, FO_SHARPS_RATIO = 3, 
	FO_AVG_PL_TO_MAX_DD = 4, FO_ROR_PC =5, FO_CUM_PL_PC=6} FitnessObjective;
TYPEDEF_PTR_TO(FitnessObjective);

typedef struct
{
	Int 	total_trades;
	Int 	prof_trades;
	Double 	pc_prof_trades;
	Double 	total_gains;
	Double 	total_losses;
	Double 	p_and_l;
	Double 	pl_end_period;
	Double 	avg_gains;
	Double 	avg_losses;
	Double 	avg_pl;
	Double 	stdev_pl;
	Double 	win_loss_ratio;
	Double 	max_gain;
	Double 	max_loss;
	Int 	max_num_dds;
	Double 	max_dd_val;
	Double 	ror_pc;           			/*	Rate of return (percent)					*/
	Double 	annual_ror_pc;            	/*	Annualised rate of return (percent)			*/
	Double 	sharps_ratio;
	Double 	confidence;
	Double 	avg_pl_to_max_dd;
	Double	accuracy;            		/*	= 1/RMSE									*/
	Double	cum_pl_pc;  				/*	Cumulative profit and loss percentage.		*/
} PerformRec;
TYPEDEF_PTR_TO(PerformRec);

typedef enum {P_SHORT = -1, P_FLAT = 0, P_LONG = 1} 	MarketPosition;
TYPEDEF_PTR_TO(MarketPosition);

#define	PRINT_MARKETPOSITION(A)	((A) == P_SHORT ? "SHORT" : ((A) == P_LONG ? "LONG " : "FLAT "))
#define	PRINT_BUYSELL(A)		((A) == 1 ? "BUY " : ((A) == -1 ? "SELL" : "NONE"))
#define	PRINT_HITMISS(A)		((A) == TRUE ? "HIT " : ((A) == FALSE ? "MISS" : "    "))

typedef		struct
{
	Double 			value;       			/* 	Output of neural network. 				*/ 
	Double 			equity;            		/* 	Cumulative equity build-up 				*/ 
	MarketPosition	position;
	MarketPosition	prev_position;
	Bool 			pos_changed;
	Double			prev_price;             /*	Previous actual price. 					*/
	Double			predict_price; 			/*	Price predicted by the network. 		*/   
	Double			no_pos_zone;			/* 	Value of the no-position zone. 			*/  
	Double			pl_pc;					/*	Percentage p/l on a trade. 				*/
	int				prev_sig;				/* 	buy/sell signals */
	int				curr_sig;
	Int				num_drawdowns;          /*	Current number of consecutive losses 	*/
	Double			drawdown;               /*	Current value of consecutive losses 	*/
	Bool			hit;					/* 	Flag if correct buy/sell signal. 		*/
} ResultsRec;
TYPEDEF_PTR_TO(ResultsRec); 



/*
 *	Structure types for Error Back-Prop project.
 */

typedef struct
{
	Int				num_records; 
	Int				num_inputs;	
	Int				num_outputs;
	Int				num_fields;
	DoublePtrPtr	values;
	DoublePtr		mean;
	DoublePtr		std_dev;
	DoublePtr		max_lim;
	DoublePtr		min_lim;
	Int				ignore_recs;
	Bool			data_feed;  
	char			file_name[100];
	
} DataPairs;

TYPEDEF_PTR_TO(DataPairs);

/*
 *	Operation signal for back-propagation network.
 *	Either test or train.
 */
typedef enum {BP_TEST, BP_TRAIN} BPOpSignal;
TYPEDEF_PTR_TO(BPOpSignal);

#define	PRINT_BPOPSIGNAL(A)	((A) == BP_TEST ? "TESTING" : "TRAINING")

typedef struct
{
	Int				num_layers;
	IntPtr			nodes_in_layer;  
	Int				num_weights;
	DoublePtrPtrPtr	weight;
	ActivFuncPtrPtr	activ;
	Int				bias_present;    
	char			infile[100]; 
	Int				num_in_organisms; 
	char			outfile[100];
	Int				num_out_organisms; 
	BPOpSignal		op_signal;  

} NeuralNet;

TYPEDEF_PTR_TO(NeuralNet);



typedef struct
{
	Int					max_nodes;
	DoublePtr			desired;
	DoublePtrPtr		x;
	DoublePtrPtr		y;
	Double				mutation_prob; 
	Double				crossover_prob; 
	Int					max_generations;
	Double				err_limit;            
	Double				ave_batch;  
	BPOpSignal			op_signal;
	DoublePtr			fitness_weights; 
	Double				transaction_size;
	Double				transaction_cost; 
	Double				init_capital; 
	Double				no_pos_zone;
	char 				cfg_file[100];
	FILE				*train_fp; 
	FILE                *test_fp;
	FILE				*summary_fp;
	FILE				*account_fp;
} GenAlg;

TYPEDEF_PTR_TO(GenAlg);


typedef struct
{
 	Int 		window;
 	Int			prediction_delay;
 	Int			pattern_delay;    
 	Int			in_cols;
 	Int			out_cols;    
 	Int			num_records;
 	DoublePtr	inputs;
 	DoublePtr	outputs;
} DataFormat;
TYPEDEF_PTR_TO(DataFormat);

typedef struct
{
	UInt				max_uint;  
	Double				accuracy;
	Double				range;
	Double				pos_limit;
	Double				neg_limit;    
	Int					bits_per_weight; 
	Int 				num_records;
	DoublePtr			inputs;  
	Int					in_cols;
	DoublePtr			outputs; 
	Int					out_cols;
	DoublePtr			prices;	  
	DoublePtr			responses;
} GAParams;
TYPEDEF_PTR_TO(GAParams);



	
#define	FIELD_SEP	'\t'			/*	As a char this character separates fields in report files.*/

/*
 *	Suffixes for the various files.
 */
#define 	CONFIG_FILE_SUFFIX	".CFG"
#define 	TRAIN_FILE_SUFFIX	".TRA"
#define 	TEST_FILE_SUFFIX	".TES"
#define 	ACCOUNT_FILE_SUFFIX	".ACC"
#define 	SUMMARY_FILE_SUFFIX	".SUM"

 
/*
 * #endif from #ifndef G_TYPE_H
 */
#endif 

