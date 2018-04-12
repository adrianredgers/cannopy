/*			ga_main.c			UBS				4/5/94				*/
/*
 *		Driver program for Genetic algorithm. 
 *		
 *		Most arrays are counted from 1, not the usual C default of zero.
 *		Bias units, if they exist, are stored in the zeroth elements of arrays.
 *		The top, output, layer is layer 1, the bottom, input, layer is layer 'num_layers'.
 *		
 */
 

#include 	"g_type.h" 
#include	"proto.h"

#define 	DEFAULT_CONFIG		"applic.cfg"

Void Get_First_Filename(int argc, char *argv[], char *cfg_file);
 


 

int main(int argc, char *argv[])
{
	char 			cfg_file[100], old_cfg[100], bp_cfg_file[100], results_file[100],
					summary_file[100];
	DataPairsPtr	train_data, test_data, train_price_data, test_price_data, response;
	DataFormatPtr	train_arr, test_arr, train_price_arr, test_price_arr;
	DataFormatPtr	train_response_arr, test_response_arr;
	Double			init_capital, transaction_size, transaction_cost, no_pos_zone;

	fprintf(stderr,"\nCommencing program execution.");

/*
 *	Open specified configuration file of default file. 
 */
	Get_First_Filename(argc, argv, cfg_file);

do
{
/*
 *	Construct a NeuralNet topology structure.
 *	Use this structure to Construct a BackProp storage structure.
 *	Construct train and test DataPairs structures.   
 *	Contruct DataFormat structures to hold the responses of the network.
 */
   	strcpy(old_cfg,cfg_file);
	Get_Configuration(&train_data, &test_data, &train_arr, &test_arr, &train_price_data,
&test_price_data,&response, results_file, summary_file, bp_cfg_file, cfg_file,
&init_capital, &transaction_size, &transaction_cost, &no_pos_zone);  

	train_price_arr = Con_DataFormat(train_arr->window, train_arr->prediction_delay, 
train_arr->pattern_delay, 0, 1,train_arr->num_records);    
	test_price_arr = Con_DataFormat(test_arr->window, test_arr->prediction_delay, 
test_arr->pattern_delay, 0, 1,test_arr->num_records);    
	train_response_arr = Con_DataFormat(train_arr->window, train_arr->prediction_delay, 
train_arr->pattern_delay, 0, 1,train_arr->num_records);    
	test_response_arr = Con_DataFormat(test_arr->window, test_arr->prediction_delay, 
test_arr->pattern_delay, 0, 1,test_arr->num_records);    

	Prepare_Data(train_data, train_arr);
	Prepare_Data(test_data, test_arr);
	Prepare_Prices(train_price_data, train_price_arr);
	Prepare_Prices(test_price_data, test_price_arr);
/*
 *	Train the network on the inputs and outputs in train_arr.
 */	
	GA(train_arr->inputs, 
		train_arr->in_cols,  
		train_arr->outputs,  
		train_arr->out_cols,  
		train_arr->num_records,
		train_price_arr->outputs,
		train_response_arr->outputs, 
		bp_cfg_file, BP_TRAIN);

	GA(train_arr->inputs, 
		train_arr->in_cols,  
		train_arr->outputs,  
		train_arr->out_cols,  
		train_arr->num_records,
		train_price_arr->outputs,
		train_response_arr->outputs, 
		bp_cfg_file, BP_TEST);

	Profit_And_Loss(train_response_arr, train_data, train_price_data, "train data", "applic.tra",
"sum.tra", init_capital, transaction_size, transaction_cost, no_pos_zone);
/*
 *	Test the network on the inputs in test_arr->inputs
 *	Place the responses of the network in test_arr->outputs
 */
	
	GA(test_arr->inputs, 
		test_arr->in_cols,  
		test_arr->outputs,  
		test_arr->out_cols,  
		test_arr->num_records,
		test_price_arr->outputs, 
		test_response_arr->outputs, 
		bp_cfg_file, BP_TEST);

/*
 *	Perform the Error Back-Propagation algorithm on the training data.
 *	Check with the test data how well the network has learnt the problem.
 */
	Profit_And_Loss(test_response_arr, test_data, test_price_data, "test data", results_file,
summary_file, init_capital, transaction_size, transaction_cost, no_pos_zone);


	Des_DataPairs(test_data);	
	Des_DataPairs(train_data);	
	Des_DataPairs(train_price_data);	
	Des_DataPairs(test_price_data);	
	Des_DataPairs(response);	  
	Des_DataFormat(train_arr);
	Des_DataFormat(test_arr);
	Des_DataFormat(train_price_arr);
	Des_DataFormat(test_price_arr);
	Des_DataFormat(train_response_arr);
	Des_DataFormat(test_response_arr);

} while(cfg_file[0] != '\0');   


	fprintf(stderr, "\n\nProgram execution completed. %ld bytes still allocated\n",
Get_Mem_Total(FALSE));
	return(0);

} /* End of int main() */



Void Get_First_Filename(int argc, char *argv[], char *cfg_file)
{
	if(argc > 2)
	{
		fprintf(stderr, "\nUsage: single optional argument - name of configuration file.");
		exit(1);
	}
	else if(argc == 2)
    	strcpy(cfg_file, argv[1]);  
    else
    	strcpy(cfg_file, DEFAULT_CONFIG); 
    	
} /* End of Get_First_Filename() */  

	