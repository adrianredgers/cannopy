/*		ga.c			UBS			4/5/94			*/
/*
 *		Genetic algorithm.         
 *
 *	Array calling convention: 
 *		Due to revisions etc  some arrays are counted from arr[1] to arr[num_elts], 
 *		not from arr[0] to arr[num_elts-1] (which is the normal C convention).
 *	Malloc() and MAKE_SOME() statements usually contain an extra "+ 1" to allow for this.
 *	If bias units are used their weights are stored in the zeroth elements of
 *	  the relevant arrays. 
 *	If bias units are not used the zeroth elements are still allocated but left unused. 
 *
 *	Top layer = output layer is layer[1]
 *  Bottom layer = input layer is layer[num_layers]
 */



#include 	"g_type.h" 
#include	"proto.h"



Void Init_Population(PopulationPtr pop, NeuralNetPtr net, GAParamsPtr params, GenAlgPtr gen_alg)
/*
 *	Get initial population and sort by increasing fitness.
 */
{
	Int num_loaded, org;    
	Double fitness;   
	BPOpSignal tmp_op_signal;
	
/*
 *	Load up some organisms from file.
 *	Generate random organisms for the rest of the population.
 */			
	if(!stricmp(net->infile,"random"))
		num_loaded = 0;
	else
		num_loaded = Load_Organisms(net, pop, params);      
	for(org=num_loaded; org<pop->pop_size; org++)
	{
		Randomize_Weights(net);
		Weights_To_Chrom(net, pop, org, params);
	}

/*
 *	Evaluate the fitnesses of all the organisms.
 *	Normalize the fitnesses and reset the ordering array.
 *	Sort so that the ordering array indexes organisms by increasing fitness.
 */
	pop->sum_fitness = 0.0;     
	pop->max_fitness = 0.0;
	pop->min_fitness = 1000000000.0;
	tmp_op_signal =  net->op_signal;
	net->op_signal = BP_TRAIN;

	fprintf(stdout,
"\nInitial %ld (%ld loaded + %ld random) organisms:",
pop->pop_size, num_loaded, (pop->pop_size-num_loaded));	
	
	pop->min_rmse = 10000000.0;
	for(org=0; org<pop->pop_size; org++)   
	{
		fitness = Eval_Fitness(pop, net, gen_alg, params, org); 
		if(!(org%20))
			fprintf(stderr,"\n");
		fprintf(stderr,"%ld ", (org+1));
		pop->sum_fitness += fitness; 
		   
		if(fitness > pop->max_fitness)
			pop->max_fitness = fitness;
		if(fitness < pop->min_fitness)
			pop->min_fitness = fitness;
		if(pop->rmse[org] < pop->min_rmse)
			pop->min_rmse = pop->rmse[org];
	}   
/*
 *	Normalize fitnesses and set the initial ordering.
 *	Sort organisms according to fitness.
 */
	Normalize_Fitnesses(pop);
	My_Sort(0, pop->pop_size-1, pop);

	fprintf(stdout, "\nFitness: max = %lf, ave = %lf, min = %lf, min rmse = %lf",
pop->max_fitness, pop->sum_fitness/pop->pop_size, pop->min_fitness, pop->min_rmse);  
	if(gen_alg->op_signal == BP_TRAIN)
	{
		fprintf(gen_alg->train_fp,
"\nInitial %ld (%ld loaded + %ld random) organisms",
pop->pop_size, num_loaded, (pop->pop_size-num_loaded));	
		fprintf(gen_alg->train_fp, "\nFitness: max = %lf, ave = %lf, min = %lf",
pop->max_fitness, pop->sum_fitness/pop->pop_size, pop->min_fitness);      
	}
/*
 *	If testing then evalute the fitness of the fittest organism and place it 
 *	  in the output array for analysis by the calling program.
 */
	if(tmp_op_signal == BP_TEST)
	{
		net->op_signal = BP_TEST;
		pop->changed[pop->ordering[0]] = TRUE;
		fitness = Eval_Fitness(pop, net, gen_alg, params, pop->ordering[0]);
		fprintf(stdout, "\nFittest tested organism : %lf \n",fitness);
	}
} /* End of Void Init_Population() */


 
Double Propagate(NeuralNetPtr net, GenAlgPtr bp)
{
	Double out_err = 0.0;
	Int layer, up_node, down_node;
	Int	no_bias = (1-net->bias_present);
/*
 *	Propagate the inputs upwards from the bottom layer to the top. 
 *	The inputs and output spaces and values etc should have been set up already.
 *	For each node set its sum, x, its actual output, y, and the differential of
 *	  the squashing function, z.
 */

	for(layer = net->num_layers-1; layer >= 1; layer--)
	{
		for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
		{
			bp->x[layer][up_node] = 0.0;  
			if(net->bias_present)
				bp->x[layer][up_node] -= net->weight[layer][up_node][0];
			for(down_node = 1; down_node <= net->nodes_in_layer[layer+1]; down_node++)
			{
				bp->x[layer][up_node] += 
net->weight[layer][up_node][down_node]*(bp->y[layer+1][down_node]);
			}
			bp->y[layer][up_node] = net->activ[layer][up_node] (bp->x[layer][up_node]);				
		}
    }
    
/*
 *	Calculate the total error.
 */
	for(up_node = 1; up_node <= net->nodes_in_layer[1]; up_node++)
	{
		out_err += (bp->y[1][up_node]-bp->desired[up_node]) * 
(bp->y[1][up_node]-bp->desired[up_node]);	
	}		
	return(out_err);
} /* End of Double Propagate() */

                                    
                                    
