#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "solve.h"

void lsqr(int itmax, int itprint) {
  float b1, beta, alfa, rhobar, phibar;
  float a,b,rho,phi,s,t1,t2,c,teta,r;
  int iter,i,j;
  FILE *fp;
  char file[120],name[50];
 
  for(i=0;i<n;i++)
    v[i]=x[i]=0.0f;
  beta=normlz(u,m);
  printf("First normlz finished\n");
  printf("beta= %8.4f\n",beta);
  b1=beta; 
  atupv(v,u);
 
  alfa=normlz(v,n);
  printf("alfa=%8.4f\n",alfa);
  rhobar=alfa; 
  phibar=beta; 
  for(i=0;i<n;i++)
    w[i]=v[i];
  for(iter=0;iter<=itmax;iter++) { 
    a=-alfa;
    
    for(i=0;i<m;i++)
      u[i]=a*u[i];
    avpu(v,u);

    beta=normlz(u,m); 

    b=-beta; 
    for(i=0;i<n;i++)
      v[i]=b*v[i];

    atupv(v,u);
    alfa=normlz(v,n);

    rho=sqrt(rhobar*rhobar+beta*beta);
    //printf("rho=%8.4f, alfa= %8.4f, beta=%8.4f\n",rho,alfa,beta);
    c=rhobar/rho;
    s=beta/rho; 
    teta=s*alfa; 
    rhobar=-c*alfa;
    phi=c*phibar; 
    phibar=s*phibar; 
    t1=phi/rho; 
    t2=-teta/rho;
    for(i=0;i<n;i++) {
      x[i]=t1*w[i]+x[i];
      w[i]=t2*w[i]+v[i];
    }
    r=phibar/b1;
    printf("%8d %8.4f %8.4f %8.4f\n",iter, phibar, r, fabs(phibar*alfa*c));
    if(((fmod(iter,itmax)==0) | (fmod(iter,itprint)==0)) & (iter > 0)) {
      *strcpy(name,"xyyy.");
      name[1]=floor(iter/100)+'0';
      name[2]=fmod(floor(iter/10),10)+'0';
      name[3]=fmod(iter,10)+'0';
      strcat(name,filend);
      *strcpy(file,getenv("MODELDIR"));
      *strcat(file,name);
      
      for(j=0;j<n;j++) {
	xx[j]=x[j]*ww[j];
      }
 
      fp=fopen(file,"wb");
      fwrite(xx,sizeof(float),n,fp);
      fclose(fp);
      printf("Wrote ");
      printf(file);
      printf("\n");
    }
  }
  printf("Finishing lsqr\n");
  
  return;
}

