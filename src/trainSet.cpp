#include "trainSet.h"

using namespace std;


trainSet::trainSet()
{
    backup=NULL;
    shear = false;
    Xsize=1;
}
/////////////////////////////////////////////////////////////////////////
trainSet::~trainSet()
{
    int m;
    delete [] d.label;
    if ( d.pat!=NULL ) delete [] d.pat;

    if ( (Xsize>1) && (d.pat==NULL) )
        for (int m=0; m<nPat*Xsize; m++) delete x[m];

    else if (Xsize>1)
        for (int m=nPat; m<nPat*Xsize; m++) delete x[m];

    delete [] x;
    delete [] y;
}
/////////////////////////////////////////////////////////////////////////
void trainSet::Creat()
{
    int i;
    x = new double* [nPat];
    y = new double* [nPat];
    for ( i =0; i<nPat; i++)
    {
        x[i] = new double [nIn];
        y[i] = new double [nOut];
    }
}
/////////////////////////////////////////////////////////////////////////
void trainSet::XfillRand (int v)
{
      int i,j,k;
        for (i=0; i<nPat; i++)
        {
             for (j=0; j<nIn; j++)
             {
                k=rand()%100;
                if(k<v)
                x[i][j]= 1;
                else
                x[i][j]= -1;
             }
        }
}
/////////////////////////////////////////////////////////////////////////
void trainSet::Yfillparity()
{
    int i,j;
    double parity=0;
    for (i=0; i<nPat; i++)
    {
        for (j=0; j<nIn; j++)
        {
            parity = ( x[i][j]==1 && parity != 1)  || (parity ==1 && x[i][j]!=1);
        }
        y[i][0]= (2*parity-1);
         parity = 0;
    }
}
/////////////////////////////////////////////////////////////////////////
void trainSet::printTs()
{
    int i,j;
    for (i=0; i<nIn; i++) cout<<"|  I("<<i<<")\t";
    cout<<"|   P"<<endl;
    for ( i=0; i<nPat; i++ )
    {
        for (j=0; j<nIn; j++)
            cout<<"|  "<<setw(2)<<x[i][j]<<"\t";
            cout<<"|  "<<setw(2)<<y[i][0]<<endl;
    }
    cout<<"---------------------------------"<<endl<<endl<<endl;
}
/////////////////////////////////////////////////////////////////////////

/////////////////////////////// read integer from mnist//////////////////

