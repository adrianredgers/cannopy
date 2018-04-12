/*		setupnn.c			UBS			30/3/94			*/

/*
 *		Functions for setting up Error Back Propagation network. 
 *
 *	Pascal style array calling convention: 
 *		In the following subroutines arrays are counted from arr[1] to arr[num_elts], 
 *		not from arr[0] to arr[num_elts-1] (which is the normal C convention).
 *		MyMalloc() and MAKE_SOME() statements usually contain a "+ 1" term to allow for this.
 *		If bias units and their weights are used then they are stored in the zeroth
 *	  	  elemnts of the relevant arrays. 
 *		If bias units are not used the zeroth elements are still allocated, but left unused.
 */


#include 	"g_type.h" 
#include	"proto.h"
 

/*
 *	Activation functions and their differentials.
 */
Double	Tanh(Double x)
{
	Double y = tanh(x);
	return(y);
} /* End of Double Tanh() */

Double	Diff_Tanh(Double x, Double y)
{
	Double z = (1-y*y), dummy = x;  
	
	return(z);
} /* End of Double Diff_Tanh() */

             

Double	Tanh2(Double x)
{
	Double y = 2.0*tanh(x);
	return(y);
} /* End of Double Tanh2() */

Double	Diff_Tanh2(Double x, Double y)
{
	Double z = (2.0-y*y*0.5), dummy = x;
	return(z);
} /* End of Double Diff_Tanh2() */



Double	Tanh3(Double x)
{
	Double y = 3.0*tanh(x);
	return(y);
} /* End of Double Tanh3() */
    
Double	Diff_Tanh3(Double x, Double y)
{
	Double z = (3.0-y*y*0.3333333), dummy = x;
	return(z);
} /* End of Double Diff_Tanh3() */



Double	Tanh4(Double x)
{
	Double y = 4.0*tanh(x);
	return(y);
} /* End of Double Tanh4() */
    
Double	Diff_Tanh4(Double x, Double y)
{
	Double z = (4.0-y*y*0.25), dummy = x;
	return(z);
} /* End of Double Diff_Tanh4() */



Double	Sigmoid(Double x)
{
	Double y = 1.0/(1.0+exp(-x));
	return(y);
} /* End of Double Sigmoid() */


Double	Diff_Sigmoid(Double x, Double y)
{
	Double z = y*(1-y), dummy = x;
	return(z);
} /* End of Double Diff_Sigmoid() */


             
Double	Identity(Double x)
/*
 *	The identity function.
 */
{
	return(x);
} /* End of Double Identity() */


Double	Diff_Identity(Double x, Double y)
/*
 *	Differential of the identity function.
 */
{ 
	Double dummy1 = x, dummy2 = y;
	return(1.0);
} /* End of Double Diff_Identity() */



#define LIM 0.1
#define LIM_1 10.0
             
Double	Step(Double x)
/*
 *	The step function.
 */
{
	Double y;
	
	if(x < -LIM)
		return(-1.0);
	else if(x > LIM)
		return(1.0);
	y = x*LIM_1 ;
	return(y);
} /* End of Double Step() */


Double	Diff_Step(Double x, Double y)
/*
 *	Differential of the step function.
 */
{    
	Double dummy = y;
	
	if((x < -LIM) || (x > LIM))
		return(0.0); 
	else
		return(LIM_1);
} /* End of Double Diff_Step() */


/* --------------------------------------------------------- */

                                   
	
NeuralNetPtr Con_NeuralNet(Int num_layers, Int bias_present, IntPtr nodes_in_layer, 
	ActivFuncPtrPtr all_funcs, char infile[], Int num_in_organisms, char outfile[],
	Int num_out_organisms, BPOpSignal op_signal)
