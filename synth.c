#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "solve.h"

void synth(float u[]) {
  FILE *fp;
  int j,i,Lold,mrec,nn;
  int lastrec,prmulout,correctdata;
  int addnoise;
  float uu,nfactor;
  struct stat fileinfo;
  char envname[10];

  sscanf(getenv("NFACTOR"), "%f" ,&nfactor);    /* add noise to synthetic data */
  sscanf(getenv("ADDNOISE"), "%d" ,&addnoise);  /* add noise to synthetic data */
 
  /* Use an input model to correct data (right hand side) */ 
  sscanf(getenv("CORRECTDATA"),  "%d",  &correctdata);   

  /* Last matr. before damping matrices, we need to know this because else we multiply
     the damping matrices with a model resulting in nonzero right hand side for the damping
     part */
  sscanf(getenv("LASTREC"),  "%d", &lastrec);      

  mrec=minc[lastrec+1];

  nn=checkfile(getenv("SYNTHFILE"))/sizeof(float);
  printf("n= %d, nn= %d\n",n,nn);
  if (nn<n) {
    printf("WARNING, modelfile for creating synth data seems to be to short\n");
    printf("WARNING, WARNING patching up with zeros\n");
    for(i=nn;i<n;i++) {
      x[i]=0;
    }
  }
  else if (nn>n) {
    printf("WARNING, modelfile for creating synth data seems to be to be too long\n"); 
    printf("WARNING, WARNING possibly an incompatible model\n");   
    nn=n;
  }
 
  printf("Reading file %s \n",getenv("SYNTHFILE"));
  fp = fopen(getenv("SYNTHFILE"),"r");
  fread(x,sizeof(float),nn,fp);
  fclose(fp);
  
  if (correctdata>1) {
    for(j=2;j<(correctdata+1);j++) {
      strcpy(envname,"SYNTHFILExx");
      envname[9]=floor(j/10)+'0';  
      envname[10]=fmod(j,10)+'0';
      printf("envname %s \n", envname);
      nn=checkfile(getenv(envname))/sizeof(float);
      printf("n= %d, nn= %d \n",n,nn);    
      if (nn<n) {
	printf("WARNING, modelfile for creating synth data seems to be to short\n");
	printf("WARNING, WARNING patching up with zeros\n");
	for(i=nn;i<n;i++) {
	  xx[i]=0;
	}
      }
      else if (nn>n) {
	printf("WARNING, modelfile for creating synth data seems to be to be too long\n"); 
	printf("WARNING, WARNING possibly an incompatible model\n");   
	nn=n;
      }
      printf("Nr %d of %d, Reading file %s \n",j,correctdata,getenv(envname));
      fp = fopen(getenv(envname),"r");
      fread(xx,sizeof(float),nn,fp);
      fclose(fp);
      for(i=0;i<n;i++) {
	x[i]=x[i]+xx[i];
      }
    }
  }
  printf("Got model, now I am multiplying\n");

  
  Lold=0;
  if (correctdata>0) {
    printf("Correcting Data with synthetic file\n");
    for(i=0;i<mrec;i++){
      for(j=Lold;j<L[i];j++){
	u[i]=u[i]-S[j]*x[C[j]-1];
      }
      Lold=L[i];
    }
  }
  else {
    printf("Replacing Data with synthetic file\n");
    if (addnoise>0){
      srand( time(NULL) );
      printf(" add %14.4f noise \n", nfactor);}
    
    for(i=0;i<mrec;i++){
      u[i]=0.0f;
      
      for(j=Lold;j<L[i];j++){
        u[i]=u[i]+S[j]*x[C[j]-1];
/*        if (u[i]>0){
           printf("Found large u=%5.4f at i=%d,C[j]=%d, x=%5.4f, \n",u[i],i,C[j],x[C[j]-1]);} */
        }
       	
      if (addnoise>0) {
                 uu=randnorm();
	         u[i]=u[i]*1+nfactor*1.0344*uu;
		    }	  
      Lold=L[i];
    }
  }

  printf("Finished multiplying\n");
  
 
  sscanf(getenv("PRINTMULOUT"),      "%d",  &prmulout); 
  if (prmulout>0) {
    printf("Writing multiplication to %s\n",getenv("MULOUTFILE"));
    fp = fopen(getenv("MULOUTFILE"),"w");
    fwrite(u,sizeof(float),mrec,fp);
    fclose(fp);
  }
  return;
}
