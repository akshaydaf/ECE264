#include <stdio.h>
#include<stdlib.h>

int main(int argc, char ** argv)
{
   FILE *infptr = fopen("file", "r");
   fseek(infptr,0 ,SEEK_END);
   int fpos = ftell(infptr);
   int c = 0;
   if (fpos != 0)
   {
      fseek(infptr, -1, SEEK_CUR);
      c = fgetc(infptr);
      printf("C: %d\n", c);  
      while (ftell(infptr) > 1)
      {
         fseek(infptr, -2, SEEK_CUR);
         c = fgetc(infptr);
         printf("C: %d\n", c); 
      }
   }
   int *array[5]; 
   int * array = malloc(sizeof(int*)*5);
   int **[array][i];
   
   printf("fpos bitches: %d\n", fpos);
   array[4] = (int *)malloc(sizeof(int) * 2);
   int i;
   return(0);
}
