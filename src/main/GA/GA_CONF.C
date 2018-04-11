/*				ga_conf.c			UBS				4/5/94				*/
/*
 *	Get neural network configuration, parameters for stripped down back-prop algorithm.
 */


#include 	"g_type.h" 
#include	"proto.h"


#define	DEFAULT_BP_CONFIG_FILE	"ga.cfg"


Void Get_GA_Configuration(NeuralNetPtr *net, GenAlgPtr *ga, PopulationPtr *pop, char *cfg_file,
		BPOpSignal op_signal, Int in_cols, Int out_cols, Int num_records)
/*
 *	Get NeuralNet, GenAlg and DataPairs configurations from given file, or from
 *	  default file if none specified.
 *	Construct a NeuralNet topology structure.
 *	Use this structure to Construct a GenAlg storage structure.
 *	Construct train and test DataPairs structures, and original price data for P/L calc'n.
 */
{
	char new_word[100], weights_infile[100], weights_outfile[100], results_file[100];
	FILE *fp;
    Int old_mem, new_mem;           
	Int curr_layer = -1, curr_node = -1;
	IntPtr  nodes_in_layer = NULL; 
	Int item, old_item, i;
	Int num_layers = -1, bias_present = -1;
	Double float_val;          
	Double transaction_size = 10000.0, transaction_cost = 10.0, init_capital = 10000.0,
				no_pos_zone = 0.002; 
	Double	fitness_weights[10];
	Int int_val;
	Double mutation_prob = -1.0, crossover_prob = 1.0, err_limit = -1.0;
	
	ActivFunc net_func = NULL, tmp_func;
	ActivFuncPtrPtr all_funcs = NULL;
	DiffFunc net_diff = NULL, tmp_diff;

	Int  max_generations = -1, pop_size = -1, num_in_organisms = 0, num_out_organisms = 0;

	for(i=0; i<10;i++)
		fitness_weights[i] = 0.0;
    
	old_mem = Get_Mem_Total(FALSE);	
/*
 *	Open specified or deafault configuration file. 
 */                                                   
 	if(cfg_file[0] == 0)
 		strcpy(cfg_file, DEFAULT_BP_CONFIG_FILE);
    fp = Open_File(cfg_file, "r", "Get_BP_Configuration()");

	
/*
 *	Go through the items in the configuration file.   
 *	Before entering the loop initialize flags to show new item got,
 *	  and no data file name got.
 *	Get first item from configuration file.
 *	Go through parsers for the various statements: 
 *		e.g. "layers ..." , "layer ... " , "node ... " etc.
 *	At the end of each statement parser get the next item and go on through the list of
 *	  statement parsers. 
 *	Go back to the beginning of the loop if necessary, but don't go through the loop
 *	  more than once if no new recognized items were got. 	
 */	                            
 	old_item = 0; 
 	weights_infile[0] = '\0';
 	weights_outfile[0] = '\0'; 
 	results_file[0] = '\0';

	item = Scan_String(fp, new_word, cfg_file, 0);
	while(!feof(fp))
	{

/*
 *	Check that current keyword item has been used at all.
 */
		if(old_item == item)
			Error_Exit("\nUnknown keyword.",new_word, item, cfg_file);			
		else
			old_item = item;


/*
 *	Parse comments like:    
 *
 *		{C open comment deliminator}  this is the hidden layer {C close comment deliminator}   
 *
 */					
		if(!stricmp(new_word, "/*"))
		{
			do
			{
				item = Scan_String(fp, new_word, cfg_file, item);
			} while(stricmp(new_word, "*/"));
			item = Scan_String(fp, new_word, cfg_file, item);
		} /* End of parse comments */			
	
/*
 *	Parse statements like:    
 *
 *		layers 5      
 *		layers 5 functions sigmoid              
 *
 *	Top, output layer is layer[1]
 *	Bottom, input layer is layer[num-layers]
 */					
		if(!stricmp(new_word, "layers"))
		{
			if(num_layers != -1)
				Error_Exit("\nNumber of layers specified twice.",new_word,item, cfg_file);			
			item = Scan_Int(fp, &int_val, cfg_file, item); 
			num_layers = int_val;
			if(num_layers < 0)
				Error_Exit("\nNumber of layers out of range.",new_word,item, cfg_file);			

			nodes_in_layer	= MAKE_SOME(num_layers+1, Int);
			all_funcs 		= MAKE_SOME(num_layers+1, ActivFuncPtr);
			for(curr_layer = 0; curr_layer<= num_layers; curr_layer++)
			{
				nodes_in_layer[curr_layer] = -1;  
				all_funcs[curr_layer] = NULL;
			}
            curr_layer = -1;
			item = Scan_String(fp, new_word, cfg_file, item);
			if(!stricmp(new_word, "functions"))
			{
				item = Scan_Function(&net_func, &net_diff, fp, cfg_file, item);
				item = Scan_String(fp, new_word, cfg_file, item);      
			}
		} /* End of parse "layers ..." */			
	
/*
 *	Parse statements like:    
 *
 *		bias yes    
 *
 */					
		if(!stricmp(new_word, "bias"))
		{
			if(bias_present != -1)
				Error_Exit("\nBias already specified.",new_word,item, cfg_file);
			item = Scan_String(fp, new_word, cfg_file, item);
			if(!stricmp(new_word, "yes"))
				bias_present = 1;
			else if(!stricmp(new_word, "no"))
				bias_present = 0; 
			else
				Error_Exit("\nInvalid word following keyword 'bias'.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);
		} /* End of parse "bias ..." */			
	
/*
 *	Parse statements like:    
 *
 *		layer 3 nodes 8     
 *		layer 3 nodes 8 functions sigmoid    
 *
 */					
		if(!stricmp(new_word, "layer"))
		{
			if(num_layers < 0)
				Error_Exit("\nNumber of layers not previously specified.",new_word,item, cfg_file);			
			item = Scan_Int(fp, &int_val, cfg_file, item);
			curr_layer = int_val;
			if(nodes_in_layer[curr_layer] > 0)
				Error_Exit("\nLayer already specified.",new_word,item, cfg_file);			
			if((curr_layer < 1) || (curr_layer > num_layers))
				Error_Exit("\nLayer out of range.",new_word,item, cfg_file);			

			item = Scan_String(fp, new_word, cfg_file, item);
			if(stricmp(new_word, "nodes"))
				Error_Exit("\nDidn't find expected keyword 'nodes'.",new_word,item, cfg_file);			
			item = Scan_Int(fp, &int_val, cfg_file, item);
			if(int_val < 0)
				Error_Exit("\nNumber of nodes out of range.",new_word,item, cfg_file);			
			nodes_in_layer[curr_layer] = int_val; 

/*
 *	Node functions for this layer if it is not the bottom layer.
 */			
			item = Scan_String(fp, new_word, cfg_file, item);
			if(curr_layer < num_layers)
			{
				all_funcs[curr_layer] 	= MAKE_SOME(int_val+1, ActivFunc);
				tmp_func = net_func;
				if(!stricmp(new_word, "functions"))
				{
					item = Scan_Function(&tmp_func, &tmp_diff, fp, cfg_file, item); 
					item = Scan_String(fp, new_word, cfg_file, item);      
				} 
				for(curr_node = 0; curr_node <= int_val; curr_node++)
					all_funcs[curr_layer][curr_node] = tmp_func;
				curr_node = -1;
			}
			else if(!stricmp(new_word, "functions"))
				Error_Exit("\nCannot specify functions for the input layer.",new_word,item, cfg_file);			
		} /* End of parse "layer ..." */			
	
/*
 *	Parse statements like:    
 *
 *		node 5 function sigmoid    
 *
 */					
		if(!stricmp(new_word, "node"))
		{
			if(num_layers == -1)
				Error_Exit("\nNumber of layers not specified.",new_word,item, cfg_file);			
			if(curr_layer == -1)
				Error_Exit("\nCurrent layer not specified.",new_word,item, cfg_file);			
			if(curr_layer == num_layers)
				Error_Exit("\nTrying to specify node functions for input layer.",new_word,item, cfg_file);			
			item = Scan_Int(fp, &int_val, cfg_file, item);   
			curr_node = int_val;
			if((curr_node <= 0)  || (curr_node > nodes_in_layer[curr_layer]))
				Error_Exit("\nNode out of range.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);
			if(stricmp(new_word, "function"))
				Error_Exit("\nDidn't find expected keyword: 'function'.",new_word,item, cfg_file);			
			item = Scan_Function(&tmp_func, &tmp_diff, fp, cfg_file, item);
			all_funcs[curr_layer][curr_node] = tmp_func;
			item = Scan_String(fp, new_word, cfg_file, item); 
		} /* End of parse "node ..." */			


/*
 *	Parse statements like:    
 *
 *		mutation_prob 0.001     
 */					
		if(!stricmp(new_word, "mutation_prob"))
		{
			if(mutation_prob > -0.5)
				Error_Exit("\nMutation probability specified twice.",new_word,item, cfg_file);			
			item = Scan_Float(fp, &float_val, cfg_file, item);   
			mutation_prob = float_val;
			if((mutation_prob < 0.0) || (mutation_prob > 1.0))
				Error_Exit("\nMutation probability out of range.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "mutation_prob ..." */

/*
 *	Parse statements like:    
 *
 *		fitness_objective max_ror   
 *		fitness_objective 3 min_rmse + 1.5 max_pl + max_avg_pl_dd  
 *
 */					
		if(!stricmp(new_word, "fitness_objective"))
		{
			item = Scan_Fitness_Objective(fitness_weights, fp, cfg_file, item, new_word);
		} /* End of parse "fitness_objective ..." */			

/*
 *	Parse statements like:    
 *
 *		crossover_prob 0.001     
 */					
		if(!stricmp(new_word, "crossover_prob"))
		{
			item = Scan_Float(fp, &float_val, cfg_file, item);   
			crossover_prob = float_val;
			if((crossover_prob < 0.0) || (crossover_prob > 1.0))
				Error_Exit("\nCrossover probability out of range.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "crossover_prob ..." */

/*
 *	Parse statements like:    
 *
 *		max_generations 1000     
 *
 */					
		if(!stricmp(new_word, "max_generations"))
		{
			if(max_generations >= 0)
				Error_Exit("\nMaximum number of iterations specified twice.",new_word,item, cfg_file);			
			item = Scan_Int(fp, &int_val, cfg_file, item);   
			max_generations = int_val;
			if(max_generations < 0)
				Error_Exit("\nMaximum number of iterations out of range.",
new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "max_generations ..." */			


/*
 *	Parse statements like:    
 *
 *		pop_size 1000     
 *
 */					
		if(!stricmp(new_word, "pop_size"))
		{
			if(pop_size >= 0)
				Error_Exit("\nPopulation size specified twice.",new_word,item, cfg_file);			
			item = Scan_Int(fp, &int_val, cfg_file, item);   
			pop_size = int_val;
			if(pop_size < 0)
				Error_Exit("\nPopulation size out of range.",
new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "pop_size ..." */			


/*
 *	Parse statements like:    
 *
 *		error_limit 0.01     
 *
 */					
		if(!stricmp(new_word, "error_limit"))
		{
			if(err_limit > -0.5)
				Error_Exit("\nError limit specified twice.",new_word,item, cfg_file);			
			item = Scan_Float(fp, &float_val, cfg_file, item);   
			err_limit = float_val;
			if(err_limit < -0.00001)
				Error_Exit("\nError limit out of range.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "error_limit ..." */			


/*
 *	Parse statements like:    
 *
 *		transaction_size 10000.0      
 *
 */					
		if(!stricmp(new_word, "transaction_size"))
		{
			item = Scan_Float(fp, &float_val, cfg_file, item);   
			transaction_size = float_val;
			if(transaction_size < 0.0)
				Error_Exit("\nTransaction size out of range.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "transaction_size ..." */			


/*
 *	Parse statements like:    
 *
 *		transaction_cost 10.0      
 *
 */					
		if(!stricmp(new_word, "transaction_cost"))
		{
			item = Scan_Float(fp, &float_val, cfg_file, item);   
			transaction_cost = float_val;
			if(transaction_cost < 0.0)
				Error_Exit("\nTransaction cost out of range.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "transaction_cost ..." */			

/*
 *	Parse statements like:    
 *
 *		init_capital 10000.0      
 *
 */					
		if(!stricmp(new_word, "init_capital"))
		{
			item = Scan_Float(fp, &float_val, cfg_file, item);   
			init_capital = float_val;
			if(init_capital < 0.0)
				Error_Exit("\nInitial capital out of range.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "init_capital ..." */			


/*
 *	Parse statements like:    
 *
 *		no_pos_zone 0.002      
 *
 */					
		if(!stricmp(new_word, "no_pos_zone"))
		{
			item = Scan_Float(fp, &float_val, cfg_file, item);   
			no_pos_zone = float_val;
			if(no_pos_zone < 0.0)
				Error_Exit("\nNo-position zone out of range.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "no_pos_zone ..." */			



/*
 *	Parse statements like:    
 *
 *		weights random
 *		weights infile  "c:/qtm/bp/forex.wgt"  
 *		weights infile  "c:/qtm/bp/forex.wgt" organisms 5
 *		weights outfile "c:/qtm/bp/new_4x.wgt"    
 *		weights outfile "c:/qtm/bp/new_4x.wgt"  organisms 5  
 *
 */					
		if(!stricmp(new_word, "weights"))
		{
			item = Scan_String(fp, new_word, cfg_file, item);
			if(!stricmp(new_word, "random"))
			{
				if(weights_infile[0] != '\0')
					Error_Exit("\nWeights source specified twice.",
new_word,item, cfg_file);			
				strcpy(weights_infile, "random");
				item = Scan_String(fp, new_word, cfg_file, item);
			}
			else if(!stricmp(new_word, "infile"))
			{
				if(weights_infile[0] != '\0')
					Error_Exit("\nWeights source specified twice.",
new_word,item, cfg_file);			
				item = Scan_String(fp, weights_infile, cfg_file, item);
				item = Scan_String(fp, new_word, cfg_file, item);
				if(!stricmp(new_word, "organisms"))
				{
					item = Scan_Int(fp, &int_val, cfg_file, item);   
					num_in_organisms = int_val;
					if(num_in_organisms < 0)
						Error_Exit("\nNumber of in-organisms out of range.",
new_word,item, cfg_file);			
						item = Scan_String(fp, new_word, cfg_file, item);
				} 			
			}
			else if(!stricmp(new_word, "outfile"))
			{
				if(weights_outfile[0] != '\0')
					Error_Exit("\nWeights outfile specified twice.",
new_word,item, cfg_file);			
				item = Scan_String(fp, weights_outfile, cfg_file, item);
				item = Scan_String(fp, new_word, cfg_file, item);
				if(!stricmp(new_word, "organisms"))
				{
					item = Scan_Int(fp, &int_val, cfg_file, item);   
					num_out_organisms = int_val;
					if(num_out_organisms < 0)
						Error_Exit("\nNumber of out-organisms out of range.",
new_word,item, cfg_file);			
					item = Scan_String(fp, new_word, cfg_file, item);
				} 			
			}
		} /* End of parse "weights ..." */			
	

	} /* End of while not EOF */		
	fclose(fp);

/*
 *	Create a filename for the results out of the name of this configuration file.
 */
	if(results_file[0] == '\0')
		Make_Filename(results_file, cfg_file, ".sta");
	

/*
 *	Check that given number of input columns is the same as the number of inputs to
 *	  the network.
 *	Also check output columns and network outputs.  
 *	Check that in-organisms and out-organisms <= pop_size
 */
	if(in_cols != nodes_in_layer[num_layers])
		Error_Exit("\nNumber of data input columns not the same as network inputs.",
new_word,item, cfg_file);
	if(out_cols != nodes_in_layer[1])
		Error_Exit("\nNumber of data output columns not the same as network outputs.",
new_word,item, cfg_file);

	if(num_in_organisms > pop_size) 
		Error_Exit("\nNumber of file input organisms bigger than total population size.",
new_word,item, cfg_file);
	if(num_out_organisms > pop_size) 
		Error_Exit("\nNumber of file output organisms bigger than total population size.",
new_word,item, cfg_file);   

	new_mem = Get_Mem_Total(FALSE);

/*
 *	Default load and save all organisms if file specified but not number.
 */
	if(stricmp(weights_infile,"random") && weights_infile[0] && (num_in_organisms == 0))
		num_in_organisms = pop_size;
	if(weights_outfile[0] && (num_out_organisms == 0))
		num_out_organisms = pop_size;
	
/*
 *	Construct the Neural-net and the Back-prop state..
 */                                                     
	*net 	= Con_NeuralNet(num_layers, bias_present, nodes_in_layer,
all_funcs, weights_infile, num_in_organisms, weights_outfile, num_out_organisms, op_signal);
	*ga		= Con_GenAlg(*net, mutation_prob, crossover_prob, max_generations, err_limit,
 num_records, op_signal, fitness_weights, transaction_size, transaction_cost, 
 init_capital, no_pos_zone, cfg_file);                                          
	*pop 	= Con_Population(pop_size, (*net)->num_weights);

	if(op_signal == BP_TRAIN) 
	{
		fprintf((*ga)->train_fp,
"\nInitial capital = %lf, Transaction size = %lf, Transaction cost = %lf\nno-position zone = %lf\n",
init_capital, transaction_size, transaction_cost, no_pos_zone); 
	}


	old_mem = Get_Mem_Total(FALSE);                              
/*
 *	Free up the temporarily allocated space.
 */
	for(curr_layer = 0; curr_layer <= num_layers; curr_layer++)
		if(all_funcs[curr_layer])
			My_Free(all_funcs[curr_layer], nodes_in_layer[curr_layer]+1, sizeof(ActivFunc));

	My_Free(all_funcs, (num_layers+1), sizeof(ActivFuncPtr));
	My_Free(nodes_in_layer, (num_layers+1), sizeof(Int));	

	new_mem = Get_Mem_Total(FALSE);

} /* End of Get_GA_Configuration() */
	

                             
                             
Int Scan_Function(ActivFunc *net_func, DiffFunc *net_diff, FILE *fp, char *file_name, Int item)
/*
 *	Get the pointer to the string specified function, and its differential.
 */
{
	char new_word[100];
	
	item = Scan_String(fp, new_word, file_name, item);
	if(!stricmp(new_word, "sigmoid"))
	{
		*net_func = Sigmoid;
		*net_diff = Diff_Sigmoid;	
	}
	else if(!stricmp(new_word, "tanh"))
	{
		*net_func = Tanh;
		*net_diff = Diff_Tanh;	
	}
	else if(!stricmp(new_word, "2tanh"))
	{
		*net_func = Tanh2;
		*net_diff = Diff_Tanh2;	
	}
	else if(!stricmp(new_word, "3tanh"))
	{
		*net_func = Tanh3;
		*net_diff = Diff_Tanh3;	
	}
	else if(!stricmp(new_word, "4tanh"))
	{
		*net_func = Tanh4;
		*net_diff = Diff_Tanh4;	
	}
	else if(!stricmp(new_word, "identity"))
	{
		*net_func = Identity;
		*net_diff = Diff_Identity;	
	}
	else if(!stricmp(new_word, "step"))
	{
		*net_func = Step;
		*net_diff = Diff_Step;	
	}
	else
		Error_Exit("\nDid not find expected function name.",new_word,item, file_name);

	return(item);	
} /* End of Scan_Function() */


                             
Int Scan_Fitness_Objective(Double *fitness_weights, FILE *fp, char *file_name, Int item, 
char *new_word)
/*
 *	Scan in a fitness objective as a weighted sum of 
 *	  parameters.
 */
{
	Double	weight;
	Bool known = TRUE;
	
	item = Scan_String(fp, new_word, file_name, item);    
	do
	{	
/*
 *	Is the new word a number?
 *		If so then set the weight to be that number and get another word.
 *		If not then set the weight to be 1 and carry on trying to
 *		  understand the current word.
 */
		if(sscanf(new_word,"%lf", &weight) == 1) 	
			item = Scan_String(fp, new_word, file_name, item);  
		else
			weight = 1.0;  

/*
 *	Weight is set to 1 or another number, try to understand the current word.
 *	Error if not understood.
 */	
		if(!stricmp(new_word, "ACCURACY"))
			fitness_weights[FO_ACCURACY] += weight;
		else if(!stricmp(new_word, "P_AND_L"))
			fitness_weights[FO_P_AND_L] += weight;
		else if(!stricmp(new_word, "AVG_PL"))
			fitness_weights[FO_AVG_PL] += weight;
		else if(!stricmp(new_word, "SHARPS_RATIO"))
			fitness_weights[FO_SHARPS_RATIO] += weight;
		else if(!stricmp(new_word, "AVG_PL_TO_MAX_DD"))
			fitness_weights[FO_AVG_PL_TO_MAX_DD] += weight;
		else if(!stricmp(new_word, "ROR"))
			fitness_weights[FO_ROR_PC] += weight;
		else if(!stricmp(new_word, "CUM_PL_PC"))
			fitness_weights[FO_CUM_PL_PC] += weight;
		else 
			Error_Exit("\nFound unknown item after keyword 'FITNESS_OBJECTIVE' ",new_word,item, file_name);

/*
 *	Check for '+' sign to connect another term in the weighted sum.
 *		If it is a plus sign then get the next word 
 *		Otherwise it's the first word of the next statement, so exit this loop.
 */
		if(known)
		{
			item = Scan_String(fp, new_word, file_name, item);    
			if(!stricmp(new_word, "+"))
				item = Scan_String(fp, new_word, file_name, item);
			else
				known = FALSE;    
		}	

	} while(known && !feof(fp));

/*
 *	At this point 'new_word' contains the next word for the parse loop 
 *	  in the calling routine.
 */
	return(item);
		
} /* End of Scan_Fitness_Objective() */



 

