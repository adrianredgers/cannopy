/*	pandl.c				UBS				12/4/94				*/
/*
 *	Evaluate a neural network trading strategy.
 *
 */
 
 
#include "header.h"
 
 
Void 	Profit_And_Loss(DataFormatPtr response, DataPairsPtr data, DataPairsPtr prices,
			char *mesg, char *results_file, char *summary_file, 
			Double init_capital, Double transaction_size, Double transaction_cost, 
			Double no_pos_zone)
{
	Int		point, outpos;
	Double	point_err, epoch_err, desired, predict, price, fut_price;
	FILE 	*fp, *sum_fp;
    PerformRec		perform;
    ResultsRec		trade;

/*
 *	Return if there are no data-points to run on.
 */
	if(data->num_records == 0)
		return;

/*
 *	Open the predictions file for outputting the results.
 */
 	fp = Open_File(results_file,"w", "Profit_And_Loss()");
 	sum_fp = Open_File(summary_file,"w", "Profit_And_Loss()");
	fprintf(fp, "\n%s\n", mesg);

	fprintf(fp,"\nWindow-size=%ld, prediction-delay=%ld, pattern-delay=%ld\n",
response->window, response->prediction_delay, response->pattern_delay);
	fprintf(fp,
"\nInitial capital = %lf, transaction size = %lf, trans. cost = %lf\nno-position zone = %lf\n",
init_capital, transaction_size, transaction_cost, no_pos_zone);

	fprintf(sum_fp, "\n\nSummary for Profit and Loss Account\n==================================");

	fprintf(sum_fp,"\n\nWindow-size=%ld, prediction-delay=%ld, pattern-delay=%ld\n",
response->window, response->prediction_delay, response->pattern_delay);

	fprintf(sum_fp,
"\nInitial capital = %lf, transaction size = %lf, trans. cost = %lf\nno-position zone = %lf\n",
init_capital, transaction_size, transaction_cost, no_pos_zone);
	fprintf(fp,
"\npoint\tcurr price\tpredict price\tnext price\tsignal\tposition\toutcome\tp/l\t\t\t fraction p/l %%");       


	epoch_err = 0.0;
	for(point = (response->window - 1), outpos = 0; 
point < (data->num_records - response->prediction_delay); 
point += response->pattern_delay, outpos++) 
	{
		desired = data->values[0][point + response->prediction_delay];        
		price = prices->values[0][point];
		fut_price = prices->values[0][point + response->prediction_delay];
		predict = response->outputs[outpos];
		point_err = (predict-desired)*(predict-desired);
		epoch_err += point_err;

		if(outpos)
		{
			Buy_Sell_Signal(price, &trade, predict); 
			Gains_Losses(price, &perform, &trade, transaction_size,
transaction_cost);
        }
        else
 			Init_Trading(price, &perform, &trade, predict, init_capital, no_pos_zone);
		if(trade.pos_changed && (trade.position == P_FLAT))
	    	fprintf(fp,
"\n%ld\t\t%lf\t%lf\t\t%lf\t%s\t%s\t\t%s\t%lf\t\t%lf", (point+1), 
price, trade.predict_price, fut_price, 
PRINT_BUYSELL(trade.curr_sig), PRINT_MARKETPOSITION(trade.position), 
 PRINT_HITMISS(trade.hit), perform.p_and_l, trade.pl_pc);        
		else
	    	fprintf(fp,
"\n%ld\t\t%lf\t%lf\t\t%lf\t%s\t%s\t\t%s", (point+1), 
price, trade.predict_price, fut_price, 
PRINT_BUYSELL(trade.curr_sig), PRINT_MARKETPOSITION(trade.position), 
 PRINT_HITMISS(trade.hit));        
					

	} /* Next test data-point */

	epoch_err = epoch_err / response->num_records;
	epoch_err = sqrt(epoch_err);
	
	Summary_P_And_L(&perform, &trade, response->num_records, init_capital);

/*
 *	The following line would be meaningless if running on-line - 
 *	 because we wouldn't yet know the desired response.
 */
	fprintf(sum_fp, "\nRMS Error = %lf , 1/RMSE = %lf\n", epoch_err , 1.0/epoch_err);
	fprintf(sum_fp, "\nTrades: total = %ld, profitable = %ld, (= %6.2lf%%), wins/losses = %lf\n",
perform.total_trades, perform.prof_trades, perform.pc_prof_trades, perform.win_loss_ratio);
	fprintf(sum_fp, "\nTotal: gains = %lf, losses = %lf, p/l = %lf\n",
perform.total_gains, perform.total_losses, perform.p_and_l);
	fprintf(sum_fp, "\nAve per trade: gains = %lf, losses = %lf, p/l = %lf\n",
perform.avg_gains, perform.avg_losses, perform.avg_pl);
	fprintf(sum_fp, "\nCapital end period = %lf\n",
perform.pl_end_period);
	fprintf(sum_fp, "\nMaximum: gains = %lf, losses = %lf, drawdown = %lf\n",
perform.max_gain, perform.max_loss, perform.max_dd_val);
	fprintf(sum_fp, "\nMax num drawdowns %ld, avg p/l to max dd = %lf\n",
perform.max_num_dds, perform.avg_pl_to_max_dd);
	fprintf(sum_fp, "\nRate of return = %lf\n",
perform.ror_pc);
	fprintf(sum_fp, "\nSharp's ratio (= ave p/l / s.d p/l) = %lf\n",
perform.sharps_ratio);

	fclose(fp);
	fclose(sum_fp);
	    
} /* End of Void Profit_And_Loss() */


