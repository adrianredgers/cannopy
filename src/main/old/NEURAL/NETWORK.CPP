#include "neural.hpp"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream.h>
  
  
void Network::adjustWeights(void)
{
    for(int atCnxn = 0; atCnxn < nCnxns; atCnxn++)
        cnxn[atCnxn].adjust();
}

void Network::allForward(void)
{
    CLEAR_SCREEN();
    CURSOR_GOTO(1,1);
    for(atPattern = 0; atPattern < nPatterns; atPattern++)
    {
        forwardPass();
        displayDiff();
    }
}

void Network::backwardProp(void)
{
    calcOutputError();
    calcMiddleError();
    adjustWeights();
}
  
void Network::calcMiddleError(void)
{
    //This would need to be reworked for a multi-middle layer network
    for(int atMidNeurode = 0; atMidNeurode < nInLayer[1]; atMidNeurode++)
    {
        float weightedSumOfErrors = 0;
        int offset = nInLayer[0] * nInLayer[1] + nInLayer[2] * atMidNeurode;
        for(int atHighNeurode = 0; atHighNeurode < nInLayer[OUTLAYER]; atHighNeurode++)
            weightedSumOfErrors += cnxn[offset + atHighNeurode].n2->error * cnxn[offset + atHighNeurode].weight;
        neuron[1][atMidNeurode]->derivTransfer(weightedSumOfErrors);
    }
}
  

void Network::calcOutputError(void)
{
    thisPatternError = 0;
    for(int atNeurode = 0; atNeurode < nInLayer[OUTLAYER]; atNeurode++)
    {
        float diff = pat[atPattern].out[atNeurode] - neuron[OUTLAYER][atNeurode]->output;
        //You may just pass diff to calcLocalError for "classic" backprop
        //Or you can square it for "Quadratic" error func
        //Or you can cube it for "Cubic" error func.
        neuron[OUTLAYER][atNeurode]->derivTransfer(diff);
        if(diff < 0)
        {
            if(diff < -acceptableError)
                thisPatternError -= diff;
        }else{
            if (diff > acceptableError)
            thisPatternError += diff;
        }
    }
    if(thisPatternError > acceptableError)
        totalError += thisPatternError;
}

bool Network::atEpochEnd(void){
    if(atPattern == nPatterns - 1){
        atPattern = 0;
        ++iteration;
        return true;
    }else{
        atPattern++;
        return false;
    }
}
  
  
void Network::display(void)
{
    cout<<"Network iteration "<<iteration<<" Total error: "<<totalError<<"\n";
    int cnxnNumber = 0;
    for(int atLayer = 0; atLayer < nLayers; atLayer++)
    {
        cout<<"Layer "<<atLayer<<"\n";
        for(int atNeuron = 0; atNeuron < nInLayer[atLayer]; atNeuron++)
        {
            cout<<"\tNeuron["<<atNeuron<<"] Input: "<<neuron[atLayer][atNeuron]->input<<" Output: "<<neuron[atLayer][atNeuron]->output<<" Error: "<<neuron[atLayer][atNeuron]->error<<"\n";
            if(atLayer != OUTLAYER)
            {
                for(int atHighNeuron = 0; atHighNeuron < nInLayer[atLayer+1];atHighNeuron++)
                {
                    cout<<"\t\t cnxn["<<cnxnNumber<<"] Weight: "<<cnxn[cnxnNumber].weight<<" Delta: "<<cnxn[cnxnNumber].delta<<'\n';
                    cnxnNumber++;
                }
            }
        }
    }
}
  
void Network::displayDiff(void)
{
#ifdef DISPLAY_YES
   SET_TEXT_ATTR(WHITE);
   DOS_PRINT("Pattern[%d] Error: %f\n\r", atPattern, thisPatternError);
   DOS_PRINT("Desired: ");
   for(int i = 0; i < outDepth * outWidth; i++)
     DOS_PRINT("%6.3f  ", (float) pat[atPattern].out[i]);
   DOS_PRINT("\n\r");
   DOS_PRINT("Actual:  ");
   for( i = 0; i < outDepth * outWidth; i++)
   {
        float err = pat[atPattern].out[i] - neuron[OUTLAYER][i]->output;
        if(err < 0)
            err *= -1;
        if(err > acceptableError)
            SET_TEXT_ATTR(RED);
        else
            SET_TEXT_ATTR(GREEN);
   DOS_PRINT("%6.3f  ",neuron[OUTLAYER][i]->output);
   }
   DOS_PRINT("\n\r");
   SET_TEXT_ATTR(WHITE);
#endif
}
  
void Network::displayTotalError(void)
{
    cout<<"Iteration: "<<iteration<<" Total error: "<<totalError<<"\n";
}

void Network::displayPerformance(unsigned long elapsedTime)
{
   cout<<"Iterations: "<<iteration<<" Elapsed time: "<<elapsedTime<<"\n";
   float CPI = (float) nCnxns * (float) nPatterns * (float) iteration / (float) elapsedTime;
   cout.precision(3);
   cout<<" Connections per second: "<<CPI<<"\n";
}
  
void Network::forwardPass(void)
{
    // Zero all inputs
    for(int atLayer = 0; atLayer < nLayers; atLayer++)
    {
        for(int atNeuron = 0; atNeuron < nInLayer[atLayer]; atNeuron++)
            neuron[atLayer][atNeuron]->input = 0.0;
    }
    //First, put the input pattern directly into the input neuron's output
    //queues.
    for(int atNeuron = 0; atNeuron < nInLayer[0]; atNeuron++)
        neuron[0][atNeuron]->output = pat[atPattern].in[atNeuron];

     //Then, for each layer, feedforward
     int cnxnNumber = 0;
     int offset = 0;
     for(atLayer = 1; atLayer <= OUTLAYER; atLayer++)
     {
        int nCons = nInLayer[atLayer] * nInLayer[atLayer - 1];
        for(cnxnNumber = offset; cnxnNumber < nCons + offset; cnxnNumber++)
            cnxn[cnxnNumber].feedForward();
        offset += nCons;
        for(atNeuron = 0; atNeuron < nInLayer[atLayer]; atNeuron++)
            neuron[atLayer][atNeuron]->transfer();
    }
}
  
