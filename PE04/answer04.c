#include <stdlib.h>
#include <errno.h>
#include "utility.h"
#include "answer04.h"
// do not modify before this line

// recursive implementation of the conversion of
// the magnitude of a given number to a string
// the sign is taken care of by the long_int_to_string function
//

char int_to_char(int number)
{
   char c;
   if (number>=0 && number <= 9)
   {
	c = number + '0';
   }
   else if (number >= 10)
   {
	c = number + 87;
   }
   return(c);
}
void rec_magnitude_long_int_to_string(long int number, int base,
                                      char *intstring, int *stringlen)
{
   // increment the counter for counting the number of recursive calls
   // do not remove this line
   Increment_counter(&number);
   /*if (number < base)
   { 
        intstring[*stringlen] = int_to_char(number);
	*stringlen += 1;
   }*/
     if (number != 0){
	if (number < 0){
	   long int temp1 = 0;
	   temp1 = -1 * (number % base);
	   intstring[*stringlen] = int_to_char(temp1);
	   *stringlen += 1;
	   rec_magnitude_long_int_to_string((number/base), base,intstring,stringlen);
	}
	else{
	   long int temp = 0;
	   temp = number % base;
	   intstring[*stringlen] = int_to_char(temp);
	   *stringlen += 1;
	   rec_magnitude_long_int_to_string((number/base),base,intstring,stringlen);
        }
    }
   //put your code here


   // decrement the counter after all recursive calls and before
   // you exit from this function

   Decrement_counter();
}
char *long_int_to_string(long int number, int base)
{
   // the real function for long_int_to_string


   char intstring[65];  // 65 because the largest length needed is 
                        // for base 2, 1 byte for the negative sign,
                        // 63 bytes for the magnitude, and 1 byte for null char
   int stringlen = 0;

   if ((base < 2) || (base > 36)) {
      errno = EINVAL;
      return NULL;
   }
      // place the number at location 1 of intstring
   rec_magnitude_long_int_to_string(number, base, intstring, &stringlen);
   if (number < 0)
   {
	stringlen += 1;
   }
   // allocate enough space for null character
   char *ret_string = (char *)malloc(sizeof(char) * (stringlen + 1));

   // copy from intstring to ret_string and append '\0'
   int i;
   for (i = 0; i < stringlen; i++) {
	if (number < 0)
	{
	   ret_string[stringlen - i - 1] = intstring[i];
	   ret_string[0] = '-';
	}
	else{
	ret_string[stringlen - i - 1] = intstring[i];
	}   
   }
   ret_string[i] = '\0';
   return ret_string;
}
