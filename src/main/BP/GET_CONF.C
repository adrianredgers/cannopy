/*				get_conf.c			UBS				6/4/94				*/
/*
 *	Get neural network configuration, parameters for back-prop algorithm and
 *	  details of test and training data.
 *
 *	If an argument is given then look for these details in the specified file,
 *	Otherwise get the info from the default configuration file "bp.cfg"     
 */


#include	"header.h"





Void Get_Configuration(DataPairsPtr *train_data, DataPairsPtr *test_data, 
DataFormatPtr *train_arr, DataFormatPtr *test_arr,
DataPairsPtr *train_price_data, DataPairsPtr *test_price_data,
char *bp_cfg_file, char *cfg_file, char *results_file, char *summary_file, 
Double *init_capital, Double *transaction_size, Double *transaction_cost, 
Double *no_pos_zone)
/*
 *	Get DataPairs configurations from given file, or from
 *	  default file if none specified.
 *	Construct train and test DataPairs structures, and original price data for P/L calc'n.
 */
{
	char  file_name[100], new_word[100], train_file[100], test_file[100], train_price_file[100], 
			test_price_file[100];
	FILE *fp;
	Double	float_val;    
	Int item, old_item;
	Int window = 0, pattern_delay = 1, prediction_delay = 1;
	Bool test_data_feed = FALSE;                  
	Int int_val;

	Int  num_train_points = -1, num_test_points = -1, 
			ignore_train_points = -1, ignore_test_points = -1, 
			input_fields = -1, output_fields = -1,
			num_fields = -1, num_records;

	*transaction_size = 10000.0;
	*transaction_cost = 10.0;
	*init_capital = 10000.0;
	*no_pos_zone = 0.02;
	
/*
 *	Open specified configuration file of default file. 
 */
    strcpy(file_name,cfg_file);
    fp = Open_File(file_name, "r", "Get_Configuration()");
	
/*
 *	Go through the items in the configuration file.   
 *	Before entering the loop initialize flags to show new item got,
 *	  and no data file name got.
 *	Get first item from configuration file.
 *	Go through parsers for the various statements: 
 *		e.g. "train_data ..." , "test_data ... " etc.
 *	At the end of each statement parser get the next item and go on through the list of
 *	  statement parsers. 
 *	Go back to the beginning of the loop if necessary, but don't go through the loop
 *	  more than once if no new recognized items were got. 	
 */	                            
 	old_item = 0; 
 	test_file[0] = '\0';
 	train_file[0] = '\0';
 	train_price_file[0] = '\0';
 	test_price_file[0] = '\0';
 	results_file[0] = '\0';
    cfg_file[0] = '\0';
    bp_cfg_file[0] = '\0';

	item = Scan_String(fp, new_word, file_name, 0);
	while(!feof(fp))
	{

/*
 *	Check that current keyword item has been used at all.
 */
		if(old_item == item)
			Error_Exit("\nUnknown keyword.",new_word, item, file_name);			
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
				item = Scan_String(fp, new_word, file_name, item);
			} while(stricmp(new_word, "*/"));
			item = Scan_String(fp, new_word, file_name, item);
		} /* End of parse comments */			
	

/*
 *	Parse statements like:    
 *
 *		window 5 
 *		window 5 pattern_delay 1    
 *		window 5 prediction_delay 1    
 *		window 5 pattern_delay 1  prediction_delay 1  
 *
 */					
		if(!stricmp(new_word, "window"))
		{
			if(window > 0)
				Error_Exit("\nWindow length specified twice.",new_word,item, file_name);			
			item = Scan_Int(fp, &int_val, file_name, item);   
			window = int_val;
			if(window < 0)
				Error_Exit("\nWindow length out of range.",new_word,item, file_name);			
			item = Scan_String(fp, new_word, file_name, item);      
			if(!stricmp(new_word, "pattern_delay"))
			{
				item = Scan_Int(fp, &int_val, file_name, item);   
				pattern_delay = int_val;
				if(pattern_delay <= 0)
					Error_Exit("\nPattern-delay out of range.",new_word,item, file_name);
				item = Scan_String(fp, new_word, file_name, item);      
			}			
			if(!stricmp(new_word, "prediction_delay"))
			{
				item = Scan_Int(fp, &int_val, file_name, item);   
				prediction_delay = int_val;
				if(prediction_delay <= 0)
					Error_Exit("\nPrediction-delay out of range.",new_word,item, file_name);
				item = Scan_String(fp, new_word, file_name, item);      
			}			
		} /* End of parse "window ..." */			


