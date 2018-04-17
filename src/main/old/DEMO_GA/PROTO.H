/*			proto.h			UBS			20/4/94			*/

/*
 *	Prototypes of procedures and functions used by the GenNet project.
 */
 

#ifndef PROTO_H
#define		PROTO_H	1

 
/*
 *	In rnd_sort.c
 */
Double 	Random_Double(Double lo, Double hi);
Int		Random_Int(Int low, Int high);
Int		Random_UInt(UInt low, UInt high);
Int		Random_Bit(void);
Bool	Flip(Double probability); 
Void	Random_Chrom(PopulationPtr pop, Int organism, GAParamsPtr params);
Void	My_Sort(Int start, Int finish, PopulationPtr pop);
                                      
/*
 *	In statutil.c
 */
Double	Cum_Norm_Dist(Double z);
Double	Confidence(Double z, Int tail); 

/*
 *	In readfile.c
 */  
/*
Void	Read_File(Int curr_rec, DataNormalPtr data);

DataNormalPtr Con_DataNormal(String data_file, Int input_start_col, Int num_inputs, 
				Int output_start_col, Int num_outputs, Int total_fields, Int price_col, 
				Int record_start, Int num_records, Bool is_normal);
DataNormalPtr Des_DataNormal(DataNormalPtr data); 
*/

 
/*
 *	In get_conf.c
 */
Void 		Get_Configuration(DataPairsPtr *train_data, DataPairsPtr *test_data, 
				DataFormatPtr *train_arr, DataFormatPtr *test_arr,
				DataPairsPtr *train_price_data, DataPairsPtr *test_price_data, 
				DataPairsPtr *results, char *results_file, char *summary_file, 
				char *bp_cfg_file, char *cfg_file,
				Double *init_capital, Double *transaction_size, Double *transaction_cost,
				Double *no_pos_zone);  
Void 		Get_Trading_Params(char *ga_cfg_file, Double *init_capital,  
				Double *transaction_size, Double *transaction_cost, Double *no_pos_zone);
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
Int			Roulette_Wheel(DoublePtr wheel, Int num_slots);
Void 		Normalize_Fitnesses(PopulationPtr pop);
Void		Copy_Chrom(PopulationPtr to_pop, Int to_organism, PopulationPtr from_pop, 
				Int from_organism);
Void		Copy_Bytes(VoidPtr to, VoidPtr from, Int num_bytes);
Void 		Reorder_Population(PopulationPtr pop);
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
					ActivFuncPtrPtr all_funcs, 
					char weights_infile[], Int num_in_organisms, char weights_outfile[], 
					Int num_out_organisms, BPOpSignal op_signal);
VoidPtr 		Des_NeuralNet(NeuralNetPtr net);
GenAlgPtr 		Con_GenAlg(NeuralNetPtr net, Double mutation_prob, Double crossover_prob,
					Int max_generations, Double err_limit, 
					Int epoch_size, BPOpSignal op_signal, Double *fitness_weights,
					Double transaction_size, Double transaction_cost, Double init_capital,
					Double no_pos_zone, char *cfg_file);
VoidPtr			Des_GenAlg(GenAlgPtr bp, NeuralNetPtr net);

Void 		Randomize_Weights(NeuralNetPtr net);  
Void 		Save_Organisms(NeuralNetPtr net, PopulationPtr pop, GAParamsPtr params);
Void 		Save_File_Weights(NeuralNetPtr net, FILE *fp);
Int 		Load_Organisms(NeuralNetPtr net, PopulationPtr pop, GAParamsPtr params);
Void 		Get_File_Weights(NeuralNetPtr net, FILE *fp);

Void 		Weights_To_Chrom(NeuralNetPtr net, PopulationPtr pop, Int organism, 
				GAParamsPtr params);
Void 		Chrom_To_Weights(NeuralNetPtr net, PopulationPtr pop, Int organism, 
				GAParamsPtr params);
Void 		Weights_File_Error(NeuralNetPtr net, char *mesg, Int item);

/*
 *	In ga.c
 */
Void	Init_Population(PopulationPtr pop, NeuralNetPtr net, 
			GAParamsPtr ga_params, GenAlgPtr ga);
