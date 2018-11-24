#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "solve.h"

main() {
  FILE *fp;
  char i,dataok;
  int matnr,maxiter,iterprint,info[2],Lstart,fi;
  char file[200], variable[30];
  float scale;
  struct stat fileinfo;


  /* Getting basic info */
  
  sscanf(getenv("MATNR"),      "%d",  &matnr);        /* #nr of matrices (1 means 00 and 01) */ 
  sscanf(getenv("MAXITER"),    "%d",  &maxiter);      /* number of iterations */
  sscanf(getenv("ITERPRINT"),  "%d",  &iterprint);    /* Interval between writing iter out */
  sscanf(getenv("DELETE"),     "%d",  &delet);        /* Decides if synthetic data is made */
  sscanf(getenv("MKUNWEIGHT"), "%d",  &mkunweight);   /* If to make unweight or not  */
  sscanf(getenv("DOLSQR"), "%d",      &dolsqr);       /* If to run the iterationst  */
  filend=getenv("FILEND");                            /* The file ending for the run */
  
  m=0; 
  l=0;
  minc=(int *) malloc((matnr+1)*sizeof(int)); 
  linc=(int *) malloc((matnr+1)*sizeof(int)); 
  minc[0]=0;
  linc[0]=0;

  for (i=0;i<matnr;i++) { 
    strcpy(variable,"MATPATHxx"); 
    
    variable[7]=floor(i/10)+'0'; 
    variable[8]=fmod(i,10)+'0';
    
    if (delet>0) { 
      strcpy(file,getenv(variable)); 
      strcat(file,"linsize.bin");
      printf("Getting info for "); 
      printf(file);
      printf("\n");
      fp=fopen(file,"r");
      fread(info,sizeof(int),2,fp);
      fclose(fp);
 
    }
    else {  
      strcpy(file,getenv(variable)); 
      strcat(file,"L.bin");
      fi=stat(file, &fileinfo);
      info[0]=fileinfo.st_size/sizeof(int);

      strcpy(file,getenv(variable)); 
      strcat(file,"C.bin");
      fi=stat(file, &fileinfo);
      info[1]=fileinfo.st_size/sizeof(int);
    }
    m=m+info[0]; 
    l=l+info[1]; 
    //printf("l= %d,  contribution= %d \n",l,info[1]);
    minc[i+1]=info[0]+minc[i]; 
    linc[i+1]=info[1]+linc[i]; 

  } 

  strcpy(file,getenv("RAYGRIDDIR")); 
  strcat(file,"ncell.bin"); 
  checkfile(file);
  fp=fopen(file,"r"); 
  fread(&n,sizeof(int),1,fp); 
  fclose(fp); 

  printf("Number of matrix lines %d and columns %d\n",m,n);
  printf("Number of matrix entries %d\n",l);
  printf("Memory needed for matrix %5.1f Mbytes\n",(8.0*l+8.0000*m)/(1000000.0));
  
  C=(int *) malloc(l*sizeof(int)); 
  S=(float *) malloc(l*sizeof(float)); 
  L=(int *) malloc(m*sizeof(int)); 
  u=(float *) malloc(m*sizeof(float)); 
  uu=(float *) malloc(m*sizeof(float)); 
  dd=(float *) malloc(m*sizeof(float)); 
  x=(float *) malloc(n*sizeof(float)); 
  xx=(float *) malloc(n*sizeof(float)); 
  v=(float *) malloc(n*sizeof(float)); 
  w=(float *) malloc(n*sizeof(float)); 
  ww=(float *) malloc(n*sizeof(float)); 
  vtmp=(float *) malloc(n*sizeof(float)); 
  if (mkunweight>0) { 
    dN=(int *) malloc(m*sizeof(int)); 
  } 

  Lstart=0;
  for (i=0;i<matnr;i++) {      
    strcpy(variable,"SCALExx");  
    variable[5]=floor(i/10)+'0';  
    variable[6]=fmod(i,10)+'0';
    sscanf(getenv(variable),    "%f", &scale); 

    strcpy(variable,"MATPATHxx");  
    variable[7]=floor(i/10)+'0';  
    variable[8]=fmod(i,10)+'0'; 
    strcpy(file,getenv(variable)); 

    dataok=readdata(file, &S[linc[i]], &C[linc[i]], &L[minc[i]], &u[minc[i]],&dN[minc[i]], Lstart, scale); 
    if (dataok==0) { 
      printf("Data reading failed\n"); 
      return; 
    } 
    Lstart=L[minc[i+1]-1]; 
  } 
   matwork(); 
   printf("Starting Iterations\n");

  if (dolsqr>0) {
    lsqr(maxiter,iterprint); 
  }
  return;
}