/*	
 *	Constructor for a NeuralNet structure. 
 *	Allocate space for one and set it up using the given parameters.    
 *	Index of top layer is 1, index of bottom layer is num_layers.
 */
{
	Int	layer, node;
	NeuralNetPtr net;
	Int old_mem, new_mem;
	old_mem = Get_Mem_Total(FALSE);
	
	net = MAKE_A(NeuralNet);       

	net->num_layers = num_layers;
	net->bias_present = (bias_present > 0 ? 1 : 0); 
	net->op_signal = op_signal;  
	
	net->nodes_in_layer = MAKE_SOME(num_layers+1, Int);
	for(layer = 1; layer <= num_layers; layer++)
		net->nodes_in_layer[layer] = nodes_in_layer[layer];
	net->activ = MAKE_SOME(num_layers+1, ActivFuncPtr);
	net->weight = MAKE_SOME(num_layers+1, DoublePtrPtr);

	strcpy(net->infile, infile);
	net->num_in_organisms = num_in_organisms;
	strcpy(net->outfile, outfile);
	net->num_out_organisms = num_out_organisms;

/*
 *	Allocate memory for items in each layer - except the bottom layer, which
 *	  doesn't contain any weights or activation functions.
 *	Set up the activation functions and their differentials 
 */	 
 	net->num_weights = 0;
	for(layer = 1; layer <= (num_layers-1); layer++)            
	{   
		net->num_weights +=  nodes_in_layer[layer] * 
(nodes_in_layer[layer+1] + net->bias_present);
		net->weight[layer] 	= MAKE_SOME(nodes_in_layer[layer]+1, DoublePtr);
		net->activ[layer] 	= MAKE_SOME(nodes_in_layer[layer]+1, ActivFunc);
		for(node=1; node<= nodes_in_layer[layer]; node++)
		{
			net->weight[layer][node]	= MAKE_SOME(nodes_in_layer[layer+1]+1, Double); 
			net->activ[layer][node] 	= all_funcs[layer][node];;
		} /* Next node in this layer */
	} /* Next layer (not including bottom layer) */

/*
 *	Problem with setting random weights again during testing.
 */
	Randomize_Weights(net);
	if((net->op_signal == BP_TEST)) 
	{
		if(net->outfile[0] != 0)      
		{
			strcpy(net->infile, net->outfile);
			net->num_in_organisms = num_out_organisms;
		}
	}

	new_mem = Get_Mem_Total(FALSE);
		
	return(net);
} /* End of NeuralNetPtr Con_NeuralNet() */



VoidPtr Des_NeuralNet(NeuralNetPtr net)
/*	
 *	Destructor for a NeuralNet structure.
 *	Free items in the inverse order to which they were allocated (approximately).
 *	Finally free the NeuralNet structure itself.
 */
{
	Int layer, node;
	Int old_mem, new_mem;
	old_mem = Get_Mem_Total(FALSE);
	
	for(layer = 1; layer <= (net->num_layers-1); layer++)            
	{
		for(node=1; node<= net->nodes_in_layer[layer]; node++)
			My_Free(net->weight[layer][node],(net->nodes_in_layer[layer+1]+1), sizeof(Double));
		My_Free(net->weight[layer], (net->nodes_in_layer[layer]+1), sizeof(DoublePtr));
		My_Free(net->activ[layer], (net->nodes_in_layer[layer]+1), sizeof(ActivFunc));
	}	

	My_Free(net->activ, net->num_layers+1, sizeof(ActivFuncPtr));
	My_Free(net->weight, net->num_layers+1, sizeof(DoublePtrPtr));
	My_Free(net->nodes_in_layer, net->num_layers+1, sizeof(Int));
		
	My_Free(net, 1 , sizeof(NeuralNet));
	new_mem = Get_Mem_Total(FALSE);
	return(NULL);

} /* End of Void Des_NeuralNet() */    


GenAlgPtr Con_GenAlg(NeuralNetPtr net, Double mutation_prob, Double crossover_prob, 
				Int max_generations, Double err_limit , Int epoch_size,
				BPOpSignal op_signal, Double *fitness_weights,
				Double transaction_size, Double transaction_cost, Double init_capital,
				Double no_pos_zone, char *cfg_file)
