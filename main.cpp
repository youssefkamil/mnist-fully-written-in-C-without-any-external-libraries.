#include <iostream>
#include <cmath>
#include<cstdlib>
#include <ctime>
#include<conio.h>
#include<thread>
#include "layer.h"
#include "trainSet.h"
#include "net.h"
#include "trainer.h"
#include "ConvolutionN.h"
using namespace std;

class layer;
class net;
class trainer;
class trainSet;

int main()
{

    /** TRAINSET SECTION **/
    thread* t = new thread[10];
    trainer** tr = new trainer* [10];
    clock_t tic = clock();
    float*** i = new float** [10];
    for (int vv=0; vv<10; vv++)
    {
        i[vv] = new float* [300];
        for (int gg=0; gg<300; gg++)
            i[vv][gg] = new float [2];
    }
    net** N = new net* [10];
    srand(time(NULL));
    trainSet* ts = new trainSet();
    ts->loadtest_x();


    ConvN* c = new ConvN(ts,2,Avg);
    c->nFlayer[0]=6;
    c->nFlayer[1]=16;
    c->Fsize=5;
    c->PaddStep=0;
    c->MainD=28;
    c->Creat();
    c->train(ts->nPat);
    ts->nIn = 256;  // new size of input
    ts->xt = c->FinalOut2;

    ts->loadMnist_x();

    c = new ConvN(ts,2,Avg);
    c->nFlayer[0]=6;
    c->nFlayer[1]=16;
    c->Fsize=5;
    c->PaddStep=0;
    c->MainD=28;
    c->Creat();
    c->train(ts->nPat);
    ts->nIn = 256;  // new size of input
    ts->x = c->FinalOut2;

    //end convolution section....

    /*training section*/
    cout<<endl<<"start training..."<<endl;
   for (int z=0; z<10; z++)
    {
        ts->loadtest_y(z);
        ts->yt = ts->y;
        ts->loadMnist_y(z);
        N[z]= new net(3,ts);
        N[z]->alfa = 0.02;      //learning rate
        N[z]->nForLayers[0]=50; //number of neurons in hidden layer
        N[z]->nForLayers[1]=30;
        N[z]->Creat();
        N[z]->Ls[0]->active=1;
        N[z]->Ls[1]->active=1;
        tr[z] = new trainer(N[z],ts);
        tr[z]->err = 100;
        tr[z]->percent = 10;
        tr[z]->stoptime = 7080000;
        t[z] = thread(trainer::train,tr[z],99999999999999,z,ref(i[z]));
        this_thread::sleep_for(chrono::seconds(1));
    }
    for (int g=0; g<10; g++)        t[g].join();
    clock_t toc = clock();
    ofstream logs("Report\\log.txt",ios::app);
    logs<<"///////////////////////////////////////////////"<<endl;
    for (int z=0; z<10; z++)
        logs<<"No. of ERRORS for #"<<z<<": "<<i[z]<<endl;
    logs<<"training time :"<<float (toc-tic)/CLOCKS_PER_SEC<<" s"<<endl;
    ts->x = ts->backup;
    delete ts;

    return 0;
}
