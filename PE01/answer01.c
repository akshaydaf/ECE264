#include <stdlib.h>
#include "answer01.h"

/* Return the largest partial sum of the array */
/* int array[] = { 1, 4, -1, 6, -5, 4} */
/* the ith partial sum is the sum from array[0] through array[i] */
/* the partial sums are 1, 5, 4, 10, 5, 9 */
/* the largest partial sum of the array is 10 */
/* if the len is 0, the largest partial sum is 0 */
/* you may assume that array is NULL when len is 0 */
/* but is non-NULL and correct when len is > 0 */
/* you may also assume that none of the partial sums will cause an overflow */

int largest_partial_sum(int * array, int len)
{
   // modify the function
   int sum = 0;
   int temp = 0; 
   int i;
   if (len == 0)
   {
     return(0);
   }
   for (i = 0; i < len; i++)
   {
     if(i == 0)
     {
       sum = array[i];
     }
     temp += array[i];
     if (sum <= temp)
     {
	sum = temp;
     } 
   }
   return (sum);
}

/* Return the largest difference of the array */
/* int array[] = { 1, 4, -1, 6, -5, 4} */
/* the largest difference is 6 - (-5) = 11 */
/* if the len is 0, the largest difference is 0 */
/* if the len is 1, the largest difference is also 0 */
/* you may assume that array is NULL when len is 0 */
/* but is non-NULL and correct when len is > 0 */
/* you may assume that the largest difference will not cause an overflow */

int largest_difference(int * array, int len)
{
   int min = 0;
   int max = 0;
   int i = 0;
   if (len == 0)
   {
     return(0);
   }
   for (i = 0; i < len; i++)
   {
     if (i == 0)
     {
	max = array[i];
	min = array[i];
     }
     if (min > array[i])
     {
	min = array[i];
     }
     if (max < array[i])
     {
	max = array[i];
     }
   } 
   return (max - min);
}
