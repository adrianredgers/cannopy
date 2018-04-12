#include <fstream.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "neural.hpp"

int main(int argc, char** argv)
{
    if(!argc == 2)
    {
        cerr<<"USAGE: NEURAL infile\n";
        exit(1);
    }
    Network net;
    if(net.trainingFile(argv[1]) == false)
    {
        cerr<<"Error in initializing net\n";
        exit(2);
    }
#ifdef DISPLAY_YES
    CLEAR_SCREEN();
#endif
    randomize();
    time_t startTime;
    time(&startTime);
    for(;;){
        net.forwardPass();
        net.backwardProp();
        net.displayDiff();
        if(net.atEpochEnd())
        {
            if(kbhit())
                break;
            if(net.trained())
                break;
                net.displayTotalError();
            net.zeroTotalError();
#ifdef DISPLAY_YES
                  CURSOR_GOTO(1,1);
#endif
        }
    }
    time_t endTime;
    time(&endTime);
    net.allForward();
    cin.get();
    net.display();
    unsigned long elapsed = (unsigned) endTime - (unsigned) startTime;
    net.displayPerformance(elapsed);
}


    
