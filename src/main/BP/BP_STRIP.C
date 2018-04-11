/*			bp_strip.c			UBS			3/5/94				*/

/*
 *	Stripped-down back-propagation function.
 */ 


#include "header.h"


 
Void BP(DoublePtr inputs, Int in_cols, DoublePtr outputs, Int out_cols, Int num_records, 
		char *config_file, BPOpSignal op_signal)
/*   
 *	Create a network using the given configuration file.
 *	If op_signal is BP_TRAIN then train the net on the given inputs and outputs.
 *	Else if op_signal is BP_TEST then run the network on the given inputs and place the
 *	  results in the output space provided.
 */
{
	NeuralNetPtr	net;
	BackPropPtr		bp; 

	Get_BP_Configuration(&net, &bp, config_file, op_signal, in_cols, out_cols, num_records);

	if(op_signal == BP_TEST)
 		Test_Net(net, bp, inputs, in_cols, outputs, out_cols, num_records);
	else
		Train_Net(net, bp, inputs, in_cols, outputs, out_cols, num_records);

	Save_File_Weights(net);
/*
 *	Destructors for the structures allocated initially.
 *	Destroy items in the inverse order to which they were constructed.
 */ 	
	Des_BackProp(bp, net);
	Des_NeuralNet(net);	

} /* End of Void BP() */  
 
 
 
 
 
 