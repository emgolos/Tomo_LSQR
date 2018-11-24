#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "solve.h"

void matwork() { 
  FILE *fp;
  int i;
  int mksynth, mkhitcount, getmaxcol, mkcolsum, mknormcol,findnans,correctdata,rullout3dcrust;
  char file[120];

  sscanf(getenv("MKSYNTH"),    "%d",  &mksynth);               /* Decides if synthetic data is made */
  sscanf(getenv("MKHITCOUNT"), "%d",  &mkhitcount);            /* Makehitcount or not */
  sscanf(getenv("GETMAXCOL"),  "%d",  &getmaxcol);             /* Find limits on column numbers  */
  sscanf(getenv("FINDNANS"),  "%d",   &findnans);              /* Find nans in lines  */
  sscanf(getenv("MKCOLSUM"),   "%d",  &mkcolsum);              /* If to write colsum out or not  */
  sscanf(getenv("MKNORMCOL"),  "%d",  &mknormcol);             /* Normalize columns or not  */
  sscanf(getenv("CORRECTDATA"),  "%d",  &correctdata);         /* Use an input model to correct data (right hand side) */
  sscanf(getenv("RULLOUT3DCRUST"),  "%d",  &rullout3dcrust); 
  
  if ((mksynth>0) ||  (correctdata>0)) {
    printf("Starting synth\n"); 
    synth(u);   
    printf("synth finished\n"); 
  }

  if (rullout3dcrust>0) {
    printf("rull out the 3D crust\n"); 
    rullout3d();   
  }

  if (findnans>0) {
    printf("Searching for nans\n"); 
    findnan();   
  }

  if (mkhitcount>0) {
    printf("Calculating hits\n"); 
    hitcount(); 
  }

  if (getmaxcol>0) {
    printf("Finding maximum values\n");
    maxcol(); 
  }

  if (mkunweight > 0) {
    printf("Starting unweight\n");
    unweight(); 
    printf("Unweight finished, starting damping\n");
  }

  if (mkcolsum>0) {  
    printf("Starting colsum\n");
    colsum(ww); 
    strcpy(file,getenv("MODELDIR"));
    strcat(file,"sumsens.");
    strcat(file,filend);
    printf(file);
    printf("\n");
    fp=fopen(file,"wb"); 
    fwrite(ww,sizeof(float),n,fp);  
    fclose(fp);  
  }  

  if (mknormcol>0) {
    colsum(ww); 
  
     for(i=0;i<n;i++) { 
      ww[i]=1/sqrt(ww[i]);
     }

    printf("Starting normcol\n");
    normcol(ww); 
    printf("Normcol finished\n");
    strcpy(file,getenv("MODELDIR"));
    strcat(file,"w.");
    strcat(file,filend);
    fp=fopen(file,"wb"); 
    fwrite(ww,sizeof(int),n,fp);  
    fclose(fp);  
  }
  else {    
    for(i=0;i<n;i++) { 
      ww[i]=1; 
    }
  }
  return;
}