/*	
 *	Constructor for a GenAlg structure.
 *	Allocate space for and set up a GenAlg structure.
 */
{
	GenAlgPtr ga;
	Int layer, i; 
	char file_name[100];
	
	Int old_mem, new_mem;
	old_mem = Get_Mem_Total(FALSE);
	
	ga = MAKE_A(GenAlg);       
    
	ga->mutation_prob = (mutation_prob < 0.0 ? 0.0 : mutation_prob);   
	ga->crossover_prob = crossover_prob;
	ga->max_generations = (max_generations >= 0 ? max_generations : 100);
	ga->err_limit = (err_limit < 0.0 ? 0.0 : err_limit);
	ga->op_signal = op_signal;
	ga->transaction_size = transaction_size;
	ga->transaction_cost = transaction_cost;
	ga->init_capital = init_capital;      
	ga->no_pos_zone = no_pos_zone;
	strcpy(ga->cfg_file, cfg_file);

	ga->fitness_weights = MAKE_SOME(10, Double);
	for(i=0; i<10; i++)
		ga->fitness_weights[i] = fitness_weights[i];
/*
 *	Depending on whether we are training or testing open the various 
 *	  results files and add headers.
 */
	ga->train_fp = NULL;		
	ga->summary_fp = NULL;		
	ga->account_fp = NULL;		
	ga->test_fp = NULL;		

	if(op_signal == BP_TRAIN) 
	{
		Make_Filename(file_name, cfg_file, TRAIN_FILE_SUFFIX);
		ga->train_fp = Open_File(file_name, "a", "Con_GenAlg()");
		fprintf(ga->train_fp,"\nRecord of Training\n===================\n"); 
	}
    
/*
 *	Set a multiplier for batch updates.
 */
	ga->ave_batch   =  (epoch_size ? (1.0/epoch_size) : 1.0);

/*
 *	Allocate space for the two alpha arrays.   
 *	write_alpha[]  is the array to be written to.
 *	read_alpha[]   is the array to be read from,
 */
	ga->max_nodes = net->nodes_in_layer[1];
	for(layer=2; layer<= net->num_layers; layer++)
		ga->max_nodes = MAX(ga->max_nodes, net->nodes_in_layer[layer]);

/* 
 *	Allocate space for pointers to pointers.
 */
	ga->x = MAKE_SOME(net->num_layers+1, DoublePtr);
	ga->y = MAKE_SOME(net->num_layers+1, DoublePtr);

/*
 *	Allocate space for the x's, y's and z's, beta's, delta's and eta's.
 *	For each node: x is its sum, y its actual (squashed) output, and z the differential of
 *	  the squashing function at x.
 *	Set eta's (local updata signals) to be all +1 except for bias units - set to 0
 *	Set up bias units , y[0] = -1.0 , whether or not they're actually used.
 */ 

 	for(layer=1; layer <= (net->num_layers-1); layer++)
 	{
		ga->x[layer] 		= MAKE_SOME(net->nodes_in_layer[layer]+1, Double);
		ga->y[layer]		= MAKE_SOME(net->nodes_in_layer[layer]+1, Double);
	}

/*
 *	Don't allocate outputs if we're testing.
 *	In that case outputs will go into the space given by the main program.
 *	(If we're training then make the space for comparing with the desired outputs) 
 */

	new_mem = Get_Mem_Total(FALSE);
	
	return(ga);
} /* End of GenAlgPtr Con_GenAlg() */



