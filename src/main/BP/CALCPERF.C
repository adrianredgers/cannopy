/*			calcperf.c			UBS			20/4/94			*/

/*
 *	Fitness and performance calculation utilities used by the GenNet project.
 *
 *	Trading Strategy:
 *  	The trading strategy makes or loses money based upon actual trades - i.e
 *		assuming a trading position and profit-taking from it, not just predicting
 *		whether price is going up or down.
 *
 * 		The initial market position and BUY/SELL signal is calculated by Init_Trading()
 *		based upon the initial prediction of the neural network.
 *
 *		For each subsequent time-step, Buy_Sell() takes the price movement predicted
 *		by the neural network and the current market position and uses them to
 *		generate a BUY/SELL/NONE signal, and a new market position LONG/SHORT/FLAT.
 *
 *		Gains_Losses() takes the newly generated and previous BUY/SELL signals and the
 *		current and previous market positions and calculates how much profit or loss 
 *		was made by this trade. 
 *
 *		Very often there is no gain or loss as the system holds to a current market 
 *		position - gains or losses will only be made upon change of a market position. 
 *		   
 *
 */
 

#include 	"header.h"


#define	TRADING_DAYS_IN_YEAR   	250



 
Void Buy_Sell_Signal(Double price, ResultsRecPtr response, Double network_output_val) 
/*
 *	Calculate the buy-sell repsonses and market positions for the various data points.
 */
{ 

/*  
 *	Determine the buy/sell signal for each trading point. 
 */ 
	response->value = network_output_val; 
	response->predict_price = price*exp(network_output_val);    

	if (response->predict_price > (price + response->no_pos_zone))  
	{ 
		response->curr_sig = 1;
	} 
	else if (response->predict_price < (price - response->no_pos_zone))  
	{ 
		response->curr_sig = -1;
	} 
	else 
	{ 
		response->curr_sig = 0;
	}


/*
 *	Calculate the new market position based upon the current position and the buy/sell signal.
 *	Copy current position to previous, and initially assume the position stays unchanged.
 */
	response->prev_position = response->position;
	response->pos_changed = FALSE;

/* 
 *	If currently LONG, and signal is SELL or STAY-OUT then new position is FLAT
 */ 
	if ((response->position == P_LONG) && (response->curr_sig != 1))              
	{ 
		response->position = P_FLAT;
		response->pos_changed = TRUE;
	}
/* 
 *	If currently SHORT, and signal is BUY or STAY-OUT then new position is FLAT
 */ 
	else if ((response->position == P_SHORT) && (response->curr_sig != -1))
	{ 
		response->position = P_FLAT;
		response->pos_changed = TRUE;      
	}
/* 
 *	If currently FLAT, and signal is BUY or SELL then new position is LONG or SHORT
 */ 
	else if (response->position == P_FLAT)  
	{
		if(response->curr_sig == 1)
		{ 
			response->position = P_LONG;
			response->pos_changed = TRUE;
		} 
		else if(response->curr_sig == -1)
		{ 
			response->position = P_SHORT;
			response->pos_changed = TRUE;
		} 
	} 
	
}   /*  End  of Void Buy_Sell_Signal()  */ 