/*
 *	Parse statements like:    
 *
 *		train_data "foo.dat"    
 *		train_data "foo.dat" records 400  
 *		train_data "foo.dat" ignore 10    
 *		train_data "foo.dat" records 400 ignore 10    
 *
 */					
		if(!stricmp(new_word, "train_data"))
		{
			if(train_file[0] != '\0')
				Error_Exit("\nTraining data-file specified twice.",new_word,item, file_name);			
			item = Scan_String(fp, train_file, file_name, item);
			item = Scan_String(fp, new_word, file_name, item);     
			if(!strcmp(new_word, "records"))
			{
				item = Scan_Int(fp, &int_val, file_name, item);   
				num_train_points = int_val;
				item = Scan_String(fp, new_word, file_name, item); 
			}    
			if(!strcmp(new_word, "ignore"))
			{
				item = Scan_Int(fp, &int_val, file_name, item);   
				ignore_train_points = int_val;
				item = Scan_String(fp, new_word, file_name, item); 
			}    
				 
		} /* End of parse "train_data ..." */			

/*
 *	Parse statements like:    
 *
 *		bp_config "bp.cfg"    
 */					
		if(!stricmp(new_word, "algorithm_config"))
		{
			if(bp_cfg_file[0] != '\0')
				Error_Exit("\nAlgorithm configuration file specified twice.",new_word,item, file_name);			
			item = Scan_String(fp, bp_cfg_file, file_name, item);
			item = Scan_String(fp, new_word, file_name, item);     
		} /* End of parse "algorithm_config ..." */			

/*
 *	Parse statements like:    
 *
 *		train_price_data "foo.dat"   
 *
 */					
		if(!stricmp(new_word, "train_price_data"))
		{
			if(train_price_file[0] != '\0')
				Error_Exit("\nTrain price data-file specified twice.",new_word,item, file_name);			
			item = Scan_String(fp, train_price_file, file_name, item);
			item = Scan_String(fp, new_word, file_name, item);     
				 
		} /* End of parse "train_price_data ..." */			

/*
 *	Parse statements like:    
 *
 *		test_price_data "foo.dat"   
 */					
		if(!stricmp(new_word, "test_price_data"))
		{
			if(test_price_file[0] != '\0')
				Error_Exit("\nTest price data-file specified twice.",new_word,item, file_name);			
			item = Scan_String(fp, test_price_file, file_name, item);
			item = Scan_String(fp, new_word, file_name, item);     
		} /* End of parse "test_price_data ..." */			


/*
 *	Parse statements like:    
 *
 *		result_file "foo.dat"    
 *
 */					
		if(!stricmp(new_word, "results_file"))
		{
			if(results_file[0] != '\0')
				Error_Exit("\nResults data-file specified twice.",new_word,item, file_name);			
			item = Scan_String(fp, results_file, file_name, item);
			item = Scan_String(fp, new_word, file_name, item); 
		} /* End of parse "results_file ..." */			

/*
 *	Parse statements like:    
 *
 *		next_config "bp5.cfg"    
 *
 */					
		if(!stricmp(new_word, "next_config"))
		{
			if(cfg_file[0] != '\0')
				Error_Exit("\nNext configuration file specified twice.",new_word,item, file_name);			
			item = Scan_String(fp, cfg_file, file_name, item);
			item = Scan_String(fp, new_word, file_name, item); 
		} /* End of parse "next_config ..." */			