VoidPtr Des_GenAlg(GenAlgPtr ga, NeuralNetPtr net)
/*
 *	Destructor for a GenAlg structure.
 *	Free components in the inverse order to which they were created.	
 *	Finally free the GenAlg structure itself.
 */
{

	Int layer;
	Int old_mem, new_mem;
	old_mem = Get_Mem_Total(FALSE);

	My_Free(ga->fitness_weights, 10, sizeof(Double));

/*
 *	Close the results files depending on whether we're testing or training.
 */	

	if(ga->train_fp != NULL)
		fclose(ga->train_fp);
	if(ga->summary_fp != NULL)
		fclose(ga->summary_fp);
	if(ga->account_fp != NULL)
		fclose(ga->account_fp);
	if(ga->test_fp != NULL)
		fclose(ga->test_fp);

/*
 *	Free weights outputs and and functions for every layer except the bottom layer.
 */
 	for(layer=1; layer <= (net->num_layers-1); layer++)
 	{
		My_Free(ga->x[layer], net->nodes_in_layer[layer]+1, sizeof(Double));
		My_Free(ga->y[layer], net->nodes_in_layer[layer]+1, sizeof(Double));

	}


	My_Free(ga->x, net->num_layers+1, sizeof(DoublePtr));
	My_Free(ga->y, net->num_layers+1, sizeof(DoublePtr));

	My_Free(ga, 1 , sizeof(GenAlg)); 
	  
	new_mem = Get_Mem_Total(FALSE);
	return(NULL);

} /* End of Void Des_GenAlg() */    

 

 

Void Randomize_Weights(NeuralNetPtr net)
/*
 *	Randomly set the weights in the given NeuralNet to the range [-1.0 , +1.0]
 */                                                                 
{
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);

	for(layer=1; layer <= (net->num_layers-1); layer++)
		for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
			for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++)
				net->weight[layer][up_node][down_node] = 2.0*((Double) rand())/RAND_MAX - 1.0;

} /* End of Void Randomize_Weights() */ 

 


Void Weights_File_Error(NeuralNetPtr net, char *mesg, Int item)
/*
 *	Too few or too many weights in the weights infile.
 */
{
	Int layer, total, sub_tot;

	fprintf(stderr,
"\n+++ ERROR +++ Get_File_Data() : error at item %ld while reading file %s",item, net->infile);  
	fprintf(stderr,
"\n%s : expected network topology: %ld layers", mesg, net->num_layers);  

	total = net->nodes_in_layer[1]*(net->nodes_in_layer[2]+net->bias_present);	
	fprintf(stderr,"\nOutput layer = layer[1] : %ld nodes with %ld weights (%s)",
net->nodes_in_layer[1], total, (net->bias_present ? "inc. bias units" : "no bias units"));

	for(layer=2;layer<net->num_layers;layer++)
	{
		sub_tot = net->nodes_in_layer[layer]*(net->nodes_in_layer[layer+1]+net->bias_present);
		total += sub_tot;	
		fprintf(stderr,"\nLayer[%ld] : %ld nodes with %ld weights (%s)",
layer, net->nodes_in_layer[layer], sub_tot,
(net->bias_present ? "inc. bias units" : "no bias units"));	
	}

	fprintf(stderr,"\nInput layer = layer[%ld] : %ld nodes (%s)",
net->num_layers, (net->nodes_in_layer[net->num_layers]+net->bias_present), 
(net->bias_present ? "inc. bias unit" : "no bias unit"));
	fprintf(stderr,"\nExpected a total of %ld weights/n", total); 

	exit(1);
} /* End of Void Weights_File_Error() */



Void Save_Organisms(NeuralNetPtr net, PopulationPtr pop, GAParamsPtr params)
/*
 *	Save the weights of the fittest organisms to file.
 */                                                                 
{
	Int org;
	FILE *fp; 
	
	if(net->outfile[0] != '\0')	
	{
		fprintf(stderr,"\nSaving weights of %ld organisms to file %s", 
net->num_out_organisms, net->outfile);
		fp = Open_File(net->outfile, "w", "Save_Organisms()");
	    for(org = 0; org< net->num_out_organisms; org++)
	    {   
			Chrom_To_Weights(net, pop, pop->ordering[org], params);
			Save_File_Weights(net, fp);
	    } 
	
		fclose(fp);
    }
    
} /* End of Void Save_Organisms() */       



Void Save_File_Weights(NeuralNetPtr net, FILE *fp)
/*
 *	Save the weights in the given NeuralNet to the specified file
 */                                                                 
{
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);
	Int item;
	
	item = 0;    
	for(layer=1; layer <= (net->num_layers-1); layer++) 
	{
		for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
		{
			for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++)
			{
				fprintf(fp,"%lf\n", net->weight[layer][up_node][down_node]);
				item++;
			} 
			fprintf(fp,"\n");
		}   
		fprintf(fp,"\n");   
	}
	fprintf(fp,"\n");   
    
} /* End of Void Save_File_Weights() */       