/////////////////////////////////////////////////////////////////////////

 int trainSet::readI(ifstream* inDataFile)
{
   unsigned char a,b,c,d;
   unsigned int r=0;
   inDataFile->read((char*)(&a), sizeof(char));
   inDataFile->read((char*)(&b), sizeof(char));
   inDataFile->read((char*)(&c), sizeof(char));
   inDataFile->read((char*)(&d), sizeof(char));
   r=d+256*c+65536*b+16777216*a;
   return (int)r;
}
/////////////////////////////readIm  mnist/////////////////////////////////////
void trainSet::readIm(char* name)
{
   int mn, r,w;
   unsigned char* p;
   ifstream imF;
   imF.open(name,ios::binary|ios::in);
	if (!imF) {
		cout << "Unable to open file TRIMG";
		exit(1);   // call system to stop
	}
   mn=readI(&imF); nPat=readI(&imF); r=readI(&imF);  w=readI(&imF);
   nIn=r*w;
   row = r;
   column =w;
   p =(unsigned char *) malloc(nIn*nPat);
   imF.read((char*)(p), nIn*nPat);
   imF.close();
   //cout<<mn<<"  "<<nPat<<"  "<<r<<"  "<<w<<endl;
   //====================all data flat but double======================
   double *  pat= (double *) malloc(sizeof(double)*nIn*nPat);
   int i;
   for (i=0; i<nIn*nPat; i++) pat[i]=(p[i]-127.5)/127.5; // from -1 to 1
   //for (i=0; i<nIn*nPat; i++) pat[i]=p[i]/255.0; // from 0 to 1
   //==================== as a two dimensional array====================
   x=(double**) malloc(sizeof(double*)*nPat);
   for (i=0; i<nPat; i++) x[i]= pat+i*nIn;
   delete [] p;
   d.pat = pat;
   //never do "delete [] pat;"   it is the only place with pattern data
}
////////////////////////////////readLABEL/////////////////////////////////
void trainSet::readLABEL(char* name, int num )
{
   int mn,i,j;
   unsigned char* imL;
   ifstream labF;
   labF.open(name,ios::binary|ios::in);
	if (!labF) {
		cout << "Unable to open file LABEL";
		exit(1);   // call system to stop
	}
   mn=readI(&labF); nPat=readI(&labF);
   imL= (unsigned char*) malloc(nPat);
   labF.read((char*)(imL), sizeof(char)*nPat);
   labF.close();
   //cout<<mn<<"  "<<nPat<<endl;
   if( num==10){
      //====================all data flat but double 0-9 ====================
      double *  label= (double *) malloc(sizeof(double)*10*nPat);
      for (i=0; i<nPat; i++)
         for (j=0; j<10; j++)
        {
            if (j==imL[i]) label[10*i+j]=1.0;
            else label[10*i+j]=-1.0;
        }

      //As a two dimensional array ====
      nOut=10;   // transformed from an integer from 0 to 9
      y=(double**) malloc(sizeof(double*)*nPat);
      for (i=0; i<nPat; i++) y[i]= label+i*10;
      d.label = label;
      }   // end of 10
   else
      {
      //==================all data flat but double single num ==========
      double *  label= (double *) malloc(sizeof(double)*nPat);
      for (i=0; i<nPat; i++){
            if (imL[i]==num) label[i]=1.0;
            else label[i]=-1.0;
         }

     //As a two dimensional array ====
      nOut=1;   // transformed from to single
      y=(double**) malloc(sizeof(double*)*nPat);
      for (i=0; i<nPat; i++) y[i]= label+i;
      d.label = label;
      }

   delete []imL;
   //never do "delete [] label;"   it is the only place with pattern data
}
///////////////////////////////display image from mnist//////////////////
 void trainSet::displayIm(int pI )
{
   int i,j;
    for( i=0; i<row; i++){
         for( j=0; j<column; j++){
               if (i==0&&j==0)   {cout<<'.'; continue;}
               if (i==row&&j==column) {cout<<'.'; continue;}
            if(x[pI][j+column*i]<-.6 ) cout<<char(32)<<char(32);
            else if(x[pI][row*i+j]<-.2) cout<<char(176)<<char(176);
            else if(x[pI][row*i+j]<.2) cout<<char(177)<<char(177);
            else if(x[pI][row*i+j]<.56) cout<<char(178)<<char(178);
            else cout<<char(219)<<char(219);
         }
         cout<<endl;
      }

      for( i=0; i<10; i++)
      {
            if(y[pI%nPat][i]<=0 ) cout<<char(176)<<char(176);
            else cout<<char(219)<<char(219);
      }
      cout<<endl;
      for( i=0; i<10; i++){
            if(y[pI%nPat][i]<=0 ) cout<<"  ";
            else cout<<i<<' ';
      }
      cout<<endl<<"=================================================================="<<endl;
}
///////////////////////////////to load mnist//////////////////
void trainSet::loadMnist_x()
{
    char IMFname[]="Data\\train-images.idx3-ubyte";
    readIm   (IMFname);
}
/////////////////////////////////////////////////////////////////////////
void trainSet::loadMnist_y(int num)
{
    char LFname []="Data\\train-labels.idx1-ubyte";
    readLABEL(LFname,num); // num =10 means all 10 outputs
}
/////////////////////////////////////////////////////////////////////////
void trainSet::loadtest_x()
{
    char IMFname[]="Data\\t10k-images.idx3-ubyte";
    readIm   (IMFname);
}
/////////////////////////////////////////////////////////////////////////
void trainSet::loadtest_y(int num)
{
    char LFname []="Data\\t10k-labels.idx1-ubyte";
    readLABEL(LFname,num);
}
/////////////////////////////////////////////////////////////////////////
void trainSet::avgpooling()
{
    int j,k,m,i=0;
    double** temp;
    temp =(double**) malloc(sizeof(double*)*nPat*Xsize);
    for (m=0; m<nPat*Xsize; m++)
    {
        i=0;
        temp[m] = new double [nIn/4];
        for (j=0; j<row; j+=2)
        {
            for (k=0; k<column; k+=2)
                temp[m][i++] = (x[m][row*j + k] + x[m][row*j + k+1] + x[m][row*j+row + k] + x[m][row*j+row + k+1])/4.0;
        }
    }
    if (shear)  toggleset();
    delete [] d.pat;
    if (Xsize>1 ) for (int m=nPat; m<nPat*Xsize; m++) delete x[m];
    d.pat=NULL;
    x = temp;
    row /=2;
    column /=2;
    nIn = nIn/4;
}
/////////////////////////////////////////////////////////////////////////
void trainSet::maxpooling()
{
    int j,k,m,i=0;
    double** temp;
    temp =(double**) malloc(sizeof(double*)*nPat);
    for (m=0; m<nPat; m++)
    {
        i=0;
        temp[m] = new double [nIn/4];
        for (j=0; j<28; j+=2)
        {
            for (k=0; k<28; k+=2)
            {
                if(x[m][28*j + k]<x[m][28*j + k+1])
                    x[m][28*j + k]= x[m][28*j + k+1];

                if(x[m][28*j + k]<x[m][28*j+28 + k])
                    x[m][28*j + k]=  x[m][28*j+28 + k];

                if(x[m][28*j + k]<x[m][28*j+28 + k+1])
                    x[m][28*j + k]=  x[m][28*j+28 + k+1];

                temp[m][i++]=  x[m][28*j + k] ;
            }
        }
    }
    x = temp;
    nIn = nIn/4;
}
/////////////////////////////////////////////////////////////////////////
void trainSet::shearIm(float lamdax, float lamday) /**DON'T USE IT MORE THAN ONCE DURING THE PROGRAM LIFE CYCLE !**/
{
    int j,k,m,i=0;
    double** temp;
    double tx,ty;
    temp =(double**) malloc(sizeof(double*)*nPat);
    for (m=0; m<nPat; m++)
    {
        i=0;
        temp[m] = new double [nIn];
        for (j=0; j<row; j++)
        {
            for (k=0; k<column; k++)
                {
                    tx =  k + j*lamday -(column/(2/lamday)) ;
                    ty =  k*lamdax + j -(row/(2/lamdax));

                    if ( !( (tx>=row) || (tx<0) || (ty>=column) || (ty<0) || (floor(ty)-ty !=0)|| (floor(tx)-tx !=0) ) )
                    temp[m][(int)(row*ty +tx)]= x[m][row*j + k];
                }
        }
    }
    shear = true;
    backup = x;
    x = temp;
}
/////////////////////////////////////////////////////////////////////////
void trainSet::toggleset()
{
    if (backup != NULL) // make sure there is new data before toggle
    {
        shear = !shear;
        double** temp;
        temp = x;
        x = backup;
        backup = temp;
    }
}
/////////////////////////////////////////////////////////////////////////
void trainSet::expandset()
{
    int m;
    Xsize = 3;  // the new set will be X3 of the original
    double** tempx = (double**) malloc(sizeof(double*)*nPat*Xsize);

    for (m=0; m<nPat; m++)
        tempx[m] = x[m];

    shearIm(0,1);

    for (m=0; m<nPat; m++)
        tempx[m+nPat] = x[m];

    toggleset();

    shearIm(1,0);
    for (m=0; m<nPat; m++)
        tempx[m+(nPat*2)] = x[m];

    toggleset();
    backup=NULL;
    x = tempx;
}
/////////////////////////////////////////////////////////////////////////
