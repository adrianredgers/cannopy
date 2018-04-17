/*			prtype.h			UBS			30/3/94		*/

/*
 *	Inclusions file of function prototypes.
 */


/*
 *	Simulate #PRAGMA ONCE compiler directive.
 */
#ifndef PRTYPE_H
#define		PRTYPE_H	1

/*
 *	In get_conf.c
 */
Void 		Get_Configuration(DataPairsPtr *train_data, DataPairsPtr *test_data, 
				DataFormatPtr *train_arr, DataFormatPtr *test_arr,
				DataPairsPtr *train_price_data, DataPairsPtr *test_price_data, 
				char *bp_cfg_file, char *cfg_file, char *results_file, char *summary_file,
				 Double *init_capital, Double *transaction_size, Double *transaction_cost,
				Double *no_pos_zone);  
Int 		Scan_String(FILE *fp, char *new_word, char *file_name, Int item);
Int 		Scan_Int(FILE *fp, Int *value, char *file_name, Int item);
Int 		Scan_Float(FILE *fp, Double *value, char *file_name, Int item);
Void 		Error_Exit(char *mesg, char *new_word, Int item, char *file_name);
Void 		Check_Points(Int *num_train_points, Int *num_test_points, 
				Int *num_data_points, Int *ignore_train_points, Int *lead_in_period, 
				Int item, char *file_name);
Int 		Check_Triple(Int *total, Int *a, Int *b);
 
/*
 *	In allocate.c
 */
VoidPtr 	My_Free(VoidPtr ptr, Int num_items, int item_size);
VoidPtr 	My_Malloc(Int num_items, int item_size);
Int			Get_Mem_Total(Bool print_it); 
 
/*
 *	In Utils.c
 */
Void 		Acknowledge(char *mesg);
Void 		Copy_Ints(Int from_arr[], Int to_arr[], Int num_elts);
Void 		Get_File_Data(char *file_name, DataPairsPtr data, Int ignore_points);
Int 		Count_File_Data(char *file_name, Int num_fields);
Double		Prob_Distrib(Double min_limit, Double max_limit, Double pdf(Double x));
Double		Example_PDF(Double x);        
FILE 		*Open_File(char *file_name, char *mode, char *err_mesg);
Void 		Make_Filename(char *new_file, char *prefix_file, char *dot_suffix);


/*
 *	In setupnn.c
 */
Double		Sigmoid(Double x);
Double		Diff_Sigmoid(Double x, Double y);
Double		Tanh(Double x);
Double		Diff_Tanh(Double x, Double y);
Double		Tanh2(Double x);
Double		Diff_Tanh2(Double x, Double y);
Double		Tanh3(Double x);
Double		Diff_Tanh3(Double x, Double y);
Double		Tanh4(Double x);
Double		Diff_Tanh4(Double x, Double y);
Double		Identity(Double x);
Double		Diff_Identity(Double x, Double y);
Double		Step(Double x);
Double		Diff_Step(Double x, Double y);

NeuralNetPtr 	Con_NeuralNet(Int num_layers, Int bias_present, IntPtr nodes_in_layer,
					ActivFuncPtrPtr all_funcs, DiffFuncPtrPtr all_diffs, 
					char weights_infile[], char weights_outfile[], BPOpSignal op_signal);
VoidPtr 		Des_NeuralNet(NeuralNetPtr net);
BackPropPtr 	Con_BackProp(NeuralNetPtr net, Double learn_rate, Bool increasing,
					Double momentum, Int max_iterations, Double err_limit, 
					UpdateType update, Int epoch_size, BPOpSignal op_signal);
VoidPtr			Des_BackProp(BackPropPtr bp, NeuralNetPtr net);

Void 			Randomize_Weights(NeuralNetPtr net);  
Void 			Zeroize_Deltas(NeuralNetPtr net, BackPropPtr bp);
Void 			Get_File_Weights(NeuralNetPtr net);  
Void 			Weights_File_Error(NeuralNetPtr net, char *mesg, Int item);
Void 			Save_File_Weights(NeuralNetPtr net);


