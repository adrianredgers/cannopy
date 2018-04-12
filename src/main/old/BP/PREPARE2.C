/*			prepare.c			UBS			4/5/94				*/
/*
 *	Data preparation functions.
 */
 
 
#include	"header.h"



DataFormatPtr Con_DataFormat(Int window, Int prediction_delay, Int pattern_delay,
Int in_cols, Int out_cols, Int num_records)
/*
 *	Create a structure for holding windowed data.
 */
{
	DataFormatPtr data_format;
	Int old_mem, new_mem;
	old_mem = Get_Mem_Total(FALSE);
	
	
	data_format = MAKE_A(DataFormat);
	
	data_format->window = window;
	data_format->prediction_delay = prediction_delay;
	data_format->pattern_delay = pattern_delay;
	data_format->in_cols = in_cols;
	data_format->out_cols = out_cols;
	data_format->num_records = num_records;

	if(in_cols > 0)	
		data_format->inputs = MAKE_SOME(num_records*in_cols, Double);
	if(out_cols > 0)	
 		data_format->outputs = MAKE_SOME(num_records*out_cols, Double);

	new_mem = Get_Mem_Total(FALSE);
/*	fprintf(stderr,"\nMemory allocated by Con_DataFormat() = %ld bytes",new_mem-old_mem); */	

	return(data_format);    
	
} /* End of DataFormatPtr Con_DataFormat() */



VoidPtr Des_DataFormat(DataFormatPtr data_format)
/*
 *	Destructor for a DataFormat structure.
 */
{
	Int old_mem, new_mem;
	old_mem = Get_Mem_Total(FALSE);
	
	if(data_format->in_cols > 0)	
		My_Free(data_format->inputs, data_format->num_records*(data_format->in_cols), 
sizeof(Double));
	if(data_format->out_cols > 0)	
		My_Free(data_format->outputs, data_format->num_records*(data_format->out_cols), 
sizeof(Double));

	My_Free(data_format, 1 , sizeof(DataFormat)); 

	new_mem = Get_Mem_Total(FALSE);
/*	fprintf(stderr,"\nMemory freed by Des_DataFormat() = %ld bytes",old_mem-new_mem);	*/

	return(NULL);

} /* End of DataFormatPtr Des_DataFormat() */



DataPairsPtr Con_DataPairs(Int num_points, Int num_inputs, Int num_outputs, 
	Int num_fields, Int ignore_points,
	char *file_name, Bool data_feed)
/*	
 *	Constructor for a DataPairs structure.
 *	Allocate space for and set up a DataPairs structure.
 */
{
	Int i;
	DataPairsPtr data; 
	Int old_mem, new_mem;
	old_mem = Get_Mem_Total(FALSE);
	
	data = MAKE_A(DataPairs);   

	data->num_records = num_points;
	data->num_inputs = num_inputs;
	data->num_outputs = num_outputs; 
	data->num_fields = num_fields;
    data->data_feed = data_feed;         
    data->ignore_recs = ignore_points;
    for(i=0; file_name[i]; i++)
    	data->file_name[i] = file_name[i];
    data->file_name[i] = file_name[i];
    
	if(num_points > 0)
	{
		data->mean 		= (DoublePtr) My_Malloc(data->num_fields, sizeof(Double));
		data->std_dev 	= (DoublePtr) My_Malloc(data->num_fields, sizeof(Double));
		data->max_lim 	= (DoublePtr) My_Malloc(data->num_fields, sizeof(Double));
		data->min_lim 	= (DoublePtr) My_Malloc(data->num_fields, sizeof(Double));
	
		data->values = 	MAKE_SOME(data->num_fields, DoublePtr);		   
		for(i=0;i<data->num_fields; i++)
			data->values[i] = MAKE_SOME(num_points, Double);
			
	}

	new_mem = Get_Mem_Total(FALSE);
/*	fprintf(stderr,"\nMemory allocated by Con_DataPairs() = %ld bytes",new_mem-old_mem); */	
	
	return(data);
} /* End of DataPairsPtr Con_DataPairs() */