Void Train_Net(NeuralNetPtr net, GenAlgPtr gen_alg,  GAParamsPtr params, 
PopulationPtr old_pop, PopulationPtr new_pop)
/*
 *	Perform the Error Back-Propagation algorithm on the training data.
 */
{
	Int				gen, org, pop_section, parent1, parent2;
	PopulationPtr	tmp_pop;
	Double fitness;   
	
/*
 *	New population is divided into 3 sections:
 *		fittest parents  
 *		mutated crossovers (parents chosen on fitness roulette wheel)
 *		random individuals
 */
	pop_section = new_pop->pop_size/4;

/*
 *	Main loop - continue until specified number of generations
 *	  or else best rmse is less than specified limit.
 */			
	new_pop->min_rmse  = gen_alg->err_limit * 2.0;
	gen = 0;
	while((gen < gen_alg->max_generations) && (new_pop->min_rmse > gen_alg->err_limit))
	{
		gen++; 

		fprintf(stderr,"\n");
/*
 *	Copy (unmutated) fittest parents here so that the max fitness never decreases.
 */
		for(org=0; org < pop_section; org++)
			Copy_Chrom(new_pop, org, old_pop, old_pop->ordering[org]); 

/*   
 *	Choose 2 different parents using the fittness roulette wheel.
 *	Create crossed-over children.
 *	And mutate them.
 */			
		for(org=pop_section; org < 3*pop_section; org++)
		{	
			parent1 = Roulette_Wheel(old_pop->normal_fitness, old_pop->pop_size);
			do
			{
				parent2 = Roulette_Wheel(old_pop->normal_fitness, old_pop->pop_size);
			} while(parent2 == parent1);	
			Crossover(new_pop, org, old_pop, parent1, old_pop, parent2, params, 
gen_alg->crossover_prob); 
			Mutate(new_pop, org, gen_alg->mutation_prob, params);
		}

/*
 *	Insert random organisms in the last section - new blood to prevent in-breeding.
 */
		for(org=3*pop_section; org < new_pop->pop_size; org++)  
		{
			Randomize_Weights(net);
			Weights_To_Chrom(net, new_pop, org, params);    
		}

/*
 *	Replace clones by random organisms.
 */
/*		Remove_Clones(new_pop);   */

/*
 *	Calculate the fitness of the newly generated population.
 */
		new_pop->sum_fitness = 0.0;    
		new_pop->max_fitness = 0.0;
		new_pop->min_fitness = 1000000000.0;       
		new_pop->min_rmse  = 1000000000.0;   
	fprintf(stderr, "\nGeneration %ld - Evaluating fitness of organisms:", gen);

		for(org=0; org<new_pop->pop_size; org++)  
		{ 
			fitness = Eval_Fitness(new_pop, net, gen_alg, params, org); 
			if(!(org%20))
				fprintf(stderr,"\n");
			fprintf(stderr,"%ld ", (org+1));
			new_pop->sum_fitness += fitness;
			if(fitness > new_pop->max_fitness)
				new_pop->max_fitness = fitness;
			if(fitness < new_pop->min_fitness)
				new_pop->min_fitness = fitness;
			if(new_pop->rmse[org] < new_pop->min_rmse)
				new_pop->min_rmse = new_pop->rmse[org];
 		}

/*
 *	Normalize the fitnesses and reset the ordering array.
 *	Sort so that the ordering array indexes organisms by decreasing fitness.
 */
		Normalize_Fitnesses(new_pop);
		My_Sort(0, new_pop->pop_size-1, new_pop);

		fprintf(stdout, "\nFitness: max = %lf, ave = %lf, min = %lf , min rmse = %lf",
new_pop->max_fitness, new_pop->sum_fitness/new_pop->pop_size, new_pop->min_fitness,
new_pop->min_rmse);     
		fprintf(gen_alg->train_fp, "\nFitness: max = %lf, ave = %lf, min = %lf , min rmse = %lf",
new_pop->max_fitness, new_pop->sum_fitness/new_pop->pop_size, new_pop->min_fitness,
new_pop->min_rmse);
	
/*
 *	Exchange the old and new populations.
 */
		tmp_pop = old_pop;
		old_pop = new_pop;
		new_pop = tmp_pop;
	}  /* Next generation */

} /* End of Void Train_Net() */



