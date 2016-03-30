#include "utility.h"
#include "answer05.h"
#include <stdio.h> //take out this after
// do not remove #ifndef and #endif in this file, otherwise the evaluation
// would fail and you would definitely get 0

#ifndef NTEST_LOG

/* n is even */
/* *fn1 and *fn2 should store F(n) and F(n-1) */
void logFibonacci(int n, unsigned long *fn1, unsigned long *fn2)
{
  // increment the recursion counter 

  Increment_counter(&n);

  // terminating condition
	if (n == 0){
		*fn1 = 0;
		*fn2 = 0;
		return;	
	}
	if (n == 2){
		*fn1 = 1;
		*fn2 = 1;
		return;
	}
  // recursion
  // you have to halve n, 
  // depending on whether n/2 is even or odd, recursively
  // call logFibonacci with appropriate parameters
	n /= 2;
	if (n % 2 == 0)
	{
		logFibonacci(n,fn1,fn2);
		unsigned long int temp1 = 0;
		temp1 = (2 * *fn2 * *fn1) + (*fn1 * *fn1);
		unsigned long int temp2 = 0;
		temp2 = (*fn2 * *fn2) + (*fn1 * *fn1);
		*fn1 = temp1;
		*fn2 = temp2;
	}
	else
	{
		logFibonacci(n-1,fn1,fn2);
		unsigned long int fn3 = 0;
		unsigned long int temp3 = 0;
		fn3 = *fn1 + *fn2;
		temp3 = (2 * *fn1 * fn3) + (fn3 * fn3);
		unsigned long int temp4 = 0;
		temp4 = (*fn1 * *fn1) + (fn3 * fn3);
		*fn1 = temp3;
		*fn2 = temp4;
	}
   // decrement counter after all recursive calls
   // and before returning from the function.

   Decrement_counter();
}

#endif

#ifndef NTEST_FIB

/* n is can be odd or even */
/* n is assumed to be non-negative */
/* don't have to check for that */
unsigned long Fibonacci(int n)
{
   /* for storing the results when calling logFibonacci */

   unsigned long fn1 = 0; 
   unsigned long fn2 = 0;

   /* simple cases */
	if (n == 0){
		return(0);
	}
	if (n == 1){
		return(1);
	}
   /* if even n, call logFibonacci with n and return fn1 */
   /* if odd n, call with n-1 or n+1, combine fn1 and fn2 as F(n) */
	if ((n % 2) == 0){
		logFibonacci(n,&fn1,&fn2);
		return(fn1);
	}
	else{
		logFibonacci(n-1,&fn1,&fn2);
		return (fn2 + fn1);  // should change this, doing this so that compiler 
   }                   // would not complain.

}

#endif
