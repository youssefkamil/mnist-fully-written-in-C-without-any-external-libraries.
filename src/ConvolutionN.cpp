#include "ConvolutionN.h"
#include <iostream>
#include "matrix.h"
#include <cmath>

using namespace std;

ConvN::ConvN(trainSet* TS,int nlayers, PoolingType ptype): ts(TS),nL(nlayers), PT(ptype)

{
    nFlayer=matI(nL+1);
    nFlayer[-1]=1;
}

void ConvN::Creat()
{
    int i;
    InConvD=matI(nL+1); //one the output dimension of padding function then the output dimension of pooling function
    InPoolD=matI(nL);   //output dimension of convolution function
    mOutConv=new double**[nL];    //mOutConv[nL][nFilter][nPixel]
    mOutPool=new double**[nL];    //mOutPool[nL][nFilter][nPixel]
    nAllfilters=0;

    for(i=0;i<nL;i++)
    {
        nAllfilters+=(nFlayer[i]*nFlayer[i-1]);

        if(i==0)
        {
            InConvD[i-1]= MainD + PaddStep*2;
            padding=matD(InConvD[i-1]*InConvD[i-1]);    //padding at first layer only
            MainM=matD(1,1,InConvD[i-1]*InConvD[i-1]);  //change the output of padding one dimension
                                                        //to three dimension
        }
        InPoolD[i]=(InConvD[i-1]-Fsize)/1+1;
        InConvD[i]=InPoolD[i]/2;

        mOutConv[i]=new double*[nFlayer[i]];
        mOutPool[i]=new double*[nFlayer[i]];

        for(int j=0; j<nFlayer[i]; j++)
        {
            mOutConv[i][j]=new double[InPoolD[i]*InPoolD[i]];
            mOutPool[i][j]=new double[InConvD[i]*InConvD[i]];
        }
    }

    if(i==nL) FinalOut2=matD(60000,nFlayer[nL-1]*InConvD[nL-1]*InConvD[nL-1]); //finalout two dimension

    Filters=matD(nAllfilters,(Fsize*Fsize));
    ifstream fil;
    fil.open("Data\\F105.txt");
    if (!fil)
    {
		cout << "Unable to load filters";
		exit(1);   // call system to stop
	}
    for(int i=0;i<nAllfilters;i++)
    {
        for(int j=0;j< Fsize*Fsize; j++)
        {
            //Filters[i][j]=((rand()%100)-50)/250.00;
            fil >> Filters[i][j];
        }

    }
    fil.close();

    fillFilters(Filters);
}

void ConvN::Creat(double* M)
{
    for(int j=0;j<InConvD[-1]*InConvD[-1];j++) MainM[0][0][j]= M[j];
        FLindex=0;
}

void ConvN::Padding(double* M)
{
	int cols = (MainD + (PaddStep * 2));				//Number of columns in the new matrix
	int rows = (MainD + (PaddStep * 2));				//Number of rows in the new matrix
	int sizeNew = cols * rows;						//The size of the new matrix
		for (int i = 0; i < rows; i++)				//Rows of the new matrix
		{
			for (int j = 0; j < cols; j++)			//Columns of the new matrix
			{
				if (i < PaddStep || i >= MainD + PaddStep || j < PaddStep || j >= MainD + PaddStep)		//If the pixel lies in the padded area
					padding[j + cols * i] = -1;
				else
				{
					int col = j % cols;									//Get column number from the new matrix
					col -= (PaddStep);										//Calculate column number in the old matrix
					int row = i % rows; row -= (PaddStep);					//The same for rows
					int place = col + row * MainD;						//The Pixel's place in the old matrix
					padding[j + cols * i] = M[place];
				}
			}
		}
}

