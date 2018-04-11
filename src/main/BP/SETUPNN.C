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


#include	"header.h"
 

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
	ActivFuncPtrPtr all_funcs, DiffFuncPtrPtr all_diffs, char infile[], char outfile[],
	BPOpSignal op_signal)
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
	net->diff = MAKE_SOME(num_layers+1, DiffFuncPtr);
	net->weight = MAKE_SOME(num_layers+1, DoublePtrPtr);

	strcpy(net->infile, infile);
	strcpy(net->outfile, outfile);
/*
 *	Allocate memory for items in each layer - except the bottom layer, which
 *	  doesn't contain any weights or activation functions.
 *	Set up the activation functions and their differentials 
 */	
	for(layer = 1; layer <= (num_layers-1); layer++)            
	{
		net->weight[layer] 	= MAKE_SOME(nodes_in_layer[layer]+1, DoublePtr);
		net->activ[layer] 	= MAKE_SOME(nodes_in_layer[layer]+1, ActivFunc);
		net->diff[layer] 	= MAKE_SOME(nodes_in_layer[layer]+1, DiffFunc);
		for(node=1; node<= nodes_in_layer[layer]; node++)
		{
			net->weight[layer][node]	= MAKE_SOME(nodes_in_layer[layer+1]+1, Double); 
			net->activ[layer][node] 	= all_funcs[layer][node];;
			net->diff[layer][node] 		= all_diffs[layer][node];
		} /* Next node in this layer */
	} /* Next layer (not including bottom layer) */

/*
 *	Problem with setting random weights again during testing.
 */
	if((net->op_signal == BP_TEST)) 
	{
		if(net->outfile[0] == 0)
			Randomize_Weights(net);
		else
		{ 
			strcpy(net->infile, net->outfile);
			Get_File_Weights(net);
		}
	}
	else
	{
		if(!stricmp(net->infile, "random"))
			Randomize_Weights(net);
		else
			Get_File_Weights(net);   
	}

	new_mem = Get_Mem_Total(FALSE);
/*	fprintf(stderr,"\nMemory allocated by Con_NeuralNet() = %ld bytes",new_mem-old_mem); */	
		
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
		My_Free(net->diff[layer], (net->nodes_in_layer[layer]+1), sizeof(DiffFunc));
	}	

	My_Free(net->activ, net->num_layers+1, sizeof(ActivFuncPtr));
	My_Free(net->diff, net->num_layers+1, sizeof(DiffFuncPtr));
	My_Free(net->weight, net->num_layers+1, sizeof(DoublePtrPtr));
	My_Free(net->nodes_in_layer, net->num_layers+1, sizeof(Int));
		
	My_Free(net, 1 , sizeof(NeuralNet));
	new_mem = Get_Mem_Total(FALSE);
/*	fprintf(stderr,"\nMemory freed by Des_NeuralNet() = %ld bytes",old_mem-new_mem); */	
	return(NULL);

} /* End of Void Des_NeuralNet() */    



BackPropPtr Con_BackProp(NeuralNetPtr net, Double learn_rate, Bool increasing, Double momentum, 
				Int max_iterations, Double err_limit, UpdateType update, Int epoch_size,
				BPOpSignal op_signal)
