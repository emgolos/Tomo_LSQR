#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "solve.h"

void multiply() {
  int j,i,Lold;
  char file[120],*modfile,*weightfile,*ending, *multip;
  float *w,*x,*d;
  struct stat fileinfo;
  int fi,ncell,nmis,nn,nrec;
  FILE *fp;

  strcpy(file,getenv("RAYGRIDDIR")); 
  strcat(file,"nmis.bin"); 
  fp=fopen(file,"r"); 
  fread(&nmis,sizeof(int),1,fp); 
  fclose(fp);

  ncell=n-nmis;
  nrec=L[m-1];

  modfile=getenv("MODFILE");
  weightfile=getenv("WEIGHTFILE");

  strcpy(file,modfile);  
  fi=stat(file, &fileinfo);
  n=fileinfo.st_size/4;
  if (fi<0) {
    printf("Modelfile seems to be invalid\n");
    return;
  }                
  d=(float *) malloc(m*sizeof(float));
  x=(float *) malloc(n*sizeof(float));
  fp = fopen(file,"r");
  fread(x,sizeof(float),n,fp);
  fclose(fp);

  for(i=ncell;i<n;i++){
    x[i]=0;
  }     
     
  strcpy(file,weightfile);  
  fi=stat(file, &fileinfo);
  nn=fileinfo.st_size/4;
  if ((fi<0) | (nn!=n)) {
    printf("Weightfile seems to be invalid\n");
    return;
  }            
  w=(float *) malloc(n*sizeof(float));
  fp = fopen(file,"r");
  fread(w,sizeof(float),n,fp);
  fclose(fp);

  for(i=0;i<ncell;i++){
    x[i]=x[i]*w[i];
  }   

  printf("nrec=%d mrec=%d n=%d ncell=%d\n",nrec,m,n,ncell);
  Lold=0;
  for(i=0;i<m;i++){
    d[i]=0.0f;
    for(j=Lold;j<L[i];j++){        
       if (C[j]>n) { 
 	printf("%d %d %d %d  %d\n",i,j,n,ncell,C[j]); 
       } 
	d[i]=d[i]+S[j]*x[C[j]-1];
      }
    Lold=L[i];
  }

  printf("Here I have finished ax\n"); ending=getenv("ENDING");
  multip=getenv("OUTDIR");;
  
  strcpy(file,multip);
  strcat(file,"dmul.");
  strcat(file,ending);
  printf(file);
  printf("\n");
  fp = fopen(file,"wb"); 
  fwrite(d,sizeof(float),m,fp); 
  fclose(fp); 

  strcpy(file,multip);
  strcat(file,"dorg.");
  strcat(file,ending);
  printf(file);
  printf("\n");
  fp = fopen(file,"wb"); 
  fwrite(u,sizeof(float),m,fp); 
  fclose(fp);  

  strcpy(file,multip);
  strcat(file,"dN.");
  strcat(file,ending);
  printf(file);
  printf("\n");
  fp = fopen(file,"wb"); 
  fwrite(dN,sizeof(int),m,fp); 
  fclose(fp);   

  return;
}

