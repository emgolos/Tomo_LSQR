#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "solve.h"

char readdata(char *path, float *SS, int *CC, int *LL, float *DD, int *ddN, int Lstart, float scale) {
  FILE *fp, *Sfp,*Cfp,*Dfp;
  int *Lorg,Lorgold,Lold;
  int *dNorg, dNorgold, dNtmp;
  int i,j,mlen,scaling,kk;
  char filename[120], *lin,linold;

  scaling=(scale!=1);
  if (delet>0) {
    *strcpy(filename,path);
    *strcat(filename,"lin.bin");
    mlen=checkfile(filename)/sizeof(char);
    lin=(char *) malloc(mlen*sizeof(char));  
    fp = fopen(filename,"r");
    fread(lin,sizeof(char),mlen,fp);
    fclose(fp);
  }
  else {    

    *strcpy(filename,path);
    *strcat(filename,"L.bin");
    checkfile(filename);
    mlen=checkfile(filename)/sizeof(int);
    lin=(char *) malloc(mlen*sizeof(char));
    for (j=0;j<mlen;j++) {
      lin[j]=1;
    }
  }

  Lorg=(int *) malloc(mlen*sizeof(int)); 
  *strcpy(filename,path);
  *strcat(filename,"L.bin");
  fp = fopen(filename,"r");
  fread(Lorg,sizeof(int),mlen,fp);
  fclose(fp);  

  if (mkunweight>0) {
    printf("Reading N\n");
    dNorg=(int *) malloc(mlen*sizeof(int)); 
    *strcpy(filename,path);
    *strcat(filename,"N.bin");
    checkfile(filename);
    fp = fopen(filename,"r");
    fread(dNorg,sizeof(int),mlen,fp);
    fclose(fp);
    printf("Reading N finished\n");

    dNorgold=dNorg[0];
    for(j=1;j<mlen;j++) {
      dNtmp=dNorg[j]-dNorgold;
      dNorgold=dNorg[j];
      dNorg[j]=dNtmp;
    }
  }

  *strcpy(filename,path);
  *strcat(filename,"S.bin");
  Sfp = fopen(filename,"r");

  *strcpy(filename,path);
  *strcat(filename,"C.bin");
  Cfp = fopen(filename,"r");

  *strcpy(filename,path);
  *strcat(filename,"D.bin");
  Dfp = fopen(filename,"r");
  Lorgold=0;
  Lold=0;  

  j=0;
  kk=0;
  linold=0;
  for (i=0;i<mlen;i++) {
	  //printf("i=%d \n",i);
    if (lin[i]>0) {
      LL[j]=Lorg[i]-Lorgold+Lold;
      if (mkunweight > 0) {
	ddN[j]=dNorg[i];
      }
      if (linold<1) { 
	fseek(Sfp,Lorgold*sizeof(float),SEEK_SET);
	fseek(Cfp,Lorgold*sizeof(int),SEEK_SET);
	fseek(Dfp,i*sizeof(float),SEEK_SET);
      } 
      fread(&SS[Lold],sizeof(float),LL[j]-Lold,Sfp);
      fread(&CC[Lold],sizeof(int),LL[j]-Lold,Cfp);
      fread(&DD[j],sizeof(float),1,Dfp);

      Lold=LL[j];
      j++;
    }
  
    Lorgold=Lorg[i];
    linold=lin[i];
    /* if (i>=(kk*mlen/10.0)) {
      printf("%2.0f percent of matrix read\n",100*(i*1.00)/mlen);
      kk++;
    } */
  }
  printf("i=%d, j=%d L[%d]=%d\n",i,j,j-1,L[j-1]);

  fclose(Sfp);
  fclose(Cfp);

  if (scaling) {
    printf("Scaling with %10.5f\n",scale);
    for (i=0;i<LL[j-1];i++) {
      SS[i]=SS[i]*scale;
    }
    for (i=0;i<j;i++) {
      DD[i]=DD[i]*scale;
    }
  }

  for (i=0;i<j;i++) {
    LL[i]=LL[i]+Lstart;
  }

  free(Lorg); 
  free(lin);
  return 1; 
}


