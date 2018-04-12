/*			statutil.c			UBS			20/4/94			*/

/*
 *	Set-up utilities used by the GenNet project.
 *
 *
 *
 *		Double	Cum_Norm_Dist(Double z);
 *		Double	Confidence(Double z, Int tail); 
 *
 *
 */
 

#include 	"header.h" 



Double Cum_Norm_Dist(Double z)    
/* 
 *	Compute the cumulative normal distribution for z 
 * 	Probability of a number greater than z is Nz = 1 - Nz;  
 */
{ 
	Double C1, C2, C3, C4, C5, C6, w, y, x, u, v;
	
	if (z < -6.0)  
		z = -6.0;
	if (z > 6.0)  
		z = 6.0;
	if (z >= 0.0)  
		w = 1.0;
	else
		w = -1.0;    
		
	y = (1.0 / 1.0 + 0.2316419 * w * z);
	C1 = 2.506628;
	C2 = 0.3193815;
	C3 = -0.3565638;
	C4 = 1.7814779;
	C5 = -1.821256;
	C6 = 1.3302744;
	x = (y * (C2 + y * (C3 + y * (C4 + y * (C5 + y * C6)))));
	v = (-z * z / 2.0);
	u = (0.5 - (exp(v) / C1));  
	
	return(0.5 + w * u * x);
} /* End of Double Cum_Norm_Dist() */



Double Confidence (Double z, Int tail)
{ 
	Double x, y, k, v, w;
	
	x = 1.0 / (ABS(z) * 0.2316419 + 1.0);
	y = x * x;
	k = ((x * 0.31938153) - (y * 0.356563782) + (x * y * 1.781477937) 
- (y * y * 1.821255978) + (y * y * x * 1.330274429)); 
	if(ABS(z) > 20.0)
	{
		w = 100000000000.0;   
		v = 0.0;
	}
	else
	{
		v = (exp(z * z) * 6.283185307); 
		w = 1.0 - 2.0 * k * (1.0 / sqrt(v));  
	}
	
	if (tail == 1)  
		return(1.0 - (1.0 - w) / 2.0);
	else
		return(w);
} /* End of Double Confidence() */