Void Init_Trading(Double price, PerformRecPtr perform, ResultsRecPtr response,
Double network_output, Double init_capital, Double no_pos_zone)
/* 
 *	initialize all variables for multiple runs.  
 *	Given current price and ndr we can obtain true and predicted prev. price.
 */ 
{
	perform->total_trades = 0;
	perform->prof_trades = 0;
	perform->total_gains = 0.0;
	perform->total_losses = 0.0;
	perform->max_gain = 0.0;
	perform->max_loss = 0.0;
	perform->max_dd_val = 0.0;
	perform->max_num_dds = 0;
	perform->avg_pl = 0.0;
	perform->stdev_pl = 0.0;    
	perform->p_and_l = 0.0;
	perform->cum_pl_pc = 0.0;
/*  
 *	Determine the starting position for the first struct. 
 */ 
	response->value = network_output;
	response->no_pos_zone = no_pos_zone;
	response->prev_price = price;
	response->predict_price = price * exp(network_output);
	response->pos_changed = FALSE; 
	response->position = P_FLAT;
	response->prev_position = P_FLAT;
	response->pl_pc = 0.0;
	
	response->equity = init_capital;           /* set the starting equity = init_capital */ 
	response->num_drawdowns = 0;
	response->drawdown = 0.0;
	response->hit = UNKNOWN;   
	response->prev_sig = 0;
		
/* 
 *	for the first row establish the signal 
 */ 
	if (response->predict_price > (price + response->no_pos_zone))  
	{ 
		response->curr_sig = 1;
		response->position = P_LONG;
	} 
	else if (response->predict_price < (price - response->no_pos_zone))  
	{ 
		response->curr_sig = -1;
		response->position = P_SHORT;
	} 
	else 
	{ 
		response->curr_sig = 0;
		response->position = P_FLAT;
	}


} /* End of Void Init_Trading() */

 

Void Gains_Losses(Double price, PerformRecPtr perform, ResultsRecPtr response, 
Double transaction_size, Double transaction_cost)
{ 
	Double gain, loss;
	
/* 
 *	Start by assuming no change in position.  
 *	And return() if the position really has not changed.
 */ 
	perform->p_and_l = 0.0;         
	response->pl_pc = 0.0;
	response->hit = UNKNOWN;
	if(!response->pos_changed)
		return;

/* 
 *	Test if opening a position from a previously FLAT position.
 *	I.E. previously out of the market and going LONG or SHORT. 
 *	Otherwise we are closing a position. 
 */ 
	if (response->prev_position == P_FLAT)  
	{ 
		response->prev_sig = response->curr_sig;
		response->prev_price = price;
	} 
/* 
 *	Closing a LONG position, first with profit, then without.
 *	If the price hasn't changed then assume a loss because of transaction costs. 
 */ 
	else if (response->prev_position == P_LONG)
	{ 
		if (price > response->prev_price)  
		{ 
			perform->total_trades++;
			perform->prof_trades++;  
			response->hit = TRUE;
			gain = (price - response->prev_price) * transaction_size - transaction_cost;
			if (gain > perform->max_gain)  
				perform->max_gain = gain;
			perform->total_gains += gain; 
			perform->stdev_pl += gain*gain;
			response->equity += gain;
			perform->p_and_l = gain; 
			response->pl_pc =  (price - response->prev_price)/response->prev_price;
			perform->cum_pl_pc +=  response->pl_pc;
			response->prev_sig = 0;
			response->drawdown = 0;
			response->num_drawdowns = 0;
		}
		else
		{
			perform->total_trades++;
			loss = (response->prev_price - price) * transaction_size + transaction_cost;
			if (loss > perform->max_loss)  
				perform->max_loss = loss;
			perform->total_losses += loss;
			response->hit = FALSE;
			response->equity -= loss;
			perform->p_and_l = -loss;
			response->pl_pc =  (price - response->prev_price)/response->prev_price;
			perform->cum_pl_pc +=  response->pl_pc;
			perform->stdev_pl += loss*loss;
			response->drawdown += loss;
			response->num_drawdowns++;
			if (response->num_drawdowns > perform->max_num_dds)  
				perform->max_num_dds = response->num_drawdowns;
			if (response->drawdown  >=  perform->max_dd_val)  
				perform->max_dd_val = response->drawdown;
		} 
	}
/* 
 *	Closing a SHORT position, first with profit, then without.
 *	If the price hasn't changed then assume a loss because of transaction costs. 
 */ 
	else if (response->prev_position == P_SHORT)  
	{ 
		if(price < response->prev_price)  
		{ 
			perform->total_trades++;
			perform->prof_trades++;  
			response->hit = TRUE;
			gain = (response->prev_price - price) * transaction_size - transaction_cost;
			if (gain > perform->max_gain)  
				perform->max_gain = gain;
			perform->total_gains += gain; 
			perform->stdev_pl += gain*gain;
			response->equity += gain;
			perform->p_and_l = gain;
			response->pl_pc =  (response->prev_price - price)/response->prev_price;
			perform->cum_pl_pc +=  response->pl_pc;
			response->drawdown = 0;
			response->num_drawdowns = 0;
		}
		else
		{
			perform->total_trades++;
			loss = (price - response->prev_price) * transaction_size + transaction_cost;
			if (loss > perform->max_loss)  
				perform->max_loss = loss;
			perform->total_losses += loss;
			response->hit = FALSE;
			response->equity -= loss;
			perform->p_and_l = -loss;
			perform->stdev_pl += loss*loss;
			response->drawdown += loss;
			response->pl_pc =  (response->prev_price - price)/response->prev_price;
			perform->cum_pl_pc +=  response->pl_pc;
			response->num_drawdowns++;
			if (response->num_drawdowns > perform->max_num_dds)  
				perform->max_num_dds = response->num_drawdowns;
			if (response->drawdown  >=  perform->max_dd_val)  
				perform->max_dd_val = response->drawdown;
		} 
	} 

} /* End of Void Gains_Losses() */



