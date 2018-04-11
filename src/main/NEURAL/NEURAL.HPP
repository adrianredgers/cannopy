//Neural.hpp
//(c) 1990, Miller Freeman Publications
//Released to the public domain
//Disclaimer of warranty: If you use this and something bad happens, you can
//have my sympathy but not my money
//Larry O'Brien
//V. 3.0
  
#ifndef NEURAL_HPP
#define NEURAL_HPP
  
#define OUTLAYER (nLayers - 1)
  
//Portability defines.  These are the functions which should give you
//some trouble if you're trying to port the code to another compiler
//You could replace them other compiler-specific code or, for maximum
//portability, ANSI escape sequences.  Or, leave DISPLAY_YES undefined
//and they will be stripped out.  The network will run and tell you only
//the total error (for all patterns).  Boring, but portable
#define DISPLAY_YES
#define CURSOR_GOTO gotoxy
#define SET_TEXT_ATTR textcolor
#define CLEAR_SCREEN clrscr
#define DOS_PRINT cprintf
  
const int M_FACTOR = 5;    //This scales the input to the neuron
const int MAX_LAYERS = 3;
const int MAX_NEURONS = 40;
const int ALLOC_ERR = 5;
  
enum bool{false,true};
  
struct Neuron{
    Neuron(void);
    void setZero(void);
    inline void transfer(void);
    inline float derivTransfer(float);
    void sumOfErrors(float);
    float input;
    float output;
    float error;
};
  
struct Connection{
    void set(Neuron*, Neuron*);
    void setRandom(float, float, float);
    void displaySelf(void);
    inline void feedForward(void);
    float weight;
    float learningConstant;
    float samadCoefficient;
    float momentum;
    float delta;
    Neuron* n1;
    Neuron* n2;
    void adjust(void);
    inline int firstNeuronIs(Neuron* n);
    inline int secondNeuronIs(Neuron* n);
};
  
struct Pattern{
    int getMem(int, int);
    float* in;
    int* out;
};
  
class Network{
    Neuron*  neuron[MAX_LAYERS][MAX_NEURONS];
    Connection*  cnxn;
    int   nLayers;
    int nCnxns, nNeurons;
    int*  nInLayer;
    int nPatterns;
    int atPattern;
    int inWidth, inDepth, outWidth, outDepth;
    int iteration;
    Pattern*  pat;
    float acceptableError;
    float thisPatternError;
    float totalError;
    void adjustWeights(void);
    void calcOutputError(void);
    void calcMiddleError(void);
public:
    Network(void);
    Network(int, int*);
    bool trained(void);
    bool trainingFile(char*);
    void displayDiff(void);
    void display(void);
    void displayTotalError(void);
    void displayPerformance(unsigned long);
    void forwardPass(void);
    void backwardProp(void);
    void allForward(void);
    bool atEpochEnd(void);
    void zeroTotalError(void);
};
  
#endif
