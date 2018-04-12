/*			rnd_sort.c			UBS			20/4/94			*/

/*
 *	Random number generation and sort utilities used by the GenNet project.
 *	
 *	The utilities are:
 *		
 *		Double 	Random_Seed();
 *		Void	Advance_Random();
 *		Void	Start_Random(Double seed);
 *		Double	Random();
 *		Int		Rnd(Int low, Int high);
 *		Bool	Flip(Double probability);
 *		Double	Power(Double base, Double index);
 *		Double	Radial(Double x, Double	mu, Double sigma);
 *
 *
 */
 
 
/*
	do
	{
		x = min_limit+(max_limit-min_limit)*((Double) rand())/RAND_MAX;	
	} while (pdf(x) < ((Double) rand())/RAND_MAX);
*/
 

#include 	"g_type.h" 
#include	"proto.h"



Double Random_Double(Double lo, Double hi)
/* 
 *	Returns a random number between lo and hi 
 */ 
{ 
	Double x;    
	
	x = ((Double) rand())/RAND_MAX; 
	return(x*(hi-lo)+lo);
	
} /* End of Double Random_Double() */



Bool Flip(Double probability)
/* 
 *	Flips a biased coin.
 */ 
{ 
	if((((Double) rand())/RAND_MAX) <= probability)
		return(TRUE);
	else
		return(FALSE);
} /* End of Bool Flip() */

  

Int Random_Int(Int low, Int high)
/* 
 *	Picks a Random() number within a range.
 */ 
{ 
	Int x;
	
	x = low + ((rand())%(high-low+1)); 
	return(x);

/*	return((Int) (Random()*(high+1-low) + low)); */  
} /* End of Int Random_Int() */ 


 
Int Random_UInt(UInt low, UInt high)
/* 
 *	Picks a Random() long unsigned integer within a range.
 */ 
{ 
	UInt x;
	
	x = low + (((UInt) (rand()))%(high-low+1)); 
	return(x);

/*	return((Int) (Random()*(high+1-low) + low)); */  
} /* End of UInt Random_UInt() */ 
 
Int Random_Bit(void)
/* 
 *	Returns a random bit: 1 or 0.
 */ 
{ 
	return((rand())%2);

} /* End of Int Random_Bit() */ 


Void My_Swap(IntPtr ordering, Int i, Int j)
{
	Int tmp;
	tmp = ordering[i];
	ordering[i] = ordering[j];
	ordering[j] = tmp;
} /* End of Void My_Swap() */


Void My_Sort(Int left, Int right, PopulationPtr pop)
/* 
 *	Recursively Quicksorts a Population of strings by increasing fitness.  
 *	Indexes to organisms are held in an array to save copying loads of bytes. 
 */ 
{ 
	Double val;           /* used to divide array */ 
	Int last, i;     		/* positions to be flipped */ 
	Void My_Swap(IntPtr ordering, Int a, Int b);

	if(left >= right)
		return;

	My_Swap(pop->ordering, left, (left+right)/2);		   
	last = left; 
	val = pop->fitness[pop->ordering[left]];
	
	for(i= left+1; i<=right; i++)
	{
		if(pop->fitness[pop->ordering[i]] > val)
		{
			last++;
			My_Swap(pop->ordering, last, i);		
		}	
	}
	My_Swap(pop->ordering, left, last);		

	My_Sort(left, last-1, pop);
	My_Sort(last+1, right, pop);   
   
} /* End of Void My_Sort() */ 



Void Copy_Chrom(PopulationPtr to_pop, Int to_organism, PopulationPtr from_pop, 
Int from_organism)
{

	Copy_Bytes(to_pop->chrom[to_organism], from_pop->chrom[from_organism], 
sizeof(Double)* to_pop->chrom_len);

	to_pop->rmse[to_organism] 		= from_pop->rmse[from_organism];
	to_pop->fitness[to_organism] 	= from_pop->fitness[from_organism];
	to_pop->changed[to_organism] 	= from_pop->changed[from_organism];
	to_pop->parent1[to_organism] 	= from_pop->parent1[from_organism];
	to_pop->parent2[to_organism] 	= from_pop->parent2[from_organism];
	to_pop->x_site[to_organism] 	= from_pop->x_site[from_organism];

} /* End of Void Copy_Chrom() */



Void Copy_Bytes(VoidPtr to, VoidPtr from, Int num_bytes)
{
	for(num_bytes--;num_bytes>=0;num_bytes--)
		((char *) (to)) [num_bytes] = ((char *) (from)) [num_bytes];
} /* End of Void Copy_Bytes() */
 


Void Random_Chrom(PopulationPtr pop, Int organism, GAParamsPtr params)
{
	Int gene = params->bits_per_weight;   /* <- stops a warning from the compiler. */
		
	for(gene=0; gene < pop->chrom_len; gene++)
		pop->chrom[organism][gene] = ((UInt) rand()) + ((UInt) rand())*32767UL;

	pop->changed[organism] = TRUE;
	
} /* End of Void Random_Chrom() */


 