/*
 *	Parse statements like:    
 *
 *		test_data "foo.dat"    
 *		test_data "foo.dat" records 400    
 *		test_data "foo.dat" ignore 10    
 *		test_data "foo.dat" records 400 ignore 10    
 *		test_data "foo.dat" datafeed   
 *		test_data "foo.dat" datafeed ignore 10    
 *
 */					
		if(!stricmp(new_word, "test_data"))
		{
			if(test_file[0] != '\0')
				Error_Exit("\nTest data-file specified twice.",new_word,item, file_name);			
			item = Scan_String(fp, test_file, file_name, item);
			item = Scan_String(fp, new_word, file_name, item);     
			if(!stricmp(new_word, "records"))
			{
				item = Scan_Int(fp, &int_val, file_name, item);   
				num_test_points = int_val;
				item = Scan_String(fp, new_word, file_name, item); 
			}
			else if(!stricmp(new_word, "data_feed"))
			{
				test_data_feed = TRUE;
				item = Scan_String(fp, new_word, file_name, item); 
			}
			    
			if(!stricmp(new_word, "ignore"))
			{
				item = Scan_Int(fp, &int_val, file_name, item);   
				ignore_test_points = int_val;
				item = Scan_String(fp, new_word, file_name, item); 
			}    
				 
		} /* End of parse "test_data ..." */			

	

/*
 *	Parse statements like:    
 *
 *		transaction_size 10000.0      
 *
 */					
		if(!stricmp(new_word, "transaction_size"))
		{
			item = Scan_Float(fp, &float_val, file_name, item);   
			*transaction_size = float_val;
			if(*transaction_size < 0.0)
				Error_Exit("\nTransaction size out of range.",new_word,item, file_name);			
			item = Scan_String(fp, new_word, file_name, item);      
		} /* End of parse "transaction_size ..." */			


/*
 *	Parse statements like:    
 *
 *		transaction_cost 10      
 *
 */					
		if(!stricmp(new_word, "transaction_cost"))
		{
			item = Scan_Float(fp, &float_val, file_name, item);   
			*transaction_cost = float_val;
			if(*transaction_cost < 0.0)
				Error_Exit("\nTransaction cost out of range.",new_word,item, file_name);			
			item = Scan_String(fp, new_word, file_name, item);      
		} /* End of parse "transaction_cost ..." */			


/*
 *	Parse statements like:    
 *
 *		init_capital 10000      
 *
 */					
		if(!stricmp(new_word, "init_capital"))
		{
			item = Scan_Float(fp, &float_val, file_name, item);   
			*init_capital = float_val;
			if(*init_capital < 0.0)
				Error_Exit("\nInitial capital out of range.",new_word,item, file_name);			
			item = Scan_String(fp, new_word, file_name, item);      
		} /* End of parse "init_capital ..." */			

	

/*
 *	Parse statements like:    
 *
 *		no_pos_zone 0.02      
 *
 */					
		if(!stricmp(new_word, "no_pos_zone"))
		{
			item = Scan_Float(fp, no_pos_zone, file_name, item);   
			if(*no_pos_zone < 0.0)
				Error_Exit("\nNo-position zone out of range.",new_word,item, file_name);			
			item = Scan_String(fp, new_word, file_name, item);      
		} /* End of parse "no_pos_zone ..." */			



/*
 *	Parse statements like:    
 *
 *		input_fields 3     
 *
 */					
		if(!stricmp(new_word, "input_fields"))
		{
			if(input_fields > 0)
				Error_Exit("\nInput-fields specified twice.",new_word,item, file_name);			
			item = Scan_Int(fp, &int_val, file_name, item);   
			input_fields = int_val;
			if(input_fields <= 0)
				Error_Exit("\nInput-fields out of range.",new_word,item, file_name);			
			item = Scan_String(fp, new_word, file_name, item);      
		} /* End of parse "input_fields ..." */			

/*
 *	Parse statements like:    
 *
 *		num_fields 6     
 *
 */					
		if(!stricmp(new_word, "num_fields"))
		{
			if(num_fields > 0)
				Error_Exit("\nNumber of fields specified twice.",new_word,item, file_name);			
			item = Scan_Int(fp, &int_val, file_name, item);   
			num_fields = int_val;
			if(num_fields <= 0)
				Error_Exit("\nNumber of fields out of range.",new_word,item, file_name);			
			item = Scan_String(fp, new_word, file_name, item);      
		} /* End of parse "num_fields ..." */			