VoidPtr Des_DataPairs(DataPairsPtr data)
/*	
 *	Destructor for a DataPairs structure.
 *	Free items in the inverse order to which they were allocated (approximately).
 */
{
	Int i;
	Int old_mem, new_mem;
	old_mem = Get_Mem_Total(FALSE);   
	
	if(data->num_records > 0)
	{	
		My_Free(data->min_lim, data->num_fields, sizeof(Double));
		My_Free(data->max_lim, data->num_fields, sizeof(Double));
		My_Free(data->std_dev, data->num_fields, sizeof(Double));
		My_Free(data->mean, data->num_fields, sizeof(Double));
		for(i=0;i<data->num_fields; i++)
			My_Free(data->values[i], data->num_records, sizeof(Double));
		My_Free(data->values, data->num_fields, sizeof(DoublePtr));
	}	
	My_Free(data, 1 , sizeof(DataPairs));   
	 
	new_mem = Get_Mem_Total(FALSE);
/*	fprintf(stderr,"\nMemory freed by Des_DataPairs() = %ld bytes",old_mem-new_mem); */	
	return(NULL);

} /* End of Void Des_DataPairs() */ 


        
Void Prepare_Data(DataPairsPtr data, DataFormatPtr df)
/*                    
 *	Prepare (windowed) data in data_format using the column items in 'data'. 
 *
 * 	If data_format->window is positive the data is traeted as a 
 *	  windowed time-series.
 *	In this case there are no output fields in the data file, instead the 
 *	  output is taken to be the value(s) of the (first) data field(s) for 
 *	  the predicted time point.
 *
 * +++ NOTE +++ this routine is affected by the form in which the data appears.
 */
{
	Int inpos, outpos, slice, point, col;
	

	if(df->window > 0)
	{
/*
 *	Copy the data-points into the tapped delay-line (windowed time-series).
 *	Set the fields of the predicted data-point to be the desired output of the neural net.
 */

		for(inpos=0, outpos=0, point = (df->window - 1); 
point < (data->num_records - df->prediction_delay); point += df->pattern_delay)
		{
			for(slice = (point - df->window + 1); slice<= point; slice++)
				for(col=0; col < data->num_inputs; col++, inpos++)
					df->inputs[inpos] = data->values[col][slice]; 
					
			for(col = 0; col< df->out_cols; col++, outpos++)
				df->outputs[outpos] = data->values[col][point + df->prediction_delay];       
		}
	}
	else
	{ 
/*	
 *	Copy the first 'data->num_inputs' fields into the input layer of the net,
 *	set the next 'data->num_outputs' fields to be the desired output of the net.
 */ 
		for(inpos=0, outpos=0, point=0; point<data->num_records; point++) 
		{
			for(col = 0; col < data->num_inputs; col++, inpos++)
				df->inputs[inpos] = data->values[col][point];
			for(col=data->num_inputs; col< data->num_fields; col++, outpos++)
				df->outputs[outpos] = data->values[col][point];
		}
	}		
} /* End of Void Prepare_Data() */






        
Void Prepare_Prices(DataPairsPtr data, DataFormatPtr df)
/*                    
 *	Prepare prices in 'data' to go in df->outputs. 
 *
 * +++ NOTE +++ this routine is affected by the form in which the data appears.
 */
{
	Int outpos, point;
/*
 *	Copy the data-points into the tapped delay-line (windowed time-series).
 *	Move in steps of pattern-delay (which may be more than 1).
 */
		for(point = (df->window - 1), outpos = 0; 
point < (data->num_records - df->prediction_delay); 
point += df->pattern_delay, outpos++)
			df->outputs[outpos] = data->values[0][point]; 
} /* End of Void Prepare_Prices() */