/*	
 *	Constructor for a BackProp structure.
 *	Allocate space for and set up a BackProp structure.
 */
{
	BackPropPtr bp;
	Int node,layer;
	Int old_mem, new_mem;
	old_mem = Get_Mem_Total(FALSE);

	bp = MAKE_A(BackProp);       
    
	bp->learn_rate = (learn_rate < 0.0 ? 0.0 : learn_rate); 
	bp->increasing = increasing;    
	bp->momentum_term = (momentum <= 0.0 ? FALSE : TRUE);
	bp->momentum = (momentum <= 0.0 ? 0.0 : momentum);
	bp->max_iterations = (max_iterations >= 0 ? max_iterations : 10000000);
	bp->update = (update == UD_BATCH ? UD_BATCH : UD_STOCHASTIC);
	bp->err_limit = (err_limit < 0.0 ? 0.0 : err_limit);
	bp->op_signal = op_signal;
/*
 *	Set a multiplier for batch updates.
 */
	bp->ave_batch   =  (epoch_size ? (1.0/epoch_size) : 1.0);

/*
 *	Allocate space for the two alpha arrays.   
 *	write_alpha[]  is the array to be written to.
 *	read_alpha[]   is the array to be read from,
 */
	bp->max_nodes = net->nodes_in_layer[1];
	for(layer=2; layer<= net->num_layers; layer++)
		bp->max_nodes = MAX(bp->max_nodes, net->nodes_in_layer[layer]);

	bp->beta 	= MAKE_SOME(bp->max_nodes+1, Double);
 	bp->alpha1 	= MAKE_SOME(bp->max_nodes+1, Double);
 	bp->alpha2 	= MAKE_SOME(bp->max_nodes+1, Double);
	bp->read_alpha = bp->alpha1;
	bp->write_alpha = bp->alpha2;	

/* 
 *	Allocate space for pointers to pointers.
 */
	bp->x = MAKE_SOME(net->num_layers+1, DoublePtr);
	bp->y = MAKE_SOME(net->num_layers+1, DoublePtr);
 	bp->z = MAKE_SOME(net->num_layers+1, DoublePtr);
 	bp->delta = MAKE_SOME(net->num_layers+1, DoublePtrPtr);
 	bp->m_delta = MAKE_SOME(net->num_layers+1, DoublePtrPtr);
 	bp->eta = MAKE_SOME(net->num_layers+1, DoublePtr);

/*
 *	Allocate space for the x's, y's and z's, beta's, delta's and eta's.
 *	For each node: x is its sum, y its actual (squashed) output, and z the differential of
 *	  the squashing function at x.
 *	Set eta's (local updata signals) to be all +1 except for bias units - set to 0
 *	Set up bias units , y[0] = -1.0 , whether or not they're actually used.
 */ 

 	for(layer=1; layer <= (net->num_layers-1); layer++)
 	{
		bp->x[layer] 		= MAKE_SOME(net->nodes_in_layer[layer]+1, Double);
/*
 *	Don't allocate outputs if we're testing.
 *	In that case outputs will go into the space given by the main program.
 *	(If we're training then make the space for comparing with the desired outputs) 
 */
		if((op_signal == BP_TRAIN) || (layer >1)) 
			bp->y[layer]		= MAKE_SOME(net->nodes_in_layer[layer]+1, Double);
		bp->z[layer] 		= MAKE_SOME(net->nodes_in_layer[layer]+1, Double);
		bp->eta[layer] 		= MAKE_SOME(net->nodes_in_layer[layer]+1, Double);

		bp->delta[layer]	= MAKE_SOME(net->nodes_in_layer[layer]+1, DoublePtr);
		bp->m_delta[layer]	= MAKE_SOME(net->nodes_in_layer[layer]+1, DoublePtr);
		
		for(node = 1; node <= net->nodes_in_layer[layer]; node++)
		{
			bp->delta[layer][node] = MAKE_SOME(net->nodes_in_layer[layer+1]+1, Double);
			bp->m_delta[layer][node] = MAKE_SOME(net->nodes_in_layer[layer+1]+1, Double);
			bp->eta[layer][node] = 1.0;		
		}

		bp->eta[layer][0] = 0.0;
	}

/*
 *	Don't allocate outputs if we're testing.
 *	In that case outputs will go into the space given by the main program.
 *	(If we're training then make the space for comparing with the desired outputs) 
 */

/*
 *	Set the initial values of the weights increments to zero.
 */
	Zeroize_Deltas(net, bp);  
	
	new_mem = Get_Mem_Total(FALSE);
/*	fprintf(stderr,"\nMemory allocated by Con_BackProp() = %ld bytes",new_mem-old_mem);	 */
	
	return(bp);
} /* End of BackPropPtr Con_BackProp() */



