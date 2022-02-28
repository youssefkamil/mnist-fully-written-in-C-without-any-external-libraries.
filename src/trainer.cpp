#include "trainer.h"
#define pross (m/60000.0) * 100.0
#define sucss (1.0 - (errorCount/60000) ) * 100.0

using namespace std;
/////////////////////////////////////////////////////////////////////////
trainer::trainer(net* theNet, trainSet* Ts)
{
    percent = 0;
    stoptime = 600000;
    err= 500;
    Net= theNet;
    ts =Ts;
    px = ts->x;
    py = ts->y;
    nL = &(Net->nL);
    pnIn = Net->pnIn;
    pnOut = Net->pnOut;
    pnPat = Net->nPatterns;
    pa = Net->Ls[(*nL-1)]->mOutF;
    mda = new double [*pnOut];
    Net->Ls[(*nL-1)]->pInB = mda;
    ind= new int [(*pnPat)*(ts->Xsize)]; //array of index for dynamic selection Batching


    for (int i=0; i<Net->nL; i++)   Net->Ls[i]->tr = this;      // link layer to trainer class
}
/////////////////////////////////////////////////////////////////////////
void trainer::Nreset()
{
  int i ;
  for (i = 0; i<*nL; i++)
        Net->Ls[i]->reset();
}
/////////////////////////////////////////////////////////////////////////
void trainer::NFF()
{
    int i ;
    for (i = 0; i<*nL; i++)
        Net->Ls[i]->FF();

}
/////////////////////////////////////////////////////////////////////////
void trainer::NBP()
{
    int i;
    for (i = *nL; i>0; i--)
        Net->Ls[i-1]->BP();
}
/////////////////////////////////////////////////////////////////////////
void trainer::Nupdate(int m)
{
  int i;
  for (i = *nL; i>0; i--)
        Net->Ls[i-1]->update(m);
}
/////////////////////////////////////////////////////////////////////////
void trainer::Ndropout(int percent)
{
    int i ;
    for (i = 0; i<*nL-1; i++)
        Net->Ls[i]->dropout(percent);
        Net->Ls[*nL-1]->dropout(-1); // we don't want to dropout in output layer
}
/////////////////////////////////////////////////////////////////////////
void trainer::delta()     // 0 for MaxError , 1 for Loss , 2 for error Count , default is error count
{
    int error = 0;
    int s;
    for (s=0; s<*pnOut; s++)
    {
        mda[s] = py[m%(ts->nPat)][s] - pa[s];
        error += ( abs(mda[s])>0.8 );
        if(abs(mda[s])<=0.8 )        //relate to dynamic selection Batching
           ind[m]=0;
    }
    errorCount += (error>0);
}
/////////////////////////////////////////////////////////////////////////
void trainer::dice()
{
    if (rand()%2)   ts->toggleset();
}
/////////////////////////////////////////////////////////////////////////
float trainer::train (int cycles, int z,float**& b)
{
    clock_t tic = clock();
    clock_t toc;
    int c,mode=1;       // Mode 1 training , Mode 0 testing
    int errbat=0;        // numbers of patters still training in the Batch
    int batch=0;         //number of batching
    int counter=0;      //if error in every batch<10 counter increased to exit batching mode
    bool batching=1;
     for (c=0; c<cycles; c++)
    {
        toc = clock();

        //------------------------------ batching training ------------------------------
        while(batching)
        {
            toc = clock();
            Ndropout(percent);
            batch++;
            errorCount = 0;
            int g=0;
            for (m = 0; m<(*pnPat)*(ts->Xsize); m++ ) //create array of index
            {
                int r=rand()%100;
                if(r>1)
                    ind[m]=0;
                else if (r<=1)
                    ind[m]=1;
            }
        //-------------------to check the errors in the batch before training the same batch---------------
            for (m = 0; m <(*pnPat)*(ts->Xsize); m++ )
            {
                if(ind[m]==1)
                {
                    Net->Ls[0]->pInF = px[m];
                    NFF();
                    delta();
                }
            }
        if(batch%20==0)
            cout<<endl<<endl<<"Batch "<<batch<<"  #"<< z <<" : "<< errorCount;

            if(errorCount<10)   //will exit Batching when errorcount < 10 for 3 times
            {
                counter++;
                if(counter==5)  batching=0;
            }
            else
                counter=0;

    //---------------------------------training the Batch---------------------------
            do
            {
                errorCount=0;
                errbat=0;
                for (m = 0; m <(*pnPat)*(ts->Xsize); m++ )
                    {
                        if(ind[m]==1)
                        {
                            errbat++;
                            Net->Ls[0]->pInF = px[m];
                            NFF();
                            delta();
                            NBP();
                        }
                    }
                if(errorCount>0)    //special case, to not divide by zero
                {
                    Nupdate(errbat);
                    Nreset();
                }
            }   while(errorCount>0);

        if (  (toc-tic) >stoptime )       // to break from batching if more than 2 hours
            break;
        }

        if (  (toc-tic) >stoptime )       //to  break if training takes more than 2 hours
        break;


        //------------------------------ full batch training --------------------------------

        Nreset();
        errorCount = 0;
        if ( (mode == 1)&&(c%20 == 0 )) Ndropout(percent);  // change dropout every 20 cycle
        else if (mode == 0) Ndropout(0);

        int g=0;        // update after g pattern
        for (m = 0; m <(*pnPat)*(ts->Xsize); m++ )
        {
            Net->Ls[0]->pInF = px[m];
            NFF();
            delta();
            if ( (g == 10) && (mode == 1) )
            {
                Nupdate(g+1);
                Nreset();
                g=0;
            }
            if (mode == 1)
                NBP();
            g++;
        }
        //display progress
        cout<<endl<<endl<<"\t\t\t\t\t"<<c+1<<"cycle ";
        cout<<"errors:"<<errorCount<<"   #"<<z<<"  MODE "<<mode;
        if ( mode == 1)
            Nupdate(g+1);
        toc = clock();
         if( (errorCount<err) && (mode == 0) )
            break;

        else if (errorCount<err)
            { mode =0;  --c; }
        else if (errorCount>150)    //if the error more than 150 turn on mini batching
                {batching=1;}
        else
            { mode =1; Ndropout(percent); }
    }

    b[z][c] = errorCount;
    return errorCount;
}

