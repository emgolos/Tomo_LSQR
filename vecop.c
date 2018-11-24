#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "solve.h"

void avpu(float v[], float u[])
{
int j,i,Lold;
float utmp;

Lold=0;
for(i=0;i<m;i++){
   utmp=0.0f;
   for(j=Lold;j<L[i];j++){
      utmp=utmp+S[j]*v[C[j]-1];
   }
   Lold=L[i];
   u[i]=u[i]+utmp;   
}
return;
}

void atupv(float v[], float u[])
{
int j,i,Lold;

for(i=0;i<n;i++)
   vtmp[i]=0.0f;

Lold=0;
for(i=0;i<m;i++){
   for(j=Lold;j<L[i];j++){
      vtmp[C[j]-1]=vtmp[C[j]-1]+S[j]*u[i];
      //if isnan(S[j]) {
		//printf("nan found for S at row i=%d, cell C=%d\n",i,C[j]);
  //}
   }
   Lold=L[i];   
}

for(i=0;i<n;i++)
   v[i]=v[i]+vtmp[i];
return;
}


float normlz(float y[], int ll)
{
float s, ss;
int  i;

s=0.0f;
//printf("Starting normlz\n");
for(i=0;i<ll;i++) {
  s=s+y[i]*y[i];
  if isnan(y[i]) {
		//printf("nan found for T at cell i=%d, y=%5.4f \n",i,y[i]);
		//break;
  }
}
//printf("s= %5.4f\n",s);
ss=1/sqrt(s);
//printf("ss= %8.4f\n",ss);
for(i=0;i<ll;i++)
  y[i]=y[i]*ss;
return sqrt(s);
  }


float medal(float y[], int ll)
{
float s;
int  i;

s=0.0f;
for(i=0;i<ll;i++)
  s=s+y[i];
return s/ll;
  }


void colsum(float ww[]) {
  int j,i,Lold,kk;
  kk=0;
  for(i=0;i<n;i++)
    ww[i]=0.0f;
  Lold=0;
  for(i=0;i<m;i++) {
    for(j=Lold;j<L[i];j++){
      ww[C[j]-1]=ww[C[j]-1]+fabs(S[j]);
    }
    Lold=L[i];   
    /* if (i>=(kk*m/10.0)) {
      printf("%2.0f percent of colsum done\n",100*(i*1.00)/m);
      // printf("Lold = %d, C[Lold]= %d\n",Lold,C[Lold]);
      kk++;
    } */
  }
  return;
}


void normcol(float ww[]) {
  int j,i,Lold,kk;
  kk = 0;

  Lold=0;
  for(i=0;i<m;i++) {
    for(j=Lold;j<L[i];j++) {
      S[j]=ww[C[j]-1]*S[j];
    }
    Lold=L[i];  
 
    /* if (i>=(kk*m/10.0)) {
      printf("%2.0f percent of normcol done\n",100*(i*1.00)/m);
      kk++;
    } */
  }
  return;
}

void ax()
{
  int j,i,Lold;
 
  Lold=0;
  for(i=0;i<m;i++){
    dd[i]=0.0f;
    for(j=Lold;j<L[i];j++){
      dd[i]=dd[i]+S[j]*x[C[j]-1];
    }
    Lold=L[i];
  }
  return;
}

void unweight() {
  int j,i,Lold,kk,unweightlim;
  float scal;
  kk=0;
  Lold=0;
  sscanf(getenv("UNWEIGHTLIM"),  "%d",  &unweightlim);
  for(i=0;i<m;i++) {
    if(dN[i]>unweightlim) 
      {
	scal=sqrt(unweightlim/dN[i]);
	u[i]=u[i]*scal; 
	for(j=Lold;j<L[i];j++) {
	  S[j]=S[j]*scal;
	}
      }
    Lold=L[i];  
 
    /* if (i>=(kk*m/10.0)) {
      printf("%2.0f percent of unweight done\n",100*(i*1.00)/m);
      kk++;
    }  */   
  }
  return;
}


void maxcol() {
  int j,i,Lold,kk;
  int  *maxC;
  int  *minC; 
  char file[200];

  FILE *fp;
  kk=1;
  maxC=(int *) malloc(m*sizeof(int));
  minC=(int *) malloc(m*sizeof(int)); 
  Lold=0;

  for(i=0;i<m;i++) {
    maxC[i]=0;
    minC[i]=500000;
    for(j=Lold;j<L[i];j++) {
      if (maxC[i] < C[j]) {
	maxC[i]=C[j];
      }
      if (minC[i] > C[j]) {
	minC[i]=C[j];
      }
    }
  
    Lold=L[i];  
 
    if (i>=(kk*m/10.0)) {
      printf("%2.0f percent of find max done\n",100*(i*1.00)/m);
      kk++;
    }     
  }
  strcpy(file,getenv("MODELDIR"));
  strcat(file,"maxC.");
  strcat(file,filend);
  fp=fopen(file,"wb");     
  fwrite(maxC,sizeof(int),m,fp);      
  fclose(fp);
 
  strcpy(file,getenv("MODELDIR"));
  strcat(file,"minC.");
  strcat(file,filend);
  fp=fopen(file,"wb");    
  fwrite(minC,sizeof(int),m,fp);      
  fclose(fp);  
  return;
}



void hitcount() {
  int j,i;
  int  *hits;
  char file[200];
  FILE *fp;
  hits=(int *) malloc(n*sizeof(int));
  
  for(i=0;i<n;i++) {
    hits[i]=0;
  }
  

  for(i=0;i<L[m-1];i++) {
    hits[C[i]-1]=hits[C[i]-1]+1;
  }

  strcpy(file,getenv("MODELDIR"));
  strcat(file,"hits.");
  strcat(file,filend);
  fp=fopen(file,"wb");  
  fwrite(hits,sizeof(int),n,fp);      
  fclose(fp);
  return;
}

void findnan() {
  FILE *fp;
  int i,j;
  int fi;
  j=0;
  fp = fopen("Snans.bin","w");
  fclose(fp);
  fp = fopen("Snans.bin","a+");
  for (i=0;i<L[m-1];i++) {
    if (isnan(S[i])) {
      fwrite(&i,sizeof(int),1,fp);
      j++;
    }
    if (fmod(i,500000)==0) {
      printf("Finished %d %3.1f%%\n",i,1.00*i/L[m-1]*100);
    }
  }  
  fclose(fp);
  printf("Total number of nans is %d\n",j);
return;
}

void rullout3d() {
   FILE *fp;
   int i;
   int lastrec, mrec;
   float *ud;
   
   sscanf(getenv("LASTREC"),  "%d", &lastrec);      
   mrec=minc[lastrec+1];
   
   ud=(float *)malloc(mrec*sizeof(float));
   fp = fopen(getenv("MULOUTFILE"),"r");
   checkfile(getenv("MULOUTFILE"));
   printf("Reading data  from %s\n",getenv("MULOUTFILE"));
   fread(ud,sizeof(float),mrec,fp);
   fclose(fp);
   for(i=0;i<mrec;i++) {
     u[i]=ud[i];
   }
  return;
}
