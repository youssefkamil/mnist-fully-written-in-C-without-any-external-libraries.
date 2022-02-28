#include "matrix.h"
#include <iostream>
using namespace std;



    //------------------------------------------------------
   matrix::matrix (int colum):r(1),c(colum)
    {
        m=new double*[r];
        for(int i=0; i<r; i++)
            m[i]=new double[c];
    }
    //-------------------------------------------------------
     matrix::matrix (int row,int colum):r(row),c(colum)
    {
        m=new double *[r];
        for(int i=0; i<r; i++)
            m[i]=new double[c];
    }
    //-------------------------------------------------------
     matrix::~matrix ()
    {
        for(int i=0; i<r; i++)
            delete [] m[i];
        delete [] m ;
    }
    //----------------------------------------------------------
    void  matrix::Emult (matrix* x,matrix* z) //check
    {
        if (z->r!=x->r || z->c!=x->c)
        {
            cout<<"Not the same"<<endl;
        }
        else
        {
            for(int n=0; n<r; n++)
                for(int j=0; j<c; j++)
                   m[n][j]=z->m[n][j]*x->m[n][j];
        }
    }
    //--------------------------------------------------------------
    void  matrix::mult(matrix* x,matrix* z)//check
    {
        if(x->c!=z->r)
        {
            cout<<"Multipli Error"<< x->c << ", " << z->r<<endl;
        }
        if (x== this|| z== this)
        {
            cout<<"Multipli Error"<< "resusing matrixes is prohebted" <<endl;
        }

        for(int n=0; n<r; n++)
        {
            for(int j=0; j<c; j++)
            {
                m[n][j]=0;
                for(int y=0; y<x->c; y++)
                    m[n][j]+=x->m[n][y]*z->m[y][j];
            }
        }
    }
    //------------------------------------------------
    void  matrix::add (matrix* x,matrix* z)//check
    {
        if(x->r!=z->r&&x->c!=z->c)
        {
            cout<<"Error input"<<endl;
        }
        else
        {
            for(int n=0; n<r; n++)
                for(int j=0; j<c; j++)
                    m[n][j]=x->m[n][j]+z->m[n][j];
        }
    }
    //-----------------------------------------------------
    void  matrix::sub(matrix* x,matrix* z)//check
    {
        if(x->r!=z->r&&x->c!=z->c)
        {
            cout<<"Error input"<<endl;
        }

        else
        {
            for(int n=0; n<r; n++)
                for(int j=0; j<c; j++)
                    m[n][j]=x->m[n][j]-z->m[n][j];
        }
    }
    //-------------------------------------------------------
    void  matrix::sub(matrix* x)//check
    {
        if(r!=x->r||c!=x->c)
        {
            cout<<"Error"<<endl;
        }
        else
            for(int n=0; n<r; n++)
                for(int j=0; j<c; j++)
                    m[n][j]=m[n][j]-x->m[n][j];
    }
    //-------------------------------------------------------
    matrix*  matrix::operator =(matrix* x)//check
    {
        matrix * rus = new matrix(x->r,x->c);
        for(int j=0;j<r;j++)
            for(int n=0;n<c;n++)
                m[j][n]=x->m[j][n];
        return rus;
    }
    //---------------------------
    void  matrix::eqaul(matrix* x)//check
    {
        if(r!=x->r||c!=x->c)
        {
            cout<<"Error"<<endl;
        }
        else
        for(int j=0;j<r;j++)
            for(int n=0;n<c;n++)
                m[j][n]=x->m[j][n];
    }
    //---------------------------------------------
    void  matrix::fil()//check
    {
        for(int j=0;j<r;j++)
        {
            for(int n=0;n<c;n++)
            {
                cout<<"value of ["<<j<<"]["<<n<<"]=?";
                cin>>m[j][n];
            }
        }
    }
    //------------------------------------------
    void  matrix::print(char * str)//check
    {
        cout<<"the value off "<<str<<endl;
        for(int j=0;j<r;j++)
        {
            for(int n=0;n<c;n++)
                cout<<m[j][n]<<" , ";//cout<<"["<<j<<"]["<<n<<"]="<<m[j][n]<<" , ";
            cout<<endl;
        }
    }
    //----------------------------------------
    void  matrix::print2mat(matrix* x)//check
    {
        cout<<"The of value of matrix"<<endl;
        for(int i=0;i<c;i++)
        {
            for(int j=0;j<r;j++)
                cout<<setw(3)<<m[j][i]<<" ";//<<setw(6)
            cout<<"="<<setw(9)<<x->m[0][i];
            cout<<endl;
        }
    }
    //---------------------------------------
    void  matrix::square(matrix* x)//check
    {
        this->mult(x,x);
    }
      //---------------------------------------
    void  matrix::Esquare(matrix* x)//check
    {
       Emult(x,x);
    }
    //--------------------------------------------
    void  matrix::transpose(matrix* x)//check
    {
        if(x->r!=c || x->c!= r)
        {
            cout<<"transpos: Error input dimention"<<endl;
        }
        for(int j=0;j<r;j++)
        {
            for(int n=0;n<c;n++)
                m[j][n]=x->m[n][j];
        }
    }
    //------------------------------------------------------
    void  matrix::filrandnum(int p) //p for percent //check
    {
       for(int j=0;j<r;j++)
        {
            for(int n=0;n<c;n++)
            {
            double r = rand() % 100;
            if(r>p)
                r=1;
            else
                r=-1;
            m[j][n]=r;
            }
        }
    }
    //----------------------------------------------------------
    void  matrix::filrandfrac() //p for percent//check
    {
        for(int j=0;j<r;j++)
        {
            for(int n=0;n<c;n++)
                m[j][n]=((rand()%100)-50)/200.00;
        }
    }
    //-----------------------------------------------------------------------
    void  matrix::filone()
    {
        for(int j=0;j<r;j++)
            for(int n=0;n<c;n++)
              m[j][n]=1;
    }
    //----------------------------
    void  matrix::sub(int p ,matrix* x)
    {
        int i,j;
        if(c!=x->c||r!=x->r)
        {
            cout<<"Sub: error coming"<<endl;
        }
        for(i=0;i<r;i++)
        {
            for(j=0;j<c;j++)
                m[i][j]=p-(x->m[i][j]);
        }

    }
    //-----------------------------
    double  matrix::sumofmat()//check
    {
        int i,j;
        double sum=0;
        for(i=0;i<r;i++)
        {
            for(j=0;j<c;j++)
               sum+=m[i][j];
        }
        return sum;
    }
    //--------------------------------
    double  matrix::sumofrow (int num)// num number of colum
    {
        int i;
        double sum =0;
        for(i=0;i<c;i++)
            sum+=m[num][i];
        //cout<<"sum of row ="<<sum<<endl; //getch();
        return sum;
    }
    //---------------------------------------------------
    double  matrix::sumofcolum (int num)
    {
        int i;
        double sum =0;
        for(i=0;i<r;i++)
            sum+=m[i][num];
        return sum;
    }
    //--------------------------------
    void  matrix::Ediv(double d, matrix * x)//check
    {
        int i,j;
        if(c!=x->c||r!=x->r)
        {
            cout<<"Sub: error coming"<<endl;
        }
        for(i=0;i<r;i++)
        {
            for(j=0;j<c;j++)
               m[i][j]=x->m[i][j]/d;
        }
    }
    //----------------------------------------
    void  matrix::Emult(double d ,matrix * x)
    {
        if (r!=x->r || c!=x->c)
        {
            cout<<"Not the same"<<endl;
        }
        else
        {
            for(int n=0; n<r; n++)
                for(int j=0; j<c; j++)
                   m[n][j]=d*x->m[n][j];
        }
    }
    //-----------------------------------------------------------
    void  matrix::geta(matrix * x, matrix * w,matrix * b)
    {
        double z=0;
        int i,j,k;
        if(x->r!=w->c||w->r!=b->r||b->c!=1)
            cout<<"Geta : Error input dimention "<<endl;

        if(r!=w->r||r!=b->r||c!=x->c)
            cout<<"Geta : Error output dimention"<<endl;
        else
        {
            for(k=0;k<x->c;k++)
            {
              for(j=0;j<w->r;j++)
                {
                  z=0;
                  for(i=0;i<w->c;i++) z+=w->m[j][i]*x->m[i][k];

                  z+=b->m[j][0];
                  m[j][k]=tanh(z);
                }
            }
        }
    }

    //-----------------------------------
    void  matrix::getdldz2(matrix * y,matrix * a)
    {
        if(c!=y->c||r!=y->r||y->r!=a->r||y->c!=a->c)
            cout<<"Error getdldz"<<endl;
        int i,j;
        for(i=0;i<y->r;i++)
            for(j=0;j<y->c;j++)
                m[i][j]=(y->m[i][j]-a->m[i][j])*(1-(a->m[i][j]*a->m[i][j]));
    }
    //-----------------------------------
