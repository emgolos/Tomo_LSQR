/* rand example */
#include <stdio.h>
#include <math.h>
#include <time.h>

float randnorm() {
	float x1, x2, w, y1, y2, num;
 
         do {
         	srand( (unsigned)time(NULL) );num=rand()%10000; num=num/10000;
                 x1 = 2.0 * num - 1.0;
                srand( (unsigned)time(NULL) );num=rand()%10000; num=num/10000;
                 x2 = 2.0 * num - 1.0;
                 w = x1 * x1 + x2 * x2;
         } while ( w >= 1.0 );

         w = sqrt( (-2.0 * log( w ) ) / w );
         y1 = x1 * w;
         y2 = x2 * w;
	 
	 return(y1);
}
 
	