Void Summary_P_And_L(PerformRecPtr perform, ResultsRecPtr response, Int num_records,
Double init_capital)
/* 
 *	Calculate all the summary results in the perform record. 
 */ 
{

	Double temp1, temp2, temp3;
	Int num_days;
	   	
	perform->p_and_l = perform->total_gains - perform->total_losses;
	perform->avg_pl = perform->p_and_l;
	if(perform->total_trades <= 1)   
	{
		perform->total_trades = 1;
		perform->stdev_pl = 1.0;
	}
	else
	{
/*
 *	Patent method of calculating s.d. on the fly - requires sum of squares.
 */
		perform->stdev_pl = perform->stdev_pl - 
perform->avg_pl * perform->avg_pl/perform->total_trades;
		perform->stdev_pl = sqrt(perform->stdev_pl /(perform->total_trades-1));		
		perform->avg_pl	 /= perform->total_trades;
	}

	
/* 
 *	Calculate some temp values to faciltate producing RoR figures 
 */ 
	num_days = (Int) (num_records * 365.0 / TRADING_DAYS_IN_YEAR);
	temp1 = num_days / 365.0;
	temp2 = response->equity / init_capital;
	temp3 = exp(temp1 * log(ABS(temp2)));        
	
	/* calculate the other performance results */ 
	perform->pc_prof_trades = (perform->prof_trades*100.0 / perform->total_trades);
	perform->pl_end_period = response->equity;    
	if(perform->prof_trades)
		perform->avg_gains = perform->total_gains / perform->prof_trades;
	else
		perform->avg_gains = 0.0;
	if ((perform->total_trades - perform->prof_trades) == 0)  
		perform->avg_losses = 0.0;
	else
		perform->avg_losses = perform->total_losses / (perform->total_trades - perform->prof_trades);
	if ((perform->total_trades - perform->prof_trades) == 0)  
		perform->win_loss_ratio = 0.0;
	else
		perform->win_loss_ratio = perform->prof_trades * 1.0 / (perform->total_trades - perform->prof_trades);
	perform->ror_pc = (response->equity - init_capital)/init_capital;
	perform->annual_ror_pc = (temp3 - 1.0) * 100.0;
	perform->sharps_ratio = perform->avg_pl / perform->stdev_pl;
	perform->confidence = Confidence((perform->avg_pl / perform->stdev_pl), 2) * 100.0;
	if (EQUAL(perform->max_dd_val, 0.0))  
		perform->avg_pl_to_max_dd = 0.0;
	else
		perform->avg_pl_to_max_dd = perform->avg_pl  / perform->max_dd_val;

} /* End of Void Summary_P_And_L() */




 


