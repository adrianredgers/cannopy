/*			calcstat.c			UBS			20/4/94			*/

/*
 *	Population statistics calculation functions used by the GenNet project.
 */
 

#include 	"g_type.h" 
#include	"proto.h"
#include	"globals.h"


Void Prescale (Double umax, Double uavg, Double umin, 
		Double *a, Double *b)
/* 
 *	Calculate scaling coefficients for linear scaling 
 */ 
{
	Int fmultiple=2;
	Double delta;
	 
	if (umax > (fmultiple * uavg - umax) / (fmultiple - 1.0))       /* non negative test */ 
	{ 
		delta = umax - uavg;
		*a = (fmultiple - 1.0) * uavg / delta;
		*b = uavg * (umax - fmultiple * uavg) / delta;
	} 
	else
	{ 
		delta = uavg - umin;
		*a = uavg / delta;
		*b = -umin * uavg / delta;
	} 
} /* End of Void Prescale() */ 



Double Linear_Scale (Double u, Double a, Double b)
/* 
*	Scale the objective function 
*/ 
{ 
return(a * u + b);
}
      
      
Void Scale_Pop (Population pop, Int pop_size, Double *max, 
		Double *avg, Double *min, Double *sum_fitness)
/* 
 *Scale the entire Population. 
 */ 
{
	Int j;
	Double a, b;
	 
	Prescale(*max, *avg, *min, &a, &b);
	*sum_fitness = 0.0;
	for(j = 1; j<= pop_size; j++)
	{ 
		pop[j].fitness = Linear_Scale(pop[j].fitness, a, b);
		*sum_fitness = *sum_fitness + pop[j].fitness;
	} 
} /* End of Double Linear_Scale() */  


Void Statistics (Population pop, FitnessRec *fitness)
{
	Int j;
 
	fitness->max = pop[1].fitness;
	fitness->min = pop[1].fitness;
	fitness->sum = pop[1].fitness;
	for(j = 2; j<=POP_SIZE; j++)
	{    /* determine total fitness */ 
		fitness->sum +=  pop[j].fitness;  /* determine max fitness */ 
		if (pop[j].fitness  >=  fitness->max)  
			fitness->max = pop[j].fitness;
		if (pop[j].fitness  <=  fitness->min)  
			fitness->min = pop[j].fitness;
	} 
	/* determine AVE_FITNESS fitness */ 
	fitness->ave = fitness->sum / POP_SIZE;
	/* Scale_Pop(pop, POP_SIZE, max, AVE_FITNESS, MIN_FITNESS, *sum_fitness); */ 
	/* -->>>>this has been added for linear scaling see p. 79 Goldberg */ 

} /* End of Void Statistics() */



      

 