/*
 *	Parse statements like:    
 *
 *		output_fields 3     
 *
 */					
		if(!stricmp(new_word, "output_fields"))
		{
			if(output_fields >= 0)
				Error_Exit("\nOutput-fields specified twice.",new_word,item, file_name);			
			item = Scan_Int(fp, &int_val, file_name, item);   
			output_fields = int_val;
			if(output_fields < 0)
				Error_Exit("\nOutput-fields out of range.",new_word,item, file_name);			
			item = Scan_String(fp, new_word, file_name, item);      
		} /* End of parse "output_fields ..." */			

	
	} /* End of while not EOF */		
	fclose(fp);

/*
 *	Create a filename for the results out of the name of this configuration file.
 */
	if(results_file[0] == '\0')
		Make_Filename(results_file, file_name, ".sta");
	Make_Filename(summary_file, results_file, ".sum");
	 
/*
 *	Check consistency of specified configuration and fill in the gaps if possible.
 *	Check: num_fields = input_fields + output_fields
 *	If window then
 *		check: nodes in input layer = window * input_fields.
 *	Else
 *		check: nodes in input layer = input_fields AND 
 *		nodes in output layer = output_fields.
 *
 *	Set ignore test and train points to 0 if they are unspecified.
 *	If num_train_points or num_test_points are not specified then pre-count through
 *	  the corresponding file(s). 
 */
    if(window)
    	num_fields = input_fields;
	else if (!Check_Triple(&num_fields, &input_fields, &output_fields))
		Error_Exit("\nSpecified values of total, input and output fields contradict.",  "EOF",
item, file_name);

/*
	if ((window > 0) && (output_fields > 0))
		Error_Exit("\nCannot have output fields in windowed data.",  "EOF",
item, file_name);
	else if(window > 0)
		output_fields = 0;
	else if (output_fields > 0)  
		window = 0;
	else
		Error_Exit("\nNeed to specifiy either output fields or window size.",  "EOF",
item, file_name);
*/
	if(ignore_train_points < 0)
		ignore_train_points = 0;
	if((num_train_points < 0) && train_file[0])
		num_train_points = Count_File_Data(train_file, num_fields) - ignore_train_points;
	if(num_train_points < 0)
		num_train_points = 0;

	if(ignore_test_points < 0)
		ignore_test_points = 0;
	if((num_test_points < 0) && (!test_data_feed) && test_file[0])
		num_test_points = Count_File_Data(test_file, num_fields) - ignore_test_points;
	if(num_test_points < 0)
		num_test_points = 0;

/*
 *	Construct the data-format structures that will contain the (windowed) test and 
 *	  train data for the neural net.   
 *	If the data is a windowed time-series then calculate the number of records.
 *	Otherwise, use the given number of points.
 */      
	num_records = (window ? (num_train_points-window-prediction_delay+1)/pattern_delay :
num_train_points);
	*train_arr = Con_DataFormat(window, prediction_delay, pattern_delay, 
(window ? window*input_fields : input_fields), output_fields, num_records);  

	num_records = (window ? (num_test_points-window-prediction_delay+1)/pattern_delay :
num_test_points);
	*test_arr = Con_DataFormat(window, prediction_delay, pattern_delay, 
(window ? window*input_fields : input_fields), output_fields, num_records);  

/*
 *	Construct the data-pairs structures.
 */
	*train_data = Con_DataPairs(num_train_points, input_fields, output_fields, num_fields, 
ignore_train_points, train_file, FALSE);
	Get_File_Data(train_file, *train_data, ignore_train_points);   
	
	*train_price_data = Con_DataPairs(num_train_points, 1, 0, 1,
ignore_train_points, train_price_file, FALSE);
	Get_File_Data(train_price_file, *train_price_data, ignore_train_points);

	*test_data 	= Con_DataPairs(num_test_points, input_fields, output_fields, num_fields,
ignore_test_points, test_file, test_data_feed);
	Get_File_Data(test_file, *test_data, ignore_test_points); 
	
	*test_price_data = Con_DataPairs(num_test_points, 1, 0, 1,
ignore_test_points, test_price_file, test_data_feed);
	Get_File_Data(test_price_file, *test_price_data, ignore_test_points);

/*
 *	Open and close the results file to clear it.
 */
	fp = Open_File(results_file, "w", "Get_Configuration()");
	fclose(fp); 
	fp = Open_File(summary_file, "w", "Get_Configuration()");
	fclose(fp); 
                              

} /* End of Get_Configuration() */
	


