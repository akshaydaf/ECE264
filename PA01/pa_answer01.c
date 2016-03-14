#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include "pa_answer01.h"


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
      return(0);
   }
}
// you should try to re-use a majority of the algorithms in PE02 and PE03
// for this programming assignment.
// However, you have to take care of endptr, base 0, and base 16
// these are the three additional things you have to deal with in
// this assignment, as compared to the str_to_long_int function
// that you wrote for PE02 and PE03.
//
// it is really IMPORTANT that you extensively try out the function
// strtol on your own.
//
long int my_strtol(const char *nptr, char **endptr, int base)
{
   long int ret_value = 0;  // return value
   long int count = 0;
   int negative = 0;
   int first = 0;
	int ct = 0;// length of array
	int index = 0;
	const char *sum = 0;
   if (base > 36 || base == 1)
   {
      errno = EINVAL;
      return(ret_value);
   }
		sum = nptr;
		for (ct = 0; nptr[ct] != '\0'; ct++)
		{
		}
      while(isspace(*nptr)){
        nptr++;
      }
      if (*nptr == '+' || *nptr == '-')
      {
		negative = (*nptr == '-');
		nptr++;
      }
		if (base == 0 || base == 16)
		{
			if (*nptr == '0')
			{
				nptr++;
				if (*nptr == 'x' || *nptr == 'X'){
					base = 16;
					nptr++;
					if (*nptr == '\0'){
						nptr--;
						*endptr = (char *) (nptr);
						return(ret_value);
					}
					count = char_to_int(*nptr);
					if(count > (base - 1)) 
					{
						*endptr = (char *) (nptr - 1);
						return(ret_value);
					}
				}
				else if(*nptr == '0' && base != 16){
					base = 8;
					nptr++;
					if (*nptr == '\0'){
						nptr--;
						*endptr = (char *)(nptr + 1);
						return(ret_value);
					}
					count = char_to_int(*nptr);
					if(count > (base - 1))
					{
						*endptr = (char *)(nptr);
						return(ret_value);
					}
				}
				else if(*nptr == '\0' || base != 16)
				{
					nptr--;
					base = 10;
				}
				else  
				{
					base = 16;
				}
			}
			else if (base == 16)
			{
				base = 16;
			}
			else 
			{
				base = 10;
			}
		}
		while (*nptr != '\0')
		{
			if(negative == 0){
	    		count = char_to_int(*nptr);
	    		if (count <= (base - 1)){
					if ((LONG_MAX / base) >= ret_value){
		   			ret_value *= base;
		   			if ((LONG_MAX - count) >= ret_value){
		       			index++;
							ret_value += count;
							*endptr = (char *) (nptr + 1);
						}
		   			else{
						  while(*nptr != '\0')
						  {
							  count = char_to_int(*nptr);
							  if (count <= (base - 1))
							  {
							     *endptr = (char *)(nptr + 1);
							  }
							  nptr++;
						  }
						  ret_value = LONG_MAX;
		       		  errno = ERANGE;
		       		  return(LONG_MAX); 
		   			}
	        		}
	        		else{
						while (*nptr != '\0')
						{
							count = char_to_int(*nptr);
							if (count <= (base - 1))
							{
								*endptr = (char *)(nptr + 1);
							}
							nptr++;
						}
	            	ret_value = LONG_MAX;
	            	errno = ERANGE;
						return(LONG_MAX);
            	}
	    		}
				else
				{
					if (index >= 1){
					*endptr = (char *)(nptr);
					}
					else{
					*endptr = (char *) (sum);
					}
					return(ret_value);
				}
	    		nptr++;
        }
		  else{
			count = char_to_int(*nptr);
			if (count <= (base - 1)){
				if((LONG_MIN / base) <= ret_value){
					if(first == 1){
						ret_value *= base;
					}
					else{
						ret_value *= -base;
					}
					if ((LONG_MIN + count <= ret_value)){
						index++;
						ret_value -= count;
						*endptr = (char *)(nptr + 1);
					}
					else{	
						  while(*nptr != '\0')
						  {
							  count = char_to_int(*nptr);
							  if (count <= (base - 1))
							  {
							     *endptr = (char *)(nptr + 1);
							  }
							  nptr++;
						  }
						ret_value = LONG_MIN;
						errno = ERANGE;
						return(LONG_MIN);	
					}
				}
				else{
				  	while(*nptr != '\0')
				   {
						count = char_to_int(*nptr);
					   if (count <= (base - 1))
					   {
					   	*endptr = (char *)(nptr + 1);
					   }
					   nptr++;
				   }
					ret_value = LONG_MIN;
					errno = ERANGE;
					return(LONG_MIN);
				}
			}
			else{
				if (index >= 1){
					*endptr = (char *)(nptr);
				}
				else{
					*endptr = (char *) (sum);
				}
				return(ret_value);
			}
			nptr++;
			first = 1;
		}
	}
   // if invalid base, set errno and return immediately

   // valid base, skip over white space

   // if encounter a sign, expect a number after that 
   // if a negative sign is encountered, change the sign

   
   // the exact algorithm is not given, but these are 
   // the tasks the function has to perform:  
   //
   // determine the actual base if the base is 0
   // if it starts with '0', it is octal
   // if '0' is followed by 'x' or 'X' it is hexadecimal or base 16
   // otherwise, it is decimal
   // therefore you have to determine the actual base and the starting
   // location to perform conversion
   //
   // for bases 2 to 15, perform conversion as in PE02
   //
   // for base 16, you have to decide when the conversion should
   // start: after the optional "0x" or "0X" or immediately
   // question: if you have "0xG", what should be converted?
   //
   // for bases 17 to 36, perform conversion as in PE02
   // 
   // after you have determine the real base, and where to start,
   // perform the conversion

   // clean up, set ret_value properly if the conversion went out of range
   // set errno if necessary
   // set *endptr properly if the endptr is not NULL

   return ret_value;
}
