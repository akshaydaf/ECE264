#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char ** argv)
{
   FILE *fptr = fopen("car.bmp", "rb");
   if (fptr == NULL) {
	return EXIT_FAILURE;
   }
   FILE *fptr2 = fopen("output", "wb");
   if (fptr2 == NULL)
   {
      fclose(fptr);
	return EXIT_FAILURE;
   }
   int i = 0;
   FILE *fptr3 = fopen("output2", "wb");
   if (fptr3 == NULL){
      fclose(fptr);
      fclose(fptr2);
      return EXIT_FAILURE;
   }
   BMP_Image *image;
   BMP_Image *imagenew; 
   image = Read_BMP_Image(fptr); 
   if (image == NULL) {
	fclose(fptr);
	fclose(fptr2);
        fclose(fptr3);
	return EXIT_FAILURE;
   }

   int left_x = 100;
   int right_x = 120;
   int bottom_y = 146;
   int top_y = 390;
   imagenew = Crop_BMP_Image(image, left_x, right_x, bottom_y, top_y);
   if (imagenew == NULL)
   {
      fclose(fptr);
      fclose(fptr2);
      fclose(fptr3);
      Free_BMP_Image(image);
	return EXIT_FAILURE;
   }
      
   i = Write_BMP_Image(fptr3, imagenew);
   if (i == FALSE)
   {
      Free_BMP_Image(image);
      Free_BMP_Image(imagenew);
      fclose(fptr);
      fclose(fptr2);
      fclose(fptr3);
      return EXIT_FAILURE;
   }
   Free_BMP_Image(image);
   Free_BMP_Image(imagenew);
   fclose(fptr);
   fclose(fptr2);
   fclose(fptr3);
   return EXIT_SUCCESS; 
}
