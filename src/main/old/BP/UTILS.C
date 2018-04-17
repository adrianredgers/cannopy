/*		utils.c		UBS			30/3/94				*/
/*
 *	General utilities routines.   
 *	
 *		Copy_Ints()  
 *		Get_File_Data()
 */
 
 
#include "header.h"

 
Void Make_Filename(char *new_file, char *prefix_file, char *dot_suffix)
{
	Int i, j;
				
 	i = 0;
 	while((prefix_file[i] != '\0') && (prefix_file[i] != '.'))
 	{
 		new_file[i] = prefix_file[i];
 		i++;
 	}  
	j = 0;
	while(dot_suffix[j] != '\0') 	
 	{
 		new_file[i] = dot_suffix[j];
 		i++;
 		j++;
 	}
 	new_file[i] = '\0'; 
	 	
} /* End of Void Make_Filename() */



Void Acknowledge(char *mesg)
{      
	char ch;
	fprintf(stderr,"\n%s : press <return> to continue. ", mesg);
	fscanf(stdin, "%c", &ch);     
} /* End of Void Acknowledge() */


  
Void Copy_Ints(Int from_arr[], Int to_arr[], Int num_elts)
{
	while(num_elts>0)
	{
		num_elts--;
		to_arr[num_elts] = from_arr[num_elts];
	}
} /* End of Void Copy_Ints() */


  
/* ----------------------------------------------------------------------- */


Void Get_File_Data(char *file_name, DataPairsPtr data, Int ignore_points)
/*      
 *	Get data points from file: file_name. 
 *	Allocate an array space for the data points and place them in the array.
 *	Return a pointer to the array, return statistics about the data points as
 *	  VAR arguments.
 */ 
{
	Int 		rec, field; 
	FILE		*file_in;
	Int			good_scan;
	float		float_value;

/*
 *	Catch a zero number of records to be read.
 */
	if(data->num_records == 0)
		return;
		
	fprintf(stderr,"\n\nObtaining data from file '%s'",file_name);
	if(ignore_points > 0)
	{
		fprintf(stderr,"\nIgnoring the first %ld data points in the file.\nReading in the following %ld data points",
ignore_points, data->num_records); 
	}
	else if (ignore_points < 0)   
	{
		fprintf(stderr,"\n+++ ERROR +++ Get_File_Data() :  could not ignore %ld data-points,\n",
ignore_points);
		exit(1);
	}
	
	file_in = fopen(file_name, "r");
	if(file_in == NULL)
	{
		fprintf(stderr,"\n+++ ERROR +++ Get_File_Data() : could not open file '%s'", file_name);
		exit(1);
	}

/*
 *	Step past the points to be ignored.
 */	
	for(rec=0; rec<ignore_points; rec++)
	{
		for(field=0; field<data->num_fields; field++)
		{
			good_scan = fscanf(file_in,"%f ",&float_value);
			if(good_scan != 1)
			{
				fprintf(stderr,"\n+++ ERROR +++ Get_File_Data() : error reading record %ld, field %ld of file %s",
	rec+1, field+1, file_name);
				exit(1);  
			}
		} /* Next field */
	} /* Next record */

/*
 *	Read in the first data-record that is not to be ignored.  
 *	Initialize values of mean, standard deviation, and limits for each field.
 */
	for(field=0; field< data->num_fields; field++)
	{
		good_scan = fscanf(file_in,"%f ",&float_value);
		if(good_scan != 1)
		{
			fprintf(stderr,"\n+++ ERROR +++ Get_File_Data() : error reading record %ld, field %ld of file %s",
ignore_points+1, field+1, file_name);
			exit(1);  
		}  
		data->values[field][0] 	= float_value;
		data->mean[field] 		= float_value;
		data->std_dev[field] 	= float_value*float_value;
		data->max_lim[field] 	= float_value;
		data->min_lim[field] 	= float_value;
		
	} /* Next field */

/*
 *	Get the remaining records. 
 *	Increment the means and standard deviations and update the
 *	  limits if necessary.
 */	
	for(rec=1; rec< data->num_records; rec++)
	{
		for(field=0; field < data->num_fields; field++)
		{
			good_scan = fscanf(file_in,"%f ",&float_value);
			if(good_scan != 1)
			{
				fprintf(stderr,"\n+++ ERROR +++ Get_File_Data() : Error reading record %ld, field %ld of file %s",
(rec+ignore_points+1), field+1, file_name);
				exit(1);  
			}
	
		    data->values[field][rec] = float_value;
		 	data->mean[field] += float_value;
		 	data->std_dev[field] += float_value*float_value;
			if(float_value < data->min_lim[field])
				data->min_lim[field] = float_value;
			if(float_value > data->max_lim[field])
				data->max_lim[field] = float_value;	
		} /* Next field */ 
	} /* Next data rec */    
	 
	fclose(file_in);

/*
 *	Complete the calculations of the means and standard deviations.
 */
	for(field=0; field<data->num_fields; field++)
	{
		data->mean[field] /= 1.0 * (data->num_records);
		data->std_dev[field] = (data->num_records/(data->num_records-1.0))*
(data->std_dev[field]/data->num_records - data->mean[field]*(data->mean[field]));
/*
		printf("\nStats for field %ld : mean=%lf, s.d^2=%lf, minimum=%lf, maximum=%lf",
(field+1), data->mean[field], data->std_dev[field], data->min_lim[field], data->max_lim[field]);
*/
	} 

} /* End of Void Get_File_Data() */

   
/* ------------------------------------------------------------------------ */
   

