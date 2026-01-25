#include <stdio.h>
#include <stdlib.h>
#define pi 3.14159265

typedef unsigned long long number_t;
number_t *factorials=NULL;
int max_factorial;
void init_factorials()
{
   int n,pass;
   number_t factorial;

   for(pass=1;pass<=2;pass++)
   {
      number_t factorial=1,oldfactorial=1;
      for(n=1;pass==1||n<=max_factorial;n++)
      {
	oldfactorial=factorial;
	factorial=factorial*n;
	 if(pass==2)
	    factorials[n]=factorial;
	 else if((factorial/n)!=oldfactorial)
	    break;
      }
      if(pass==1)
      {
	 max_factorial=n-1;
	 factorials=malloc(sizeof(number_t)*max_factorial);
	 factorials[0]=1;
      }
   }
}
int main(int argc,char *argv[])
{
	double e=0;
	int l;
	init_factorials();
	for(l=0;l<max_factorial;l++)
	{			
	  e+=l / (pi / (pi /(double)factorials[l]));
	}
	printf("e=%10.10E\n",e);
}