Void Crossover(PopulationPtr new_pop, Int child, PopulationPtr old_pop1, Int parent1, 
PopulationPtr old_pop2, Int parent2, GAParamsPtr params, Double crossover_prob)
/*
 *	Create child organism in position 'child' of new_pop from parents in old populations.
 */
{
	Int x_site, gene, base_x, gene_x; 
	UInt mask;
	
	if(Flip(crossover_prob))
	{	
		x_site = Random_Int(0, new_pop->chrom_len * params->bits_per_weight-2); 
		
		gene_x = x_site/params->bits_per_weight;
		for(gene=0; gene<gene_x; gene++)
			new_pop->chrom[child][gene] = old_pop1->chrom[parent1][gene];
			 
		base_x = x_site%params->bits_per_weight;
		if(base_x != (params->bits_per_weight-1))
		{		
			mask =  (((UInt) 1)<<(base_x+1))-1;
			new_pop->chrom[child][gene_x] = old_pop1->chrom[parent1][gene_x] & mask;	
			new_pop->chrom[child][gene_x] += 
	old_pop2->chrom[parent2][gene_x] & (params->max_uint-mask);	
	    }
	    else
			new_pop->chrom[child][gene_x] = old_pop1->chrom[parent1][gene_x];
	    	     
	    
		for(gene=gene_x+1; gene< new_pop->chrom_len; gene++)
			new_pop->chrom[child][gene] = old_pop2->chrom[parent2][gene];   
			
		new_pop->changed[child] = TRUE;    
	}
	else
		Copy_Chrom(new_pop, child, old_pop1, parent1);    
		
    
} /* End of Void Crossover() */



Int Mutate(PopulationPtr pop, Int organism, Double prob, GAParamsPtr params)
/*
 *	Mutate specified organism in the population according to the
 *	  given mutation rate.
 */
{
	Int base, gene, num_mutations = 0;
	UInt g_val, mask;
	
	for(gene=0; gene<pop->chrom_len; gene++)
	{
		for(base=0; base<params->bits_per_weight; base++)
		{
			if(Flip(prob))
			{ 
				mask = ((UInt) 1)<<base;
				g_val = pop->chrom[organism][gene];
				if(g_val&mask) 
					g_val -= mask;
				else
					g_val += mask;
				pop->chrom[organism][gene] = g_val; 
				num_mutations++;
			}
		}
	} 
	if(num_mutations)
		pop->changed[organism] = TRUE;    

	return(num_mutations);
} /* End of Int Mutate() */ 

                               
                               
                               
Double Eval_Fitness(PopulationPtr pop, NeuralNetPtr net, GenAlgPtr gen_alg, 
	GAParamsPtr params, Int organism)  
