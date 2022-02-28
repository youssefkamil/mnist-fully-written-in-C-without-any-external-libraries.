#ifndef TRAINSET_H
#define TRAINSET_H
#include<fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include<cmath>

using namespace std;
class trainSet
{
    public:

    int nIn;    //input
    int nOut;   //output
    int nPat;   //patterns
    int row;    //No. of rows in image
    int column; //No. of columns in image
    int Xsize;  //how much the trainset is doubled
    bool shear; //if shear is activated or not
    double** x;      //input
    double** xt;
    double** yt;
    double** backup; //backup original input
    double** y;      //output
    trainSet();
    ~trainSet();
    void Creat();    //to actually construct the TS
    void printTs();
    void XfillRand(int v);  //to fill x
    void Yfillparity();     //to fill y with parity

    /**     FOR MNIST     **/
    int readI(ifstream* inDataFile); // to read integer from mnist file
    void readIm(char* name);
    void readLABEL(char* name,int num);
    void displayIm(int pI);
    void loadMnist_x();
    void loadMnist_y(int num =10);      //0-9 to choose which label to output but if 10 this means all labels in one array
    void loadtest_x();
    void loadtest_y(int num);
    void avgpooling();                  //minimizing the image to 14*14 with avg pixels pooling
    void maxpooling();
    void shearIm(float lamdax,float lamday);  //shear affect on images , hint use parameters (1,0.5) or (0.5,1) or (1,0) or (0,1) only for stable image avoiding aliasing ...
    void toggleset();   //switch between different dataset
    void expandset();      //expand the set by Xsize

    // helping destruction to reach unreachable memory and free it
    struct destroy
    {
        double* pat;
        double* label;
    }d;
};

#endif // TRAINSET_H