Void Chrom_To_Weights(NeuralNetPtr net, PopulationPtr pop, Int organism, GAParamsPtr params)
/*
 *	Decode the specified organism into weights in the given NeuralNet.
 */                                                                 
{
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);
	Int item;
	
	item = 0;    
	for(layer=1; layer <= (net->num_layers-1); layer++)
		for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
		{
			for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++)
			{
				net->weight[layer][up_node][down_node] = pop->chrom[organism][item] * 
params->accuracy + params->neg_limit;
				item++;
			} 
		}   
} /* End of Void Chrom_To_Weights() */       



Int Load_Organisms(NeuralNetPtr net, PopulationPtr pop, GAParamsPtr params)
/*
 *	Load the weights of organisms from file.  
 *	Returns the number of organisms actually loaded.
 */                                                                 
{
	Int org;
	FILE *fp; 
	
	fprintf(stderr,"\nGetting weights for %ld organisms from file %s", 
net->num_in_organisms, net->infile);
	fp = Open_File(net->infile, "r", "Get_File_Weights()");  
	org = 0;
    while((org< net->num_in_organisms) && !feof(fp))
    {
		Get_File_Weights(net, fp);
		Weights_To_Chrom(net, pop, org, params);  
		pop->changed[org] = TRUE;
		org++;
    } 
	fclose(fp);
	
	if(org < net->num_in_organisms)
	{
	 	fprintf(stderr, 
"\nWarning: expected %ld organisms in file '%s', found only %ld \n(padding with random organisms)\n",
net->num_in_organisms, net->infile, org);
	}

	return(org);
		    
} /* End of Int Load_Organisms() */       


 
Void Get_File_Weights(NeuralNetPtr net, FILE *fp)
/*
 *	Set the weights in the given NeuralNet from the specified file
 */                                                                 
{
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);
	float	float_val;  
	Int	good_scan, item;
	
	

	item = 0;
	for(layer=1; layer <= (net->num_layers-1); layer++)
		for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
			for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++)
			{
				if(feof(fp))
					Weights_File_Error(net,"Expecting more weights", item);
				good_scan = fscanf(fp,"%f ",&float_val);
				item++;
				if(good_scan != 1)
				{
					fprintf(stderr,
"\n+++ ERROR +++ Get_File_Data() : error while reading file %s", net->infile);
					fprintf(stderr,"\nlayer %ld, up-node %ld, down-node %ld, (bias=%s), item %ld", 
layer, up_node, down_node, (no_bias ? "NO" : "YES"), item);
					exit(1);  
				}
				net->weight[layer][up_node][down_node] = float_val; 
			}    

} /* End of Void Get_File_Weights() */       



Void Weights_To_Chrom(NeuralNetPtr net, PopulationPtr pop, Int organism, GAParamsPtr params)
/*
 *	Encode the weights in the given NeuralNet to the specified organism in the
 *	  given population.
 */                                                                 
{
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);
	Int item;
	Double raw_val;

/*
 *	Note: params->neg_limit is a negative value.
 */	
	item = 0;    
	for(layer=1; layer <= (net->num_layers-1); layer++)
		for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
		{
			for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++)
			{    
				if(net->weight[layer][up_node][down_node] >= params->pos_limit)
					pop->chrom[organism][item] = params->max_uint;
				else if(net->weight[layer][up_node][down_node] <= params->neg_limit)
					pop->chrom[organism][item] = 0;
				else    
				{
					raw_val = 
(net->weight[layer][up_node][down_node] - params->neg_limit) / params->accuracy;
					pop->chrom[organism][item] = UINT_ROUND(raw_val);  
				}
				item++;
			} 
		}     
	
	pop->changed[organism] = TRUE;
	
} /* End of Void Weights_To_Chrom() */       



      
