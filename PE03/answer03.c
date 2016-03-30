#include <errno.h>
#include <ctype.h>
#include <limits.h>

#include "answer03.h"


int char_to_int(char c)
{
   int digit = 0;
   if (isdigit(c))
   {
      digit = c - '0';
      return(digit);
   }
   else if (islower(c))
   {
      digit = c - 87;
      return(digit);
   }
   else if(isupper(c))
   {
      digit = c - 55;
      return(digit);
   }
   else 
   {
      return 36;
   }
}
// you are not allowed to change the contents at the location whose address in 
// stored in nptr (the way to interpret the type of nptr is that it is an address
// pointing to a constant char.)
//
// you have to check for whether the base in the valid range 2 through 36
// if not, set errno to EINVAL
// 
// This is a suggestion on how you should process the string
// (1)  skip over all white space
// (2)  if a +/- sign is encountered, set the polarity of the number accordingly
//      By default, the number is assumed to be positive if no sign is given.
// (3)  Convert the string until you encounter an invalid symbol for that 
//      base or when you reach the end of the string.
//
// examples (not exhaustive, of course):
//    "10a:" in base 10 is 10, in base 16 is 266, in base 36 is 1306
//    "   -10a:" in base 10 is -10, in base 16 is -266, and in base 36 is -1306 
//    " +10a:" in base 10 is 10, in base 16 is 266, in base 36 is 1sh-4.1306
//    "922337203685477580710304" in base 10 should return LONG_MAX, and errno 
//    should be set to ERANGE.
//    "-922337203685477580710304" in base 10 should return LONG_MIN, and errno 
//    should be set to ERANGE.
//    "garbage" should be 0 in base 10, 0 in base 16, and 35479055534 in base 36
//
long int str_to_long_int(const char *nptr, int base)
{
   long int ret_value = 0;  // return value
   long int count = 0;
   int negative = 0;
   int first = 0;
   if (base > 36 || base < 2)
   {
      errno = EINVAL;
      return ret_value;
   }
                            // if no conversion happens, return 0

   // if invalid base, set errno and return immediately
      while(isspace(*nptr)){
	nptr++;
      }
      if (*nptr == '+' || *nptr == '-')
      {
	negative = (*nptr == '-');
	nptr++;
      }
	while (*nptr != '\0')
        {
	   if (negative == 0){
	      count = char_to_int(*nptr);
	      if (count <= (base - 1)){
		 if ((LONG_MAX / base) >= ret_value){
		    ret_value *= base;
		    if ((LONG_MAX - count) >= ret_value){
	            	ret_value += count;
		    }
		    else{ 
		    	ret_value = LONG_MAX;
			errno = ERANGE;
		   	return(LONG_MAX);
		    }
		 }
		 else{
		     ret_value = LONG_MAX;
		     errno = ERANGE;
		     return(LONG_MAX);
		 }
	      }
	      else
	      {
		  return(ret_value);
	      }
	      nptr++;
	   }
	  else{
	    count = char_to_int(*nptr);
	    if (count <= (base - 1)){
		if((LONG_MIN / base) <= ret_value){
		  if (first == 1){
		     ret_value *= base;
		  }
		  else{
		     ret_value *= -base;
		  }
	          if ((LONG_MIN + count <= ret_value)){
		      ret_value -= count;
	          }
		  else{
		      ret_value = LONG_MIN;
		      errno = ERANGE;
		      return(LONG_MIN);
		  }
		}
	        else{
		    ret_value = LONG_MIN;
		    errno = ERANGE;
		    return(LONG_MIN);
		}
	    } 
	    else{
		return(ret_value);
	    }
	    nptr++;
	    first = 1;
	}
	}
       

   // valid base, skip over white space



   // if encounter a sign, expect a number after that 
   // if a negative sign is encountered, the conversion must
   // produce a negative number








   // now, convert the string to number
   // the conversion has to account for the +/- sign
   // need to check whether you have out-of-range ret_value 





















   // clean up before returning
   // need to set errno and ret_value correctly if the converted number 
   // is out of range








  return ret_value;
}
