/*			ga_strip.c			UBS			4/5/94				*/

/*
 *	Stripped-down back-propagation function.
 */ 


#include 	"g_type.h" 
#include	"proto.h"


 
Void GA(DoublePtr inputs, Int in_cols, DoublePtr outputs, Int out_cols, Int num_records, 
		DoublePtr prices, DoublePtr responses, char *config_file, BPOpSignal op_signal)
/*   
 *	Create a network using the given configuration file.
 *	If op_signal is BP_TRAIN then train the net on the given inputs and outputs.
 *	Else if op_signal is BP_TEST then run the network on the given inputs and place the
 *	  results in the output space provided.
 */
{
	NeuralNetPtr	net;
	GenAlgPtr		gen_alg; 
	GAParamsPtr		ga_params;
	PopulationPtr 	pop, wk_pop; 
	
		
	ga_params = Con_GAParams(inputs, in_cols, outputs, 
out_cols, num_records, prices, responses);           
	fprintf(stderr,"\n\nGetting GA configuration.");
	Get_GA_Configuration(&net, &gen_alg, &pop, config_file, op_signal, in_cols, 
out_cols, num_records);  
	fprintf(stderr,"\n");
	Init_Population(pop, net, ga_params, gen_alg);
	wk_pop = Con_Population(pop->pop_size, pop->chrom_len);	

		
	if(op_signal == BP_TRAIN) 
	{
		Train_Net(net, gen_alg, ga_params, pop, wk_pop);
	}
/*	else
 		Test_Net(net, gen_alg, ga_params, pop); */

	Save_Organisms(net, pop, ga_params);


/*
 *	Destructors for the structures allocated initially.
 *	Destroy items in the inverse order to which they were constructed.
 */ 	
	Des_Population(pop);
	Des_Population(wk_pop);
	Des_GenAlg(gen_alg, net); 
	Des_GAParams(ga_params);
	Des_NeuralNet(net);	

} /* End of Void GA() */  
 
 

GAParamsPtr Con_GAParams(DoublePtr inputs, Int in_cols, DoublePtr outputs, 
Int out_cols, Int num_records, DoublePtr prices, DoublePtr responses)  
/*
 * 	Constructor for GAParams structure.
 *	Set accuracy and range.
 */
{
	GAParamsPtr ga_params = MAKE_A(GAParams);

/*
 *	Each weight will be coded as one UInt.
 *	Given weight accuracy (hard-coded) and bits per UInt, calculate the allowed range.
 *  Calculate positive and negative limits for the weight range.  
 *	For accuracy of 0.000000001 and 32 bits per weight, limits are: +- 2.1474836465
 *	  in steps of 0.000000001
 *	NB. A weight of 0.000000.. cannot be implemented by this scheme, only: +- 0.0000000005    
 *
 *	So long as the weight range is a reasonable sizee (at least +1 to -1) a neuron should 
 *	  be able to calculate any normalized function. Certainly, if its activation function
 *	  is normalized (e.g. -1 < Tanh(x) < 1) , not so certainly in the case of the
 *	  un-normalized Identity(x) function.
 */
	ga_params->accuracy			= 0.000000001;
    ga_params->max_uint			= (1UL<<(BITS_PER_UINT-1)) + ((1UL<<(BITS_PER_UINT-1))-1);
	ga_params->range			= ga_params->accuracy * ga_params->max_uint;  
	ga_params->pos_limit		= ga_params->range * 0.5;
	ga_params->neg_limit		= -ga_params->range * 0.5;
	ga_params->bits_per_weight	= BITS_PER_UINT;

/*
 *	Store data items to pass them around easily.
 */
	ga_params->num_records			= num_records;
	ga_params->inputs				= inputs;  
	ga_params->in_cols				= in_cols;
	ga_params->outputs				= outputs; 
	ga_params->out_cols				= out_cols;
	ga_params->prices 				= prices;
	ga_params->responses 			= responses;

	return(ga_params);

} /* End of GAParamsPtr Con_GAParams() */
 


GAParamsPtr Des_GAParams(GAParamsPtr ga_params)
{
	My_Free(ga_params, 1, sizeof(GAParams));
	return(NULL);

} /* End of GAParamsPtr Des_GAParams() */
 


PopulationPtr Con_Population(Int pop_size, Int chrom_len)  
/*
 * 	Constructor for GAParams structure.
 *	Set accuracy and range.
 */
{
	PopulationPtr pop = MAKE_A(Population);
	Int org;
	
	pop->pop_size = pop_size; 
	pop->chrom_len = chrom_len; 

	pop->rmse = MAKE_SOME(pop_size, Double);
	pop->fitness = MAKE_SOME(pop_size, Double);
	pop->normal_fitness = MAKE_SOME(pop_size, Double);
	pop->changed = MAKE_SOME(pop_size, Bool);
	pop->parent1 = MAKE_SOME(pop_size, Int);
	pop->parent2 = MAKE_SOME(pop_size, Int);
	pop->x_site = MAKE_SOME(pop_size, Int);
	pop->ordering = MAKE_SOME(pop_size, Int);

	pop->chrom = MAKE_SOME(pop_size, UIntPtr);
	for(org=0; org<pop_size; org++)	    
	{
		pop->ordering[org] = org;
		pop->chrom[org] = MAKE_SOME(chrom_len, UInt);
	}
		
	return(pop);

} /* End of PopulationPtr Con_Population() */
 


PopulationPtr Des_Population(PopulationPtr pop)
{
	Int org;

	My_Free(pop->rmse, pop->pop_size, sizeof(Double));
	My_Free(pop->fitness, pop->pop_size, sizeof(Double));
	My_Free(pop->normal_fitness, pop->pop_size, sizeof(Double));
	My_Free(pop->changed, pop->pop_size, sizeof(Bool));
	My_Free(pop->parent1, pop->pop_size, sizeof(Int));
	My_Free(pop->parent2, pop->pop_size, sizeof(Int));
	My_Free(pop->x_site, pop->pop_size, sizeof(Int));
	My_Free(pop->ordering, pop->pop_size, sizeof(Int));

	for(org=0; org< pop->pop_size; org++)	
		My_Free(pop->chrom[org], pop->chrom_len, sizeof(UInt));        
		
	My_Free(pop->chrom, pop->pop_size, sizeof(UIntPtr));
	


	My_Free(pop, 1, sizeof(Population));
	return(NULL);

} /* End of PopulationPtr Des_Population() */
 


 