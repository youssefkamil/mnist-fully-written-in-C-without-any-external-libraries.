#ifndef TRAINER_H
#define TRAINER_H
#include "net.h"
#include "trainSet.h"
#include "layer.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include<conio.h>
#include <cstdlib>
#include <ctime>

class trainSet;
class net;

class trainer
{
  public:
  int m;                //current pattern number
  int percent;          //dropout percentage
  int stoptime;
  double err;           //error % to end training if lower
  trainSet* ts;         //pointer to training set
  net* Net;             //the net
  float errorCount;     //sum of pattern in error
  double* pa;           //the output of the last layer
  double* mda;          //dl/da
  //the upcoming pointers are to variables to improve computation
  int* pnIn;            //pointer #input
  int* pnOut;           //pointer #output
  int* pnPat;           //pointer #patterns
  int* nL;              //pointer #layers
  double** px;          //pointer to input
  double** py;          //pointer to output
  int* ind;
  trainer(net* theNet, trainSet* Ts);
  void NFF();           //network feed forward
  void NBP();           //network back propagation
  void Nupdate(int m);       //network update b , w
  void Nreset();        //network reset db , dw
  void Ndropout(int percent);   // network dropout neurons with percentage
  void delta();         //to update dl/da
  float train (int cycles,int z,float**& b);       //to train
  float trainbat (int cycles);    //to train with batching mode , still need a lot of work !!!!!
  void dice();                    // randomly toggle trainset between shear and original
  int test();                     //test based on highest output
  int Otest();                    //test based on error between actual and predicted
};



#endif // TRAINER_H
