/*		bp.c			UBS			30/3/94			*/
/*
 *		Error Back Propagation algorithm.         
 *
 *	Array calling convention: 
 *		In the following subroutines arrays are counted from arr[1] to arr[num_elts], 
 *		not from arr[0] to arr[num_elts-1] (which is the normal C convention).
 *	Malloc() and MAKE_SOME() statements usually contain an extra "+ 1" to allow for this.
 *	If bias units and their weights are used they are stored in the zeroth elements of
 *	  the relevant arrays. 
 *	If bias units are not used the zeroth elements are still allocated but left unused. 
 *
 *	Top layer = output layer is layer[1]
 *  Bottom layer = input layer is layer[num_layers]
 */


#include	"header.h"
  
 
Double Propagate(NeuralNetPtr net, BackPropPtr bp)
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
			bp->z[layer][up_node] = net->diff[layer][up_node] (bp->x[layer][up_node], bp->y[layer][up_node]);				
		}
    }
    
/*
 *	Calculate the alphas and the total error.
 */
	for(up_node = 1; up_node <= net->nodes_in_layer[1]; up_node++)
	{
		bp->write_alpha[up_node] = bp->y[1][up_node]-bp->desired[up_node];	
 		out_err += bp->write_alpha[up_node] * (bp->write_alpha[up_node]);
	}		
	return(out_err);
} /* End of Double Propagate() */ 
        
    
     
Void Stochastic_Adjust_Weights(NeuralNetPtr net, BackPropPtr bp)
/*
 *	Adjust the weights settings in 'net' for the given data point.
 *	Uses new_alpha[] values set in previous call to Propagate().
 */
{                                    
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);
    
    if(bp->momentum_term)
	{
		for(layer=1; layer <= (net->num_layers - 1); layer++)
		{
			Swap_BackProp_Alphas(bp, net->nodes_in_layer[layer+1]);
			for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
			{
				bp->beta[up_node] = bp->read_alpha[up_node] * bp->z[layer][up_node];

				if(net->bias_present)
				{
					bp->delta[layer][up_node][0] = 
bp->learn_rate*bp->eta[layer][up_node] * bp->beta[up_node] +
bp->momentum * bp->delta[layer][up_node][0];
					bp->write_alpha[0] += 
net->weight[layer][up_node][0] * bp->beta[up_node];
				}		
									
				for(down_node=1; down_node <= net->nodes_in_layer[layer+1];down_node++)
				{
					bp->delta[layer][up_node][down_node] = 
-bp->learn_rate*bp->eta[layer][up_node] * bp->beta[up_node] * bp->y[layer+1][down_node]
+ bp->momentum * bp->delta[layer][up_node][down_node];
					bp->write_alpha[down_node] += 
net->weight[layer][up_node][down_node] * bp->beta[up_node];
	
				} /* Next down_node */	
			} /* Next up_node */   
	    } /* Next layer (not including bottom layer) */
    }
    else
	{	
		for(layer=1; layer <= (net->num_layers-1); layer++)
		{
			Swap_BackProp_Alphas(bp, net->nodes_in_layer[layer+1]);
			for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
			{
				bp->beta[up_node] = bp->read_alpha[up_node]*(bp->z[layer][up_node]);

				if(net->bias_present)
				{
					bp->delta[layer][up_node][0] = 
bp->learn_rate * bp->eta[layer][up_node] * (bp->beta[up_node]);
					bp->write_alpha[0] += 
net->weight[layer][up_node][0] * (bp->beta[up_node]);
				}
											
				for(down_node=1; down_node <= net->nodes_in_layer[layer+1]; down_node++)
				{
					bp->delta[layer][up_node][down_node] = 
-bp->learn_rate* bp->eta[layer][up_node] * bp->beta[up_node] * bp->y[layer+1][down_node];
					bp->write_alpha[down_node] += 
net->weight[layer][up_node][down_node] * (bp->beta[up_node]);
	
				} /* Next down_node */	
			} /* Next up_node */   
	    } /* Next layer (not including bottom layer) */
	}	
/*
 *	Update the weights according to the calculated delta's.
 */
	for(layer=1; layer <= (net->num_layers-1); layer++)
		for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
			for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++)
				net->weight[layer][up_node][down_node] += bp->delta[layer][up_node][down_node];

} /* End of Void Stochastic_Adjust_Weights() */       



Void Swap_BackProp_Alphas(BackPropPtr bp, Int num_alphas)
/*
 *	Swap the read_alpha and write_alpha pointers within the 'bp' structure. 
 *	Clear the required spaces in read_alpha[] to 0.0 before swapping.
 *	On exiting from this subroutine read_alpha[] contains valid info to be read,
 *	  and write_alpha[] is cleared to 0.0 in the rquired spaces in preparation to 
 *	  being rewritten.
 */
{
	DoublePtr temp = bp->read_alpha;
	Int node;
	
	for(node = 0; node <= num_alphas; node++)
		temp[node] = 0.0;
		
	bp->read_alpha = bp->write_alpha;
	bp->write_alpha = temp;

} /* End of Void Swap_BackProp_Alphas() */


     
Void Batch_Calc_Increments(NeuralNetPtr net, BackPropPtr bp)
/*
 *	Calculate the increments of the increments to be added at the end of the current epoch,
 *	  for the weights in 'net' for the given data point.
 *	Uses new_alpha[] values set in previous call to Propagate().
 */
{                                    
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);
    
	for(layer=1; layer <= (net->num_layers-1); layer++)
	{
		Swap_BackProp_Alphas(bp, net->nodes_in_layer[layer+1]);
		for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
		{
			bp->beta[up_node] = bp->read_alpha[up_node] * bp->z[layer][up_node];

			if(net->bias_present)
			{
				bp->delta[layer][up_node][0] += 
bp->learn_rate * bp->eta[layer][up_node] * bp->beta[up_node];
				bp->write_alpha[0] += 
net->weight[layer][up_node][0] * bp->beta[up_node];
			}
										
			for(down_node = 1; down_node <= net->nodes_in_layer[layer+1]; down_node++)
			{
				bp->delta[layer][up_node][down_node] -= 
bp->learn_rate * bp->eta[layer][up_node] * bp->beta[up_node] * bp->y[layer+1][down_node];
				bp->write_alpha[down_node] += 
net->weight[layer][up_node][down_node] * bp->beta[up_node];
	
			} /* Next down_node */	
		} /* Next up_node */   
    } /* Next layer (not including bottom layer) */

} /* End of Void Batch_Calc_Increments() */       