/////////////////////////////////////////////////////////////////////////
int trainer::test()
{
   Ndropout(0);
   int errorCount = 0;
   double checkY,checkPA;
   int idxpa,idxy;
   int m,s;
   for (m=0; m< *pnPat; m++)
   {
        checkY = -1;
        checkPA= -1;
        idxy = -1;
        idxpa = -2;
        Net->Ls[0]->pInF = px[m];
        NFF();
        for (s=0; s<*pnOut; s++)
            {
                if (py[m][s]>checkY)      {  idxy = s;   checkY = py[m][s]; }
                if (pa[s]>checkPA)       {  idxpa = s;  checkPA = pa[s];  }
            }
        errorCount += ( idxy!=idxpa );

        /**display wrong patterns**/
       /* ts->toggleset();
        if (idxy!=idxpa)
        {
            ts->displayIm(m);
            cout<<endl;
            cout<<"predicted :"<<idxpa;
            cout<<endl;
            for (s=0; s<10; s++)
            {
                cout<<pa[s]<<" ";
            }
      cout<<endl;
            getch();
          }
        ts->toggleset();*/
   }
   return errorCount;
}
/////////////////////////////////////////////////////////////////////////
int trainer::Otest()
{
   Ndropout(0);
   int errorCount = 0;
   int error;
   int m,s;
   for (m=0; m< *pnPat; m++)
   {
        Net->Ls[0]->pInF = px[m];
        NFF();
        error=0;
        for (s=0; s<*pnOut; s++)
        {
            mda[s] = py[m][s] - pa[s];
            error += ( abs(mda[s])>0.7 );
        }
        errorCount += (error>0);
   }
   return errorCount;
}
/////////////////////////////////////////////////////////////////////////
