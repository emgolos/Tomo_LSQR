#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "solve.h"
#include "time.h"

void resolution() {
  int i,k,kk,Ik,Ii,Lold,Ikold,Iiold,ndof,nrec,nrit;
  float *alpha,inner,outer,tim;
  float *Rdi,*Cdi;
  char file[200],outname[50];
  FILE *fp;

  nrec=m;
  ndof=n;
  nrit=100;
  if (nrit>nrec) {
    nrit=nrec;
  }

  alpha=(float *) malloc(nrec*sizeof(float));
  Rdi=(float *) malloc(ndof*sizeof(float));
  Cdi=(float *) malloc(ndof*sizeof(float));

  kk=0;
  Lold=0;
  Ikold=0;
  clock();
  for (k=0;k<nrit;k++) {
    alpha[k]=0;
    for(i=Lold;i<L[k];i++) {
      alpha[k]=alpha[k]+S[i]*S[i];
    }
    Lold=L[k];

    Iiold=0;
    outer=0;
    for (i=0;i<nrec;i++) {
      Ii=Iiold;
      Ik=Ikold;
      inner=0;
      while (Ii<L[i] & Ik<L[k]) {
	if (C[Ii]==C[Ik]) {
	  inner=inner+S[Ii]*S[Ik];
	  Ii++;
	  Ik++;
	}
	else if (C[Ii]<C[Ik]) {
	  Ii++;
	}
	else {
	  Ik++;
	}
      }
      outer=outer+inner*inner;
      Iiold=L[i];
    }
      
    alpha[k]=alpha[k]/outer;
    printf("%d %E %f\n",k,alpha[k],clock()/1000000.0);
    Ikold=L[k];
  }
  tim=clock()/1000000.0;

  sprintf(outname,"run%dx%dnnz%d.m",nrec,ndof,L[nrec-1]);
  strcpy(file,getenv("MODELDIR"));
  strcat(file,outname);
  fp=fopen(file,"wb");    
  fprintf(fp,"nrec=%d;\nndof=%d;\nnrnz=%d;\ntim=%f;\nnrit=%d;\ntt=%f;\n",nrec,ndof,L[nrec-1],tim,nrit,tim/3600.0*nrec/nrit);
  fclose(fp); 
  printf(file);
  printf("\n");  

  return;
}