Double 	Propagate(NeuralNetPtr net, GenAlgPtr bp);	
Void	Stochastic_Adjust_Weights(NeuralNetPtr net, GenAlgPtr bp);
Void 	Swap_GenAlg_Alphas(GenAlgPtr bp, Int num_alphas);
Void 	Batch_Calc_Increments(NeuralNetPtr net, GenAlgPtr bp);
Void 	Batch_Adjust_Weights(NeuralNetPtr net, GenAlgPtr bp);
Void 	Train_Net(NeuralNetPtr net, GenAlgPtr gen_alg, GAParamsPtr params,
			PopulationPtr old_pop, PopulationPtr new_pop);
Void 	Test_Net(NeuralNetPtr net, GenAlgPtr gen_alg, GAParamsPtr params,
			PopulationPtr pop);
Double	Eval_Fitness(PopulationPtr pop, NeuralNetPtr net, GenAlgPtr gen_alg, 
			GAParamsPtr params, Int organism);
Void	Crossover(PopulationPtr new_pop, Int child, PopulationPtr old_pop1, 
			Int parent1, PopulationPtr old_pop2, Int parent2, GAParamsPtr params,
			Double crossover_prob); 
Int 	Mutate(PopulationPtr pop, Int organism, Double prob, GAParamsPtr params);

/*
 *	In calcperf.c
 */
Void 	Buy_Sell_Signal(Double price, ResultsRecPtr response, Double network_output_val); 
Void 	Gains_Losses(Double price, PerformRecPtr perform, ResultsRecPtr response,
			Double transaction_size, Double transaction_cost);
Double 	Use_Fitness_Objective(Double rmse, PerformRecPtr perform,
			DoublePtr fitness_weights);
Void	Init_Trading(Double price, PerformRecPtr perform, ResultsRecPtr response,
			Double network_output, Double init_capital, Double no_pos_zone);
Void 	Summary_P_And_L(PerformRecPtr perform, ResultsRecPtr response, 
Int num_records, Double init_capital);

/*
 *	In pandl.c
 */
Void 	Profit_And_Loss(DataFormatPtr response, DataPairsPtr data, DataPairsPtr prices,
			char *mesg, char *results_file, char *summary_file, 
			Double init_capital, Double transaction_size , Double transaction_cost,
			Double no_pos_zone);

/*
 *	In ga_strip.c
 */
Void			GA(DoublePtr inputs, Int in_cols, DoublePtr outputs, Int out_cols, 
					Int num_records, DoublePtr prices, DoublePtr responses, 
					char *config_file, BPOpSignal op_signal); 
GAParamsPtr		Con_GAParams(DoublePtr inputs, Int in_cols, DoublePtr outputs, 
					Int out_cols, Int num_records, DoublePtr prices, DoublePtr responses);  
GAParamsPtr		Des_GAParams(GAParamsPtr ga_params);
PopulationPtr	Con_Population(Int pop_size, Int chrom_len);  
PopulationPtr 	Des_Population(PopulationPtr pop);

/* 
 *	In ga_conf.c
 */
Void 		Get_GA_Configuration(NeuralNetPtr *net, GenAlgPtr *bp, PopulationPtr *pop, 
				char *config_file, 
				BPOpSignal op_signal, Int in_cols, Int out_cols, Int num_records);
Int 		Scan_Function(ActivFunc *net_func, DiffFunc *net_diff, FILE *fp,
				char *file_name, Int item);
Int			Scan_Fitness_Objective(Double *fitness_weights, FILE *fp,
				char *file_name, Int item, char *new_word);
/*
 *	In prepare.c
 */
DataPairsPtr 	Con_DataPairs(Int num_points, Int num_inputs, Int num_outputs, 
					 Int num_fields, Int ignore, char *file_name, Bool data_feed);
VoidPtr 		Des_DataPairs(DataPairsPtr data);
DataFormatPtr	Con_DataFormat(Int window, Int prediction_delay, Int pattern_delay,
					Int in_cols, Int out_cols, Int num_records);
VoidPtr	 		Des_DataFormat(DataFormatPtr data_format);
Void 			Prepare_Data(DataPairsPtr data, DataFormatPtr data_format);
Void 			Prepare_Prices(DataPairsPtr data, DataFormatPtr df);
 
 
        
#endif        











 
