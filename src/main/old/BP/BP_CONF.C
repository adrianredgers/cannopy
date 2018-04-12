/*				bp_conf.c			UBS				6/4/94				*/
/*
 *	Get neural network configuration, parameters for stripped down back-prop algorithm.
 */


#include	"header.h"

#define	DEFAULT_BP_CONFIG_FILE	"bp.cfg"


Void Get_BP_Configuration(NeuralNetPtr *net, BackPropPtr *bp, char *cfg_file,
		BPOpSignal op_signal, Int in_cols, Int out_cols, Int num_records)
/*
 *	Get NeuralNet, BackProp and DataPairs configurations from given file, or from
 *	  default file if none specified.
 *	Construct a NeuralNet topology structure.
 *	Use this structure to Construct a BackProp storage structure.
 *	Construct train and test DataPairs structures, and original price data for P/L calc'n.
 */
{
	char new_word[100], weights_infile[100], weights_outfile[100];
	FILE *fp;
    Int old_mem, new_mem;           
	Int curr_layer = -1, curr_node = -1;
	IntPtr  nodes_in_layer = NULL; 
	Int item, old_item;
	Int num_layers = -1, bias_present = -1;
	UpdateType update = UD_UNKNOWN;  
	Bool increasing = FALSE;                  
	Double float_val;  
	Int int_val;
	Double learn_rate = -1.0, momentum = -1.0, err_limit = -1.0;
	
	ActivFunc net_func = NULL, tmp_func = NULL;
	ActivFuncPtrPtr all_funcs = NULL;
	DiffFunc net_diff = NULL, tmp_diff = NULL;
	DiffFuncPtrPtr all_diffs = NULL; 

	Int  max_iterations = -1;

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
			if((num_layers < 0) || (num_layers > (MAX_BP_LAYERS-1)))
				Error_Exit("\nNumber of layers out of range.",new_word,item, cfg_file);			

			nodes_in_layer	= MAKE_SOME(num_layers+1, Int);
			all_funcs 		= MAKE_SOME(num_layers+1, ActivFuncPtr);
			all_diffs 		= MAKE_SOME(num_layers+1, DiffFuncPtr);
			for(curr_layer = 0; curr_layer<= num_layers; curr_layer++)
			{
				nodes_in_layer[curr_layer] = -1;  
				all_funcs[curr_layer] = NULL;
				all_diffs[curr_layer] = NULL;
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
				all_diffs[curr_layer] 	= MAKE_SOME(int_val+1, DiffFunc); 
				tmp_func = net_func;
				tmp_diff = net_diff;			
				if(!stricmp(new_word, "functions"))
				{
					item = Scan_Function(&tmp_func, &tmp_diff, fp, cfg_file, item); 
					item = Scan_String(fp, new_word, cfg_file, item);      
				} 
				for(curr_node = 0; curr_node <= int_val; curr_node++)
				{
					all_funcs[curr_layer][curr_node] = tmp_func;
					all_diffs[curr_layer][curr_node] = tmp_diff;
				}
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
			all_diffs[curr_layer][curr_node] = tmp_diff;
			item = Scan_String(fp, new_word, cfg_file, item); 
		} /* End of parse "node ..." */			


/*
 *	Parse statements like:    
 *
 *		learn_rate 0.001     
 *		learn_rate 0.001 momentum 0.2    
 *		learn_rate 0.001 increasing    
 *		learn_rate 0.001 increasing momentum 0.2    
 *
 */					
		if(!stricmp(new_word, "learn_rate"))
		{
			if(learn_rate > -0.5)
				Error_Exit("\nLearn-rate specified twice.",new_word,item, cfg_file);			
			item = Scan_Float(fp, &float_val, cfg_file, item);   
			learn_rate = float_val;
			if((learn_rate < 0.0) || (learn_rate > 11.0))
				Error_Exit("\nLearn-rate out of range.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
			if(!stricmp(new_word, "increasing"))
			{
            	increasing = TRUE;
				item = Scan_String(fp, new_word, cfg_file, item);      
            }	
			if(!stricmp(new_word, "momentum"))
			{
				item = Scan_Float(fp, &float_val, cfg_file, item);   
				momentum = float_val;
				if((momentum < -0.0001) || (momentum > 1.0))
					Error_Exit("\nMomentum out of range.",new_word,item, cfg_file);			
				item = Scan_String(fp, new_word, cfg_file, item);
			}      
		} /* End of parse "learn_rate ..." */			

/*
 *	Parse statements like:    
 *
 *		max_iterations 1000     
 *
 */					
		if(!stricmp(new_word, "max_iterations"))
		{
			if(max_iterations >= 0)
				Error_Exit("\nMaximum number of iterations specified twice.",new_word,item, cfg_file);			
			item = Scan_Int(fp, &int_val, cfg_file, item);   
			max_iterations = int_val;
			if(max_iterations < 0)
				Error_Exit("\nMaximum number of iterations out of range.",
new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "max_iterations ..." */			

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
			if(err_limit < -0.0001)
				Error_Exit("\nError limit out of range.",new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "error_limit ..." */			

/*
 *	Parse statements like:    
 *
 *		update stochastic    
 *
 */					
		if(!stricmp(new_word, "update"))
		{
			if(update != UD_UNKNOWN)
				Error_Exit("\nUpdate type already specified.",new_word,item, cfg_file);
			item = Scan_String(fp, new_word, cfg_file, item);
			if(!stricmp(new_word, "stochastic"))
				update = UD_STOCHASTIC;
			else if(!stricmp(new_word, "batch"))
				update = UD_BATCH; 
			else
				Error_Exit("\nInvalid word following keyword 'update'.",
new_word,item, cfg_file);			
			item = Scan_String(fp, new_word, cfg_file, item);
		} /* End of parse "update ..." */			


/*
 *	Parse statements like:    
 *
 *		weights random
 *		weights infile  "c:/qtm/bp/forex.wgt" 
 *		weights outfile "c:/qtm/bp/new_4x.wgt"    
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
			}
			else if(!stricmp(new_word, "infile"))
			{
				if(weights_infile[0] != '\0')
					Error_Exit("\nWeights source specified twice.",
new_word,item, cfg_file);			
				item = Scan_String(fp, weights_infile, cfg_file, item);
			}
			else if(!stricmp(new_word, "outfile"))
			{
				if(weights_outfile[0] != '\0')
					Error_Exit("\nWeights outfile specified twice.",
new_word,item, cfg_file);			
			item = Scan_String(fp, weights_outfile, cfg_file, item);
			}
			item = Scan_String(fp, new_word, cfg_file, item);      
		} /* End of parse "data_file ..." */			
	


	
	} /* End of while not EOF */		
	fclose(fp);


/*
 *	Check that given number of input columns is the same as the number of inputs to
 *	  the network.
 *	Also check output columns and network outputs.
 */
	if(in_cols != nodes_in_layer[num_layers])
		Error_Exit("\nNumber of data input columns not the same as network inputs.",
new_word,item, cfg_file);
	if(out_cols != nodes_in_layer[1])
		Error_Exit("\nNumber of data output columns not the same as network outputs.",
new_word,item, cfg_file);
 
	new_mem = Get_Mem_Total(FALSE);
/*   fprintf(stderr, "\nBytes allocated by Get_BP_Configuation() = %ld",new_mem-old_mem);  */

/*
 *	Construct the Neural-net and the Back-prop state..
 */                                                     
	*net 		= Con_NeuralNet(num_layers, bias_present, nodes_in_layer,
all_funcs, all_diffs, weights_infile, weights_outfile, op_signal);
	*bp 		= Con_BackProp(*net, learn_rate, increasing, momentum, max_iterations, err_limit,
update, num_records, op_signal);  

	old_mem = Get_Mem_Total(FALSE);
                              
/*
 *	Free up the temporarily allocated space.
 */
	for(curr_layer = 0; curr_layer <= num_layers; curr_layer++)
	{
		if(all_funcs[curr_layer])
		{
			My_Free(all_funcs[curr_layer],nodes_in_layer[curr_layer]+1, sizeof(ActivFunc));
			My_Free(all_diffs[curr_layer],nodes_in_layer[curr_layer]+1, sizeof(DiffFunc));
		}	
	}

	My_Free(all_funcs, (num_layers+1), sizeof(ActivFuncPtr));
	My_Free(all_diffs, (num_layers+1), sizeof(DiffFuncPtr));	
	My_Free(nodes_in_layer, (num_layers+1), sizeof(Int));	

	new_mem = Get_Mem_Total(FALSE);
/*   fprintf(stderr, "\nBytes freed in Get_BP_Configuation() = %ld bytes.",old_mem-new_mem);  */

} /* End of Get_Configuration() */
	

                             
                             
Int Scan_Function(ActivFunc *net_func, DiffFunc *net_diff, FILE *fp, char *file_name, Int item)
/*
 *	Get the pointer to the string specified function, and its differential.
 */
{
	char new_word[100];
	
	item = Scan_String(fp, new_word, file_name, item);
	if(!strcmp(new_word, "sigmoid"))
	{
		*net_func = Sigmoid;
		*net_diff = Diff_Sigmoid;	
	}
	else if(!strcmp(new_word, "tanh"))
	{
		*net_func = Tanh;
		*net_diff = Diff_Tanh;	
	}
	else if(!strcmp(new_word, "2tanh"))
	{
		*net_func = Tanh2;
		*net_diff = Diff_Tanh2;	
	}
	else if(!strcmp(new_word, "3tanh"))
	{
		*net_func = Tanh3;
		*net_diff = Diff_Tanh3;	
	}
	else if(!strcmp(new_word, "4tanh"))
	{
		*net_func = Tanh4;
		*net_diff = Diff_Tanh4;	
	}
	else if(!strcmp(new_word, "identity"))
	{
		*net_func = Identity;
		*net_diff = Diff_Identity;	
	}
	else if(!strcmp(new_word, "step"))
	{
		*net_func = Step;
		*net_diff = Diff_Step;	
	}
	else
		Error_Exit("\nDid not find expected function name.",new_word,item, file_name);

	return(item);	
} /* End of Scan_Function() */



 