double ** matD(int r, int c)
 {
   double ** m=new double* [r];
        for(int i=0; i<r; i++)
            m[i]=new double[c];
    return m;
 }
 //--------------------------------------------
 double * matD(int r)
 {
   double * m=new double [r];
    return m;
 }


     //-----------------------------------
int ** matI(int r, int c)
 {
   int ** m=new int* [r];
        for(int i=0; i<r; i++)
            m[i]=new int[c];
    return m;
 }

 //--------------------------------------------
 int * matI(int r)
 {
   int * m=new int [r];
    return m;
 }

//--------------------------------------------
 double** matDP(int r)
 {
   double** m=new double* [r];
    return m;
 }

//--------------------------------------------

void convMark(double* Input, double* Filters, int InputLength, int FilterLength, double* Out)
{
    /*int OutLength = InputLength - FilterLength + 1;
	int OutSize = OutLength * OutLength;
	int start = 0;
	for (int i = 0; i < OutSize; i++)
	{
		//Out[i] = 0.0;
		for (int row = 0; row < FilterLength; row++)
		{
			for (int col = 0; col < FilterLength; col++)
			{
				Out[i] += Input[start + col + (InputLength*row)] * Filters[col + (row * FilterLength)];
			}
		}
		if (start % InputLength == InputLength - FilterLength) start += FilterLength;
		else start++;
	}*/

    int Findex;      //index for data at each filter
	int Fsize = FilterLength;
   int newMrow=InputLength-FilterLength+1;
   int newnpix= newMrow * newMrow;
   int n;
   double a=0.0;
   int index =1;
   int ss=0;
   for(int s=0;s<newnpix;s++)
   {
       Findex=0;
       if(s==newMrow*index)  {ss=InputLength*index; index++;}
       else if(s!=0) ss++;

       n=ss;

       while(Findex!=Fsize*Fsize)
       {
           a+=Input[n] * Filters[Findex];

           if(((Findex+1)%Fsize)==0)  n+=InputLength-Fsize+1;
           else n++;

           Findex++;
       }
       Out[s]=a;
       a=0.0;
   }
}

void zero(double** zero, int pat, int in)
{
	for (int i = 0; i < pat; i++)
	{
		for (int j = 0; j < in; j++)
		{
			zero[i][j] = 0.0;
		}
	}
}

void print(double* x, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if(x[j+cols*i]<-.6 ) cout<<char(32)<<char(32);
            else if(x[cols*i+j]<-.2) cout<<char(176)<<char(176);
            else if(x[cols*i+j]<.2) cout<<char(177)<<char(177);
            else if(x[cols*i+j]<.56) cout<<char(178)<<char(178);
            else cout<<char(219)<<char(219);
		}
		cout << endl;
	}
}

void copyD(double* source, double* dest, int sizeD)
{
    for(int i = 0; i<sizeD; i++)
    {
        dest[i] = source[i];
    }
}

double *** matD(int a,int b,int c)
 {
    double*** m = new double** [a];
    for(int i=0; i<a; i++)
    {
        m[i]=new double*[b];

        for(int j=0; j<b; j++)
            m[i][j]=new double [c];
    }
    return m;
 }
