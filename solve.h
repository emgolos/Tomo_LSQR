
typedef float myReal;

void avpu(float v[], float u[]);
void atupv(float v[], float u[]); 
void lsqr(int itmax, int itprint);
float normlz(float y[], int l);
float medal(float y[], int l);
char readdata(char *path, float *SS, int *CC, int *LL, float *DD, int *ddN, int Lstart, float scale);
void matwork();
int checkfile(char *file);
void colsum(float v[]);
void normcol(float v[]);
void synth(float u[]);
void ax();
void vari(int jj);
void unweight();
void maxcol();
void hitcount(); 
void findnan();  
void resolution(); 
void multiply();
void rullout3d();
float randnorm();

int m,n,l,*minc,*linc;

float *S, *u,*uu,*dd, *x, *xx;
int  *L, *C, *dN;
float *v, *w,*ww;
float *vtmp;

int mkunweight;
int delet,dolsqr;
float varsi;
char  *filend;

/* char readcolsum; */