Void Error_Exit(char *mesg, char *new_word, Int item, char *file_name)
{
    fprintf(stderr, "\n+++ Error from file '%s' at item %ld - symbol: '%s' - %s", file_name, item, new_word,mesg);
	exit(1);
} /* End of Void Error_Exit() */
                             
                             


Int Scan_String(FILE *fp, char *new_word, char *file_name, Int item)
/*
 *	Obtain the next string item from the file-pointer 'fp',
 *	Exit on error.
 */
{
 	Int i, end_found;
 	
	item++;
	if(fscanf(fp,"%s ",new_word) != 1)
	{
		if(!feof(fp)) 
		{
    		fprintf(stderr, "\n+++ Error reading string from file '%s' at item %d", file_name, item);
    		exit(1);  
    	}
    } 

/*
 *	Strip leading and trailing quotes if any. 
 */
 	if(new_word[0] == '\"')
 	{
		end_found = 0;
 		for(i=0; new_word[i]; i++) 
 		{
 			if(new_word[i+1] == '\"') 
 			{
 				end_found = 1;
 				new_word[i] = new_word[i+2];    
 			}
 			else 
 				new_word[i] = new_word[i+1];
 		}
 		if(!end_found)
 			Error_Exit("\nDid not find matching quotes.",new_word,item , file_name);
 	}
	return(item);
} /* End of Int Scan_String() */
    
    
    
Int Scan_Int(FILE *fp, Int *value, char *file_name, Int item)
/*
 *	Obtain the next integer item from the file-pointer 'fp',
 *	Exit on error.
 */
{
	item++;
	if(fscanf(fp,"%ld ",value) != 1)
	{
    	fprintf(stderr, "\n+++ Error reading integer from file '%s' at item %d", file_name, item);
    	exit(1);
    }
	return(item);
} /* End of Int Scan_Int() */


				 
Int Scan_Float(FILE *fp, Double *value, char *file_name, Int item)
/*
 *	Obtain the next floating point item from the file-pointer 'fp',
 *	Exit on error.
 */
{
	item++;
	if(fscanf(fp,"%lf ",value) != 1)
	{
    	fprintf(stderr, "\n+++ Error reading float from file '%s' at item %d", file_name, item);
    	exit(1);
    }
	return(item);
} /* End of Int Scan_Float() */

   
   
Void Check_Points(Int *train, Int *test, Int *data, Int *ignore, 
		Int *lead_in, Int item, char *file_name)
{

	if (!Check_Triple(ignore, lead_in, train))
		Error_Exit("\nSpecified values of ignore, lead-in and test points contradict.",
"EOF", item, file_name);
	if(*lead_in == -1)
	{
		*lead_in = 0;
		if(*ignore == -1)
			*ignore = *train;
		else if(*train == -1)
			*train = *ignore;
	}
	if (!Check_Triple(data, test, ignore))
		Error_Exit("\nSpecified values of total, test and ignore points contradict.","EOF",
item, file_name);
	if (!Check_Triple(ignore, lead_in, train))  
		Error_Exit("\nSpecified values of ignore, lead-in and test points contradict.","EOF",
item, file_name);

} /* End of Check_Points() */



Int Check_Triple(Int *total, Int *a, Int *b)
/*
 *	Check that '*a' and '*b' add up to '*total'
 *	If '*a' or '*b' or '*total' are set to -1 it indicates that they
 *	  have not been specified in the configuration file.
 *	Try to set a value for unspecified arguments if the other two
 *	  are specified.
 */ 
{ 

	if(*a != -1)
	{
		if(*b != -1)
		{
			if(*total == -1)
			{
		    	*total = *a + *b;
		    	return(1);		    
			}
		    else
		    {
		    	if(*total != *a + *b)
		    		return(0);
		    	else
		    		return(1);		    
		    }
		} 
		else if (*total != -1)
		{
			*b = *total - *a;
			return(1);
		}
	}
	else if((*b != -1) && (*total != -1))
	{
    	*a = *total - *b;
    	return(1);		    
	}

	return(-1);	
			

} /* End of Check_Fields() */


 