Int Count_File_Data(char *file_name, Int num_fields)
{
	Int num_points = 0, j, good_scan;
	FILE *fp; 
	char dummy[100];
	
	fprintf(stderr,"\nCounting records in file '%s'",file_name);
	
	fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		fprintf(stderr,"\n+++ ERROR +++ Count_File_Data() : could not open file '%s'", file_name);
		exit(1);
	}

	while(!feof(fp))
	{
		for(j=0;j<num_fields;j++) 
		{
			good_scan = fscanf(fp,"%s ",dummy);
			if(good_scan != 1)
			{
				fprintf(stderr, "\n+++ ERROR +++ Count_File_Data() : error scanning a field before EOF in file %s\n",
file_name);
				exit(1);
			}
		}
		num_points++;			
	}         
/*
	fprintf(stderr,"\nCounted %ld records (each with %ld fields) in file '%s'",
num_points, num_fields,file_name);
*/	
	return(num_points);

} /* End of Count_File_Data() */

   
/* ------------------------------------------------------------------------ */
   

Double	Example_PDF(Double x)        
/*
 *	Example probability distribution function.
 *		Integral over range [0.0, 10.0] = 1
 *		Function is positive over this range.
 */
{
	Double y;
	y = x*(x-2)*(x-3)*(x-7)*(x-8)*(x-10);
	y = y*y/2.9507E6;
	return(y);
} /* End of double Example_PDF() */


/* ----------------------------------------------------------------------------- */
      
      
Double	Prob_Distrib(Double min_limit, Double max_limit, Double pdf(Double x))
/*
 *	Return value in range [min_limit, max_limit] with probability 
 *	  distribution function pdf(). 
 */
{
	Double x;
	
	do
	{
		x = (max_limit-min_limit)*((Double) rand())/RAND_MAX;	
	} while (pdf(x) < ((Double) rand())/RAND_MAX);
	
	return(x);

} /* End of Double Prob_Distrib() */


/* ----------------------------------------------------------------------------- */


FILE *Open_File(char *file_name, char *mode, char *err_mesg) 
/*
 *	Attempt to open a file.
 *	Error-exit if this is not possible.
 */
{              
	FILE *fp;
	
	fprintf(stderr,"\nOpening file '%s' for ",file_name);	
	if((toupper(mode[0]) == 'R')  || (toupper(mode[1]) == 'R'))
		fprintf(stderr,"reading.");
	else if((toupper(mode[0]) == 'W') || (toupper(mode[1]) == 'W'))
		fprintf(stderr,"writing.");
	else if((toupper(mode[0]) == 'A') || (toupper(mode[1]) == 'A'))
		fprintf(stderr,"appending.");

	fp = fopen(file_name, mode);
	if(fp == NULL)
	{
		fprintf(stderr, "\n\n+++ ERROR +++ %s : could not open file '%s' for ",
err_mesg,file_name);
		if((toupper(mode[0]) == 'R')  || (toupper(mode[1]) == 'R'))
			fprintf(stderr,"reading.\n");
		else if((toupper(mode[0]) == 'W') || (toupper(mode[1]) == 'W'))
			fprintf(stderr,"writing.\n");
		else if((toupper(mode[0]) == 'A') || (toupper(mode[1]) == 'A'))
			fprintf(stderr,"appending.\n");
		exit(1); 
	}

	return(fp);
} /* End of FILE *Open_File() */