VoidPtr Des_BackProp(BackPropPtr bp, NeuralNetPtr net)
/*
 *	Destructor for a BackProp structure.
 *	Free components in the inverse order to which they were created.	
 *	Finally free the BackProp structure itself.
 */
{

	Int layer, node;
	Int old_mem, new_mem;
	old_mem = Get_Mem_Total(FALSE);
	
	My_Free(bp->alpha1, bp->max_nodes+1, sizeof(Double));
	My_Free(bp->alpha2, bp->max_nodes+1, sizeof(Double));
	My_Free(bp->beta, bp->max_nodes+1, sizeof(Double));

/*
 *	Allocate weights outputs and and functions for every layer except the bottom layer.
 */

 	for(layer=1; layer <= (net->num_layers-1); layer++)
 	{
		My_Free(bp->x[layer], net->nodes_in_layer[layer]+1, sizeof(Double));
/*
 *	Don't free outputs if we were testing.
 *	In that case outputs went into the space given by the main program.
 *	(If wee were training then free the space used for comparing with the desired outputs) 
 */
		if((bp->op_signal == BP_TRAIN) || (layer >1)) 
			My_Free(bp->y[layer], net->nodes_in_layer[layer]+1, sizeof(Double));
		My_Free(bp->z[layer], net->nodes_in_layer[layer]+1, sizeof(Double));
		My_Free(bp->eta[layer], net->nodes_in_layer[layer]+1, sizeof(Double));


		for(node = 1; node <= net->nodes_in_layer[layer]; node++)
		{
			My_Free(bp->delta[layer][node], net->nodes_in_layer[layer+1]+1, sizeof(Double));
			My_Free(bp->m_delta[layer][node], net->nodes_in_layer[layer+1]+1, sizeof(Double));
		}   
		
		My_Free(bp->delta[layer], net->nodes_in_layer[layer]+1, sizeof(DoublePtr));
		My_Free(bp->m_delta[layer], net->nodes_in_layer[layer]+1, sizeof(DoublePtr));
	}


	My_Free(bp->x, net->num_layers+1, sizeof(DoublePtr));
	My_Free(bp->y, net->num_layers+1, sizeof(DoublePtr));
	My_Free(bp->z, net->num_layers+1, sizeof(DoublePtr));
	My_Free(bp->eta, net->num_layers+1, sizeof(DoublePtr));
	My_Free(bp->delta, net->num_layers+1, sizeof(DoublePtrPtr));
	My_Free(bp->m_delta, net->num_layers+1, sizeof(DoublePtrPtr));

	My_Free(bp, 1 , sizeof(BackProp)); 
	  
	new_mem = Get_Mem_Total(FALSE);
/*	fprintf(stderr,"\nMemory freed by Des_BackProp() = %ld bytes",old_mem-new_mem);	 */
	return(NULL);

} /* End of Void Des_BackProp() */    

 

Void Randomize_Weights(NeuralNetPtr net)
/*
 *	Randomly set the weights in the given NeuralNet to the range [-1.0 , +1.0]
 */                                                                 
{
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);

	fprintf(stderr,"\nRandomizing weights");
	for(layer=1; layer <= (net->num_layers-1); layer++)
		for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
			for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++)
				net->weight[layer][up_node][down_node] = 2.0*((Double) rand())/RAND_MAX - 1.0;

} /* End of Void Randomize_Weights() */ 




Void Zeroize_Deltas(NeuralNetPtr net, BackPropPtr bp)
/*
 *	Set the weight changes and momemntum terms to zero.
 */                                                                 
{
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);

	for(layer=1; layer <= (net->num_layers-1); layer++)
		for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
			for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++) 
			{
				bp->delta[layer][up_node][down_node] = 0.0;
				bp->m_delta[layer][up_node][down_node] = 0.0; 
			}

} /* End of Void Zeroize_Deltas() */ 

 
 
Void Get_File_Weights(NeuralNetPtr net)
/*
 *	Set the weights in the given NeuralNet from the specified file
 */                                                                 
{
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);
	FILE *fp; 
	float	float_val;  
	Int	good_scan, item;
	
	
	fprintf(stderr,"\nGetting weights from file %s", net->infile);
	fp = Open_File(net->infile, "r", "Get_File_Weights()");

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
	if(!feof(fp))
		Weights_File_Error(net,"Expecting fewer weights", item);
	fclose(fp);

} /* End of Void Get_File_Weights() */       



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



Void Save_File_Weights(NeuralNetPtr net)
/*
 *	Save the weights in the given NeuralNet to the specified file
 */                                                                 
{
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);
	FILE *fp; 
	Int item;
	
	if(net->outfile[0] != '\0')	
	{
		fprintf(stderr,"\nSaving weights to file %s", net->outfile);
		fp = Open_File(net->outfile, "w", "Save_File_Weights()");
	
		item = 0;    
		for(layer=1; layer <= (net->num_layers-1); layer++)
			for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
			{
				for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++)
				{
					fprintf(fp,"%lf ", net->weight[layer][up_node][down_node]);
					item++;
				} 
				fprintf(fp,"\n");
			}   
/*		fprintf(stderr,"\nWritten %ld items to file", item);    */
	
		fclose(fp);
    }
    
} /* End of Void Save_File_Weights() */       



      
