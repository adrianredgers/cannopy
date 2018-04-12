/*		allocate.c		UBS		5/4/94				*/

#include 	"header.h"

/*
 *	Global for memory management.
 */
Int MEM_TOTAL = 0;   


VoidPtr My_Malloc(Int num_items, int item_size)
{
	VoidPtr ptr = NULL;

	if((num_items > 0) && (item_size>0))
	{		
#ifdef MM_HUGE
		ptr = _halloc(num_items, item_size);
#else
		ptr = malloc((size_t) (num_items*item_size));
#endif
	

		if(ptr == NULL)
		{
			fprintf(stderr, "\n\n+++ My_Malloc() +++ Not enough memory available for %ld items of size %d",
num_items, item_size);
			exit(1); 
		}
	}       
	else
	{
		fprintf(stderr, "\n\n+++ My_Malloc() +++ Cannot allocate %ld items of size %d",
num_items, item_size);
		exit(1);
	}
	MEM_TOTAL += num_items*item_size;	
	return(ptr);	
} /* End of VoidPtr My_Malloc() */        

 
 

VoidPtr My_Free(VoidPtr ptr, Int num_items, int item_size)
{
#ifdef MM_HUGE
	_hfree(ptr);
#else
	free(ptr);
#endif 

	MEM_TOTAL -= num_items*item_size;
	
 	return(NULL);
} /* End of VoidPtr My_Free() */
        
        
Int Get_Mem_Total(Bool print_it)
{
	if(print_it)
		fprintf(stderr,"\nTotal memory currently allocated = %ld bytes",MEM_TOTAL);

	return(MEM_TOTAL);
}    