void ConvN::conv(double*** Mconv,int Li)
{
   int Findex;      //index for data at each filter
   int FLi=FLindex;
   int newnpix= InPoolD[Li] * InPoolD[Li];
   int n;
   double a=0.0;
   int index =1;
   int ss=0;
   int j,s;

   for(j=0;j<nFlayer[Li];j++)
    {
        ss=0;
        a=0.0;
        index =1;
        FLi=FLindex;

        for(s=0;s<newnpix;s++)
        {
           Findex=0;

           if(s==InPoolD[Li]*index)  {ss=InConvD[Li-1]*index; index++;}
           else if(s!=0) ss++;

           n=ss;

           while(Findex!=Fsize*Fsize)
           {
               if(Li==0) a+=Mconv[0][0][n] * Filters[j][Findex]; //if it is the first layer, then convolution for one matrix only
               else
               {
                   for(int pi=0;pi<nFlayer[Li-1];pi++)   //pi: picture index
                    {a+=Mconv[Li-1][pi][n] * Filters[FLi][Findex]; FLi++;}
               }

               if(((Findex+1)%Fsize)==0)  n+=InConvD[Li-1]-Fsize+1;
               else n++;

               Findex++;
               FLi=FLindex;
           }
           mOutConv[Li][j][s]=tanh(a);
           a=0.0;
        }

        if(j==nFlayer[Li]-1) //FLindex=j+1;
            FLindex=nFlayer[Li]*nFlayer[Li-1];
        else FLindex+=nFlayer[Li-1];
    }
}

void ConvN::MaxPooling(double*** MPool,int Li)
{
	int NewPoolpix = InConvD[Li] * InConvD[Li];     //NewPoolpix:new number of pixels for the output image of the pooling layer

	int n;
	int index=2;
	double a = 0.0;
	int ss=0;

	for (int j = 0; j < nFlayer[Li]; j++)
	{
	    a = 0.0;
	    ss=0;
	    index=2;

        for (int s = 0; s < NewPoolpix; s++)
        {
            if((s%InConvD[Li]==0)&&(s!=0))  {ss=InPoolD[Li]*index; index+=2;}
            else if(s!=0) ss+=2;

            n=ss;
            a=MPool[Li][j][n];

            for (int p = 0; p < 4; p++)
            {
                if (a < MPool[Li][j][n]) a = MPool[Li][j][n];

                if (p == 1) n += InPoolD[Li] - 1;
                else n++;

            }

            mOutPool[Li][j][s] = a;
        }
	}
}

void ConvN::AvgPooling(double*** MPool,int Li)
{
	int NewPoolpix = InConvD[Li] * InConvD[Li];     //NewPoolpix:new number of pixels for the output image of the pooling layer
	int n;
	int index=2;
	double a = 0.0;
	int ss=0;

	for (int j = 0; j < nFlayer[Li]; j++)
	{
	    a = 0.0;
	    ss=0;
	    index=2;

        for (int s = 0; s < NewPoolpix; s++)
        {
            if((s%InConvD[Li]==0)&&(s!=0))  {ss=InPoolD[Li] * index; index+=2;}
            else if(s!=0) ss+=2;

            n=ss;
            a=0.0;

            for (int p = 0; p < 4; p++)
            {
                a += MPool[Li][j][n];

                if (p == 1) n += InPoolD[Li] - 1;
                else n++;

            }

            mOutPool[Li][j][s] = a/4;
        }
	}

}

void ConvN::ConvFF()
{
    int i;
    for(i=0;i<nL;i++)
    {
        if(i==0)
        {
            conv(MainM,i);
            if(PT==Max) MaxPooling(mOutConv,i);
            else if(PT==Avg) AvgPooling(mOutConv,i);
        }
        else
        {
            conv(mOutPool,i);
            if(PT==Max) MaxPooling(mOutConv,i);
            else if(PT==Avg) AvgPooling(mOutConv,i);
        }
    }

}

void ConvN::train(int npat)
{
    for(int i=0;i<npat;i++)
    {
        Creat(ts->x[i]);
        FLindex=0;
        ConvFF();

        for(int s=0;s<nFlayer[nL-1];s++)
            for(int n=0;n<InConvD[nL-1]*InConvD[nL-1];n++)
                FinalOut2[i][(s* InConvD[nL-1]*InConvD[nL-1]) + n] = mOutPool[nL-1][s][n];
    }
}