Void Batch_Adjust_Weights(NeuralNetPtr net, BackPropPtr bp)
/*
 *	Update the weights according to the calculated delta's at the
 *	  end of an epoch.
 */   
{
	Int layer, up_node, down_node, no_bias = (1-net->bias_present);

	if(bp->momentum_term)
	{
		for(layer=1; layer <= (net->num_layers-1); layer++)
			for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++) 
			{				
				for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++)
				{
					bp->delta[layer][up_node][down_node] =   
bp->delta[layer][up_node][down_node] * bp->ave_batch +  
bp->momentum * bp->m_delta[layer][up_node][down_node];
					net->weight[layer][up_node][down_node] += 
bp->delta[layer][up_node][down_node];

					bp->m_delta[layer][up_node][down_node] = bp->delta[layer][up_node][down_node];
					bp->delta[layer][up_node][down_node] = 0.0;
				}      
			}
	}
	else
	{
		for(layer=1; layer <= (net->num_layers-1); layer++)
			for(up_node = 1; up_node <= net->nodes_in_layer[layer]; up_node++)
				for(down_node = no_bias; down_node <= net->nodes_in_layer[layer+1]; down_node++)
				{
					net->weight[layer][up_node][down_node] += 
bp->delta[layer][up_node][down_node];   /* 	* bp->ave_batch);   	*/
					bp->delta[layer][up_node][down_node] = 0.0;
				}
	}

} /* End of Void Batch_Adjust_Weights() */

                                    
                                    
Void Train_Net(NeuralNetPtr net, BackPropPtr bp, DoublePtr input_data, 
Int in_cols, DoublePtr output_data, Int out_cols, Int num_records)
/*
 *	Perform the Error Back-Propagation algorithm on the training data.
 */
{
	Int				point, epoch;
	Double			point_err, epoch_err, old_err;

	epoch = 0;
	old_err = (bp->err_limit)*3.0;
	epoch_err = (bp->err_limit)*2.0;
	while((epoch < bp->max_iterations) && (epoch_err > bp->err_limit))
	{
		epoch_err = 0.0;
		epoch++; 

/*
 *	Set the initial input position and desired position to just before
 *	  the start of the input and output data arrays because of the
 *	  offset by one in array addressing of nodes and weights etc.
 */	
		bp->y[net->num_layers] = input_data-1; 
		bp->desired = output_data-1; 

/*
 *	Step through the training examples. 
 *	Move the input and desired output positions along.
 */			
		for(point=0; point < num_records; 
			point++, bp->y[net->num_layers] += in_cols, 
			bp->desired += out_cols)
		{	

			point_err = Propagate(net, bp); 
			epoch_err += point_err;
			if(bp->update == UD_STOCHASTIC)
				Stochastic_Adjust_Weights(net, bp);
			else	
				Batch_Calc_Increments(net, bp); 
		}  /* Next training point. */

		epoch_err *= bp->ave_batch / net->nodes_in_layer[1];        
		epoch_err = sqrt(epoch_err);
	
		if(bp->update == UD_BATCH)
			Batch_Adjust_Weights(net, bp);  
	
	
		if(bp->increasing)
		{	
			if( epoch_err > old_err*0.9999 )
				bp->learn_rate *= 0.1;
			else
				bp->learn_rate *= 1.001;
	        if(bp->learn_rate < 0.000001)
	        	bp->learn_rate = 0.000002; 
        }
	        	
		old_err = epoch_err;
		printf("\nTraining epoch %ld, Error x 1000 = %lf, learn-rate = %lf",
epoch, epoch_err*1000.0, bp->learn_rate);
	}  /* Next epoch */
	

} /* End of Void Train_Net() */
                               
                               
 
Void Test_Net(NeuralNetPtr net, BackPropPtr bp, DoublePtr input_data, 
Int in_cols, DoublePtr output_data, Int out_cols, Int num_records)
{
	Int				point, epoch;
	Double			point_err, epoch_err, old_err;

	epoch = 0;
	old_err = (bp->err_limit)*3.0;

	epoch_err = 0.0;
	epoch++; 

/*
 *	Set the initial input position and desired position to just before
 *	  the start of the input and output data arrays because of the
 *	  offset by one in array addressing of nodes and weights etc.
 */	
	bp->y[net->num_layers] = input_data-1; 
	bp->desired = output_data-1;   
	bp->y[1] = output_data-1;

/*
 *	Step through the training examples. 
 *	Move the input and desired output positions along.
 */			
	for(point=0; point < num_records; 
		point++, bp->y[net->num_layers] += in_cols, 
		bp->desired += out_cols, bp->y[1] += out_cols)
	{	
		point_err = Propagate(net, bp);
		epoch_err += point_err;
	}   	   
	epoch_err *= bp->ave_batch / net->nodes_in_layer[1];        
	epoch_err = sqrt(epoch_err);
	old_err = epoch_err;


} /* End of Void Test_Net() */