Network::Network(void)
{
    nLayers = 0;
    atPattern = 0;
    iteration = 0;
    totalError = 0;
}
  
Network::Network(int nL, int* layerSize)
{
    nLayers = nL;
    nInLayer = new int[nLayers];
    if(!nInLayer)
    {
        cerr<<"Error during memory allocation of nInLayer array!\n";
        return;
    }
    nNeurons = nCnxns = 0;
    for(int i = 0; i < nLayers; i++)
    {
        nInLayer[i] = layerSize[i];
        if(i > 0)
            nCnxns += nInLayer[i] * nInLayer[i - 1];
        nNeurons += nInLayer[i];
    }
    atPattern = 0;
    cnxn = new Connection[nCnxns];
    if(!cnxn)
    {
        cerr<<"Error during memory allocation of connections!\n";
        return;
    }
    for(int atLayer = 0; atLayer < nLayers; atLayer++)
    {
        for(int atNeuron = 0; atNeuron < nInLayer[atLayer]; atNeuron++)
        {
            neuron[atLayer][atNeuron] = new Neuron;
            if(!neuron[atLayer][atNeuron])
            {
                cerr<<"Error during memory allocation of neurons.\n"
                <<nNeurons<<" requested at "<<sizeof(Neuron)<<" bytes.\n";
                return;
            }
        }
    }
}
  
bool Network::trained(void)
{
    if(totalError < acceptableError)
        return true;
    else
        return false;
}

bool Network::trainingFile(char* fileName)
{
    ifstream dataFile(fileName);
    if(!dataFile)
    {
        cerr<<"Error opening file "<<fileName<<"\n";
        return(false);
    }
    dataFile>>"nLayers: ";
     dataFile>>nLayers;
    nInLayer = new int[nLayers];
    if(!nInLayer)
    {
        cerr<<"Error during memory allocation for nInLayer[]!\n";
        return(false);
    }
    nNeurons = nCnxns = 0;
    for(int atLayer = 0; atLayer < nLayers; atLayer++)
    {
        dataFile>>nInLayer[atLayer];
        if(atLayer > 0)
            nCnxns += nInLayer[atLayer] * nInLayer[atLayer - 1];
        nNeurons += nInLayer[atLayer];
    }
    cnxn = new Connection[nCnxns];
    if(!cnxn)
    {
        cerr<<"Error during allocation of "<<nCnxns<<" connections\n";
        return(false);
    }
    for(atLayer = 0; atLayer < nLayers; atLayer++)
    {
        for(int atNeuron = 0; atNeuron < nInLayer[atLayer]; atNeuron++)
        {
            neuron[atLayer][atNeuron] = new Neuron;
            if(!neuron[atLayer][atNeuron])
            {
                cerr<<"Error during memory allocation of neurons.\n"
                <<nNeurons<<" requested at "<<sizeof(Neuron)<<" bytes.\n";
                return(false);
            }
        }
    }
    float learningConstant, momentum, samadCoefficient;
    dataFile>>"acceptableError:">>acceptableError;
    dataFile>>"learningConstant:">>learningConstant;
    dataFile>>"Momentum:">>momentum;
    dataFile>>"samadCoefficient:">>samadCoefficient;
    int atCnxn = 0;
    for( atLayer = 0; atLayer < nLayers - 1; atLayer++)
    {
        for(int atLowNeuron = 0; atLowNeuron < nInLayer[atLayer]; atLowNeuron++)
        {
            for(int atHighNeuron = 0; atHighNeuron < nInLayer[atLayer + 1]; atHighNeuron++)
            {
                cnxn[atCnxn].set(neuron[atLayer][atLowNeuron],neuron[atLayer + 1][atHighNeuron]);
                cnxn[atCnxn].setRandom(learningConstant, momentum, samadCoefficient);
                ++atCnxn;
            }
        }
    }

    dataFile>>"nPatterns:">>nPatterns;
    dataFile>>"width:">>inWidth;
    dataFile>>"depth:">>inDepth;
    dataFile>>"outputWidth:">>outWidth;
    dataFile>>"outputDepth:">>outDepth;
    pat = new Pattern[nPatterns];
    if(!pat)
    {
        cerr<<"Error during memory allocation for patterns\n";
        return(false);
    }
    for(int i = 0; i < nPatterns; i++)
        if(!pat[i].getMem(inWidth * inDepth, outWidth * outDepth))
            return(false);
    if(!pat)
    {
        cerr<<"Error during allocation of memory for patterns!\n";
        return(false);
    }
    int temp;
    for(int atPattern = 0; atPattern < nPatterns; atPattern++)
    {
        for(int i = 0; i < inWidth * inDepth; i++)
        {
            dataFile>>temp;
            if(temp == 1)
                pat[atPattern].in[i] = 1 - acceptableError;
            else
                pat[atPattern].in[i] = acceptableError;
        }
        for( i = 0; i < outWidth * outDepth; i++)
        {
            dataFile>>temp;
            if(temp == 1)
                pat[atPattern].out[i] = 1.0;
            else
                pat[atPattern].out[i] = 0;
        }
    }
    dataFile.close();
    atPattern = 0;
    totalError = 0;
    return true;
}

void Network::zeroTotalError(void)
{
    totalError = 0.0;
}
  
  
