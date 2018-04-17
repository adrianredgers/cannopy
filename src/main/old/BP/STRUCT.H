/*			struct.h			UBS			30/3/94		*/

/*
 *	Inclusions file of this project's struct type definitions.
 *
 *
 *	Structures defined here:
 *		DataSet
 *		DoubleVect
 *		IntVect
 *		DoubleMat
 *      IntMat
 *		DataPairs
 *		NeuralNet
 *		BackProp 
 *	and their associated pointer types.
 */

/*
 *	Simulate #PRAGMA ONCE compiler directive.
 */
#ifndef STRUCT_H
#define		STRUCT_H	1



/*
 *	Structure type for holding a set of data points and indicators.
 */
typedef struct
{
	DoublePtrPtr	ind;   
	Int				n_ind;
	DoublePtr		sup;
	Int				n_points;  
	Int				lead_in;
} DataSet;

TYPEDEF_PTR_TO(DataSet);

/*
 *	Types for passing arrays of Double's and Int's.
 */
typedef struct
{
	DoublePtr	arr;
	Int			len;        
	Int			lead_in;
} DoubleVect;

TYPEDEF_PTR_TO(DoubleVect);


typedef struct
{
	IntPtr	arr;
	Int		len;
	Int		lead_in;
} IntVect;

TYPEDEF_PTR_TO(IntVect);


typedef struct
{
	DoublePtrPtr	mat;
	Int				rows;
	Int				cols;
	Int				lead_in;
} DoubleMat;

TYPEDEF_PTR_TO(DoubleMat);
 

typedef struct
{
	IntPtrPtr	mat;
	Int			rows;
	Int			cols;
	Int			lead_in;
} IntMat;

TYPEDEF_PTR_TO(IntMat);

/*
 *	Structure types for Error Back-Prop project.
 */
 #define	MAX_BP_LAYERS	8  

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


typedef struct
{
	Int				num_layers;
	IntPtr			nodes_in_layer;
	DoublePtrPtrPtr	weight;
	ActivFuncPtrPtr	activ;
	DiffFuncPtrPtr	diff;
	Int				bias_present;    
	char			infile[100];  
	char			outfile[100];
	BPOpSignal		op_signal;

} NeuralNet;

TYPEDEF_PTR_TO(NeuralNet);


typedef enum {UD_UNKNOWN, UD_BATCH, UD_STOCHASTIC} UpdateType;

typedef struct
{
	DoublePtr		alpha1;
	DoublePtr		alpha2;
	DoublePtr		read_alpha;
	DoublePtr		write_alpha;
	Int				max_nodes;
	DoublePtr		desired;
	DoublePtrPtr	x;
	DoublePtrPtr	y;
	DoublePtrPtr	z;
	DoublePtr		beta;
	DoublePtrPtrPtr	delta;
	DoublePtrPtr	eta;
	Double			learn_rate; 
	Bool			increasing;
	Double			momentum;     
	Bool			momentum_term;
	DoublePtrPtrPtr	m_delta;
	Int				max_iterations;
	Double			err_limit;  
	UpdateType		update;  
	Double			ave_batch;  
	BPOpSignal		op_signal;
} BackProp;

TYPEDEF_PTR_TO(BackProp);

/*
 *	Data structure for parsing statements of the form:
 *
 *		layers 3 nodes 5
 *		
 */
typedef enum {RT_VOID, RT_INT, RT_DOUBLE, RT_STRING} ReturnType;  

typedef struct
{
	char 		*keyword1;
	char 		*keyword2; 
	ReturnType	return_type;
	char 		*new_string;
	Int	 		new_int;
	Double		new_double;	
	Int			int_min;
	Int			int_max;
	Double		double_min;
	Double		double_max;  
} ParseStat;

TYPEDEF_PTR_TO(ParseStat);


/*
 *	DataLoop structure for moving averages, volatilities etc.
 */
typedef struct
{
	Int 		len; 
	Int 		pos;
	DoublePtr	values;
} DataLoop;

TYPEDEF_PTR_TO(DataLoop);

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

/*
#define P_SHORT	(-1)
#define	P_FLAT	0
#define	P_LONG	1 
*/
typedef enum {P_SHORT = -1, P_FLAT = 0, P_LONG = 1} 	MarketPosition;
TYPEDEF_PTR_TO(MarketPosition);

#define	PRINT_MARKETPOSITION(A)	((A) == P_SHORT ? "SHORT" : ((A) == P_LONG ? "LONG " : "FLAT "))
#define	PRINT_BUYSELL(A)	((A) == 1 ? "BUY " : ((A) == -1 ? "SELL" : "NONE"))
#define	PRINT_HITMISS(A)	((A) == TRUE ? "HIT " : ((A) == FALSE ? "MISS" : "N/A "))

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
 * #endif from #ifndef STRUCT_H
 */
#endif 

