#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "trainSet.h"
#include <fstream>

enum PoolingType{Max,Avg};
class ConvN
{
public:
    trainSet* ts;
    int nL;
    int* nFlayer;      //for each layer, number of filters
    int FLindex;             //filters for each layer index
    PoolingType PT;
    int Fsize;               //assume all filters for all layers are same size
    int PaddStep;          //padding step
    int MainD;             //main dimension of picture
    int nAllfilters;
    int* InConvD;           //input dimension foe conv from padding or pooling of previous layer
    int* InPoolD;           //input Dimension for pooling = out dimension for convolution
    double** Filters;                //Filters Matrix two dimension[Filter number][Pixels at each filter]
    double* padding;
    double*** MainM;           //main matrix but in two dimension matrix[picture number][number of pixels]
    double*** mOutConv;       //moutconv[convLayerNum][FilterNum][Pixels]
    double*** mOutPool;       //moutpool[convLayerNum][FilterNum][Pixels]
    double* FinalOut;          //final out one dimension
    double** FinalOut2;        //final out 2 dimension [npatn][pixels]

    ConvN(trainSet* TS, int nlayers ,PoolingType PT);

    void Creat();
    void Creat(double* M);              //pading ant turn the input matrix one dimention to three dimension
    void fillFilters (double** F1);
    void Padding(double* m);    //I change MainX and MainY to MainD, I change padX and PadY to paddstep
    void conv(double*** MConv, int Li);  //MConv:input matrix to convolution function,li: layer index
    void MaxPooling(double*** MPool, int Li);
    void AvgPooling(double*** MPool, int Li);
    void ConvFF();
    void train(int npat);

};


#endif // CONVOLUTION_H