void ConvN::fillFilters (double** F1)
{

//vertical
    F1[0][0]=1; F1[0][1]=0; F1[0][2]=0; F1[0][3]=0; F1[0][4]=-1;
    F1[0][5]=1; F1[0][6]=0; F1[0][7]=0; F1[0][8]=0; F1[0][9]=-1;
    F1[0][10]=1; F1[0][11]=0; F1[0][12]=0; F1[0][13]=0; F1[0][14]=-1;
    F1[0][15]=1; F1[0][16]=0; F1[0][17]=0; F1[0][18]=0; F1[0][19]=-1;
    F1[0][20]=1; F1[0][21]=0; F1[0][22]=0; F1[0][23]=0; F1[0][24]=-1;
 //horizontal
    F1[1][0]=1; F1[1][1]=1; F1[1][2]=1; F1[1][3]=1; F1[1][4]=1;
    F1[1][5]=0; F1[1][6]=0; F1[1][7]=0; F1[1][8]=0; F1[1][9]=0;
    F1[1][10]=0; F1[1][11]=0; F1[1][12]=0; F1[1][13]=0; F1[1][14]=0;
    F1[1][15]=0; F1[1][16]=0; F1[1][17]=0; F1[1][18]=0; F1[1][19]=0;
    F1[1][20]=-1; F1[1][21]=-1; F1[1][22]=-1; F1[1][23]=-1; F1[1][24]=-1;
//inward

    F1[2][0]=0; F1[2][1]=1; F1[2][2]=1; F1[2][3]=1; F1[2][4]=0;
    F1[2][5]=1; F1[2][6]=1; F1[2][7]=1; F1[2][8]=1; F1[2][9]=1;
    F1[2][10]=1; F1[2][11]=1; F1[2][12]=-24; F1[2][13]=1; F1[2][14]=1;
    F1[2][15]=1; F1[2][16]=1; F1[2][17]=1; F1[2][18]=1; F1[2][19]=1;
    F1[2][20]=0; F1[2][21]=1; F1[2][22]=1; F1[2][23]=1; F1[2][24]=0;
//outword
    F1[2][0]=0; F1[2][1]=-1; F1[2][2]=-1; F1[2][3]=-1; F1[2][4]=0;
    F1[2][5]=-1; F1[2][6]=-1; F1[2][7]=-1; F1[2][8]=-1; F1[2][9]=-1;
    F1[2][10]=-1; F1[2][11]=-1; F1[2][12]=24; F1[2][13]=-1; F1[2][14]=-1;
    F1[2][15]=-1; F1[2][16]=-1; F1[2][17]=-1; F1[2][18]=-1; F1[2][19]=-1;
    F1[2][20]=0; F1[2][21]=-1; F1[2][22]=-1; F1[2][23]=-1; F1[2][24]=0;
//diagonal 45
    F1[4][0]=0; F1[4][1]=1; F1[4][2]=1; F1[4][3]=1; F1[4][4]=1;
    F1[4][5]=-1; F1[4][6]=0; F1[4][7]=1; F1[4][8]=1; F1[4][9]=1;
    F1[4][10]=-1; F1[4][11]=-1; F1[4][12]=0; F1[4][13]=1; F1[4][14]=1;
    F1[4][15]=-1; F1[4][16]=-1; F1[4][17]=-1; F1[4][18]=0; F1[4][19]=1;
    F1[4][20]=-1; F1[4][21]=-1; F1[4][22]=-1; F1[4][23]=-1; F1[4][24]=0;
//diagonal 135
    F1[5][0]=-1; F1[5][1]=-1; F1[5][2]=-1; F1[5][3]=-1; F1[5][4]=0;
    F1[5][5]=-1; F1[5][6]=-1; F1[5][7]=-1; F1[5][8]=0; F1[5][9]=1;
    F1[5][10]=-1; F1[5][11]=-1; F1[5][12]=0; F1[5][13]=1; F1[5][14]=1 ;
    F1[5][15]=-1; F1[5][16]=0; F1[5][17]=1; F1[5][18]=1; F1[5][19]=1;
    F1[5][20]=0; F1[5][21]=1; F1[5][22]=1; F1[5][23]=1; F1[5][24]=1;;
}

