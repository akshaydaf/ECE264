#include<stdio.h>
#include<stdlib.h>
#include "bmp.h"

int main (int argc, char ** argv)
{
   if (argc == 3)
   {   
      int i = 0;
      BMP_Image *image;
      BMP_Image *new; 
      FILE *fptr = fopen(argv[1], "rb");
      FILE *fptr2 = fopen(argv[2], "wb");
      if (fptr == NULL)
      {
         fclose(fptr2);
         fprintf(stderr, "Can't open input file\n");
         return(EXIT_FAILURE);
      }
      image = Read_BMP_Image(fptr);
      if (image == NULL)
      {
         fclose(fptr);
         fclose(fptr2);
         fprintf(stderr, "Error reading input file\n");
         return EXIT_FAILURE;
      } 
      if (image -> header.bits == 16)
      {
         new = Convert_16_to_24_BMP_Image(image);
         i = Write_BMP_Image(fptr2, new);
         fclose(fptr);
         fclose(fptr2);
         Free_BMP_Image(image); 
         Free_BMP_Image(new);
         return EXIT_SUCCESS;
      }
      else if(image -> header.bits == 24)
      {
         new = Convert_24_to_16_BMP_Image_with_Dithering(image);
         i = Write_BMP_Image(fptr2, new);
         fclose(fptr);
         fclose(fptr2);
         Free_BMP_Image(image);
         Free_BMP_Image(new);
         return EXIT_SUCCESS; 
      }
      else{
         fclose(fptr);
         fclose(fptr2); 
         Free_BMP_Image(image);
         return EXIT_FAILURE;
      }    
   }
   else{
      fprintf(stderr, "Wrong number of arguments expected\n");
      return EXIT_FAILURE;
   }
}