/*
 *	In bp.c
 */
Double 	Propagate(NeuralNetPtr net, BackPropPtr bp);	
Void	Stochastic_Adjust_Weights(NeuralNetPtr net, BackPropPtr bp);
Void 	Swap_BackProp_Alphas(BackPropPtr bp, Int num_alphas);
Void 	Batch_Calc_Increments(NeuralNetPtr net, BackPropPtr bp);
Void 	Batch_Adjust_Weights(NeuralNetPtr net, BackPropPtr bp);
Void 	Train_Net(NeuralNetPtr net, BackPropPtr bp, DoublePtr input_data, 
			Int in_cols, DoublePtr output_data, Int out_cols, Int num_records);
Void 	Test_Net(NeuralNetPtr net, BackPropPtr bp, DoublePtr input_data, 
			Int in_cols, DoublePtr output_data, Int out_cols, Int num_records);

/*
 *	In pandl.c
 */
Void 	Profit_And_Loss(DataFormatPtr response, DataPairsPtr data, DataPairsPtr prices,
			char *mesg, char *results_file, char *summary_file, 
			 Double init_capital, Double transaction_size, Double transaction_cost,
			Double no_pos_zone);


/*
 *	In DataLoop.c
 */
DataLoopPtr	Con_DataLoop(Int len);
DataLoopPtr	Des_DataLoop(DataLoopPtr data);
Double 		Get_Item_DataLoop(Int my_pos, DataLoopPtr data);
Void 		Add_Item_DataLoop(Double val, DataLoopPtr data);  

/*
 *	In bp_strip.c
 */
Void		BP(DoublePtr inputs, Int in_cols, DoublePtr outputs, Int out_cols, 
				Int num_records, char *config_file, BPOpSignal op_signal); 

/* 
 *	In bp_conf.c
 */
Void 		Get_BP_Configuration(NeuralNetPtr *net, BackPropPtr *bp, char *config_file, 
				BPOpSignal op_signal, Int in_cols, Int out_cols, Int num_records);
Int 		Scan_Function(ActivFunc *net_func, DiffFunc *net_diff, FILE *fp,
				char *file_name, Int item);

/* 
 *	In prepare.c
 */
Void 			Prepare_Data(DataPairsPtr data, DataFormatPtr data_format);
Void 			Prepare_Prices(DataPairsPtr data, DataFormatPtr df);
DataPairsPtr 	Con_DataPairs(Int num_points, Int num_inputs, Int num_outputs, Int num_fields, 
					Int ignore, char *file_name, Bool data_feed);
VoidPtr 		Des_DataPairs(DataPairsPtr data);
DataFormatPtr	Con_DataFormat(Int window, Int prediction_delay, Int pattern_delay,
					Int in_cols, Int out_cols, Int num_records);
VoidPtr	 		Des_DataFormat(DataFormatPtr data_format);


/*
 *	In calcperf.c
 */
Void 	Buy_Sell_Signal(Double price, ResultsRecPtr response, Double network_output_val); 
Void 	Gains_Losses(Double price, PerformRecPtr perform, ResultsRecPtr response,
			Double transaction_size, Double transaction_cost);
Double 	Use_Fitness_Objective(Double rmse, PerformRecPtr perform,
			FitnessObjective fitness_objective);
Void	Init_Trading(Double price, PerformRecPtr perform, ResultsRecPtr response,
			Double network_output, Double init_capital, Double no_pos_zone);
Void 	Summary_P_And_L(PerformRecPtr perform, ResultsRecPtr response, 
			Int num_records, Double init_capital);

/*
 *	In statutil.c
 */
Double Cum_Norm_Dist(Double z);    
Double Confidence (Double z, Int tail);


/*
 * #endif from #ifndef PRTYPE_H
 */
#endif 

