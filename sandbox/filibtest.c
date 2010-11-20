#include<stdio.h>
#include<string.h>
#include"fi_lib.h"     /* use library fi_lib */

int henon(const double &x1, const double &x2, const double &y1, const double &y2)
{
  interval x;

  // ...
    
  printf("\n");
  printf("Computation of the sine function in ANSI-C with fi_lib\n");
  printf("======================================================\n\n");

  printf("Insert an interval argument (e.g. 'x = 1 1' or 'x = 1.01 1.02') \n");
  printf("x = ");
  x = scanInterval();

  printf("Argument x = "); 
  printInterval(x);
  printf("\n");
 
  printf("    sin(x) = "); 
  printInterval( j_sin(x) );
  printf("\n\n"); 

  return 0;
}

