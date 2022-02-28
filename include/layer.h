#ifndef LAYER_H
#define LAYER_H
#include "trainer.h"
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <fstream>

class trainer;
class layer;

class layer
{
    public:
    int active;     // 0 for linersat , 1 for tanh
    double Dpercent;// dropout percentage
    int nIn;
    int nOut;
    double alfa;
    bool* drop;
    double** w;
    double** dw;
    double* b;
    double* db;
    double* mOutF; //output a
    double* mOutB; // dl/da1
    double* pInF;  //input a
    double* pInB;  // da/da
    int* pnPat;     //pointer patterns
    trainer* tr;    //trainer pointer

    layer(int myin, int myout, double myalfa, int* mynPatern,bool initializ);
    ~layer();
    void makeBefore(layer* L);  //phantom before L
    void FF();      //to FeedForward
    void BP();      //to Back propagation
    void update(int m); // update weights
    void reset();       // reset dw,db for the next cycle
    void dropout(int percent);  // dropout neurons from this layer based on percentage
};

#endif // LAYER_H