/*
 *	Returns the fitness of an organism.
 */
{
	Int				point, i;
	Double			point_err, epoch_err;
    DoublePtr		out_arr;  
    PerformRec		perform;
    ResultsRec		response;

/*
 *	Return the previously calculated fitness if the organism hasn't changed.
 */
	if(!pop->changed[organism])
		return(pop->fitness[organism]);
		
	Chrom_To_Weights(net, pop, organism, params);

/*
 *	Set the initial input position and desired position to just before
 *	  the start of the input and output data arrays because of the
 *	  offset by one in array addressing of nodes and weights etc.
 */	
	gen_alg->y[net->num_layers] = params->inputs-1; 
	gen_alg->desired = params->outputs-1;   
	out_arr = params->responses-1;

/*
 *	Propagate through the first data point. 
 *	Move the input and desired output positions along.
 *	If we're testing then place the network outputs in the params->outputs space
 *	  for returning to the calling program. 
 */	
	epoch_err = Propagate(net, gen_alg);   
 	Init_Trading(params->prices[0], &perform, &response, 
gen_alg->y[1][1], gen_alg->init_capital, gen_alg->no_pos_zone);
	if(net->op_signal == BP_TEST)
	{
/*
 *	If it's testing then copy the network output into the array space for outputs.
 */
		for(i=1; i <= params->out_cols; i++)
		{
	    	out_arr[i] = gen_alg->y[1][i];
	    }   
	}
	    	
	gen_alg->y[net->num_layers] += params->in_cols; 
	gen_alg->desired += params->out_cols;
	out_arr += params->out_cols;
			
/*
 *	Step through the training examples. 
 *	Move the input and desired output positions along.
 *	If we're testing then place the network outputs in the params->outputs space
 *	  for returning to the calling program. 
 */	
	for(point=1; point < params->num_records; 
		point++, gen_alg->y[net->num_layers] += params->in_cols, 
		gen_alg->desired += params->out_cols, out_arr += params->out_cols)
	{	
		point_err = Propagate(net, gen_alg);
		epoch_err += point_err;  

		Buy_Sell_Signal(params->prices[point], &response, gen_alg->y[1][1]); 
		Gains_Losses(params->prices[point], &perform, &response,
gen_alg->transaction_size, gen_alg->transaction_cost);
		if(net->op_signal == BP_TEST)
		{
			for(i=1; i <= params->out_cols; i++)
		    	out_arr[i] = gen_alg->y[1][i]; 
		}
	}   	   
	epoch_err *= gen_alg->ave_batch / net->nodes_in_layer[1];        
	epoch_err = sqrt(epoch_err);
	pop->rmse[organism] = epoch_err;
	
/*
 *	Set the changed flag to false so that we don't have to keep recalculating
 *	  this organism's fitness.
 *	Return the fitness as 1/error.
 */
	Summary_P_And_L(&perform, &response, params->num_records, gen_alg->init_capital);
 	pop->fitness[organism] = Use_Fitness_Objective(epoch_err, &perform,
gen_alg->fitness_weights);
 	pop->changed[organism] = FALSE;   
		
	return(pop->fitness[organism]);
	
} /* End of Double Eval_Fitness() */


                                    
Void Test_Net(NeuralNetPtr net, GenAlgPtr gen_alg,  GAParamsPtr params, 
PopulationPtr pop)
/*
 *	Get the response (fitness) of all organisms to the test data.
 */
{
	Int	org;
	Double fitness;
/*
 *	Evaluate the fitnesses of all the organisms.
 *	Normalize the fitnesses and reset the ordering array.
 *	Sort so that the ordering array indexes organisms by increasing fitness.
 */
	pop->sum_fitness = 0.0;
	pop->max_fitness = 0.0;
	pop->min_fitness = 1000000000.0;
	for(org=0; org<pop->pop_size; org++)
	{   
		fitness = Eval_Fitness(pop, net, gen_alg, params, org);
		pop->sum_fitness += fitness;    

		if(fitness > pop->max_fitness)
			pop->max_fitness = fitness;
		if(fitness < pop->min_fitness)
			pop->min_fitness = fitness;
	} 
	Normalize_Fitnesses(pop);
	My_Sort(0, pop->pop_size-1, pop);
 	    
	fprintf(stdout,"\n\nTested %ld organisms,",pop->pop_size);	
	fprintf(stdout, " Fitness: max = %lf, ave = %lf, min = %lf\n",
pop->max_fitness, pop->sum_fitness/pop->pop_size , pop->min_fitness);  
	

} /* End of Void Test_Net() */
                               


