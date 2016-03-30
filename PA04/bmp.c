#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
int Is_BMP_Header_Valid(BMP_Header* header, FILE *fptr) {
  // Make sure this is a BMP file
  if (header->type != 0x4d42) {
     return FALSE;
  }
  // skip the two unused reserved fields

  // check the offset from beginning of file to image data
  // essentially the size of the BMP header
  // BMP_HEADER_SIZE for this exercise/assignment
  if (header->offset != BMP_HEADER_SIZE) {
     return FALSE;
  }
      
  // check the DIB header size == DIB_HEADER_SIZE
  // For this exercise/assignment
  if (header->DIB_header_size != DIB_HEADER_SIZE) {
     return FALSE;
  }

  // Make sure there is only one image plane
  if (header->planes != 1) {
    return FALSE;
  }
  // Make sure there is no compression
  if (header->compression != 0) {
    return FALSE;
  }

  // skip the test for xresolution, yresolution

  // ncolours and importantcolours should be 0
  if (header->ncolours != 0) {
    return FALSE;
  }
  if (header->importantcolours != 0) {
    return FALSE;
  }
  
  // Make sure we are getting 24 bits per pixel
  // or 16 bits per pixel
  // only for this assignment
  if (header->bits != 24 && header->bits != 16) {
    return FALSE;
  }

  // fill in extra to check for file size, image size
  // based on bits, width, and height
  fseek(fptr,0,SEEK_END);
  long int c = 0;
  c = ftell(fptr);
  int padding = header -> width * (header -> bits / 8);
  if (padding != 0)
  {
     padding = 4 - padding % 4;
     if (padding == 4)
     {
        padding = 0;
     } 
  }
  uint32_t imagesize = (((header -> width) * (header -> bits / 8)) + padding)  * header -> height;
  if (header -> imagesize != imagesize){
    return FALSE;
  }
  if(header -> size != header->imagesize + 54)
  {
    return FALSE;
  }
   
  return TRUE;
}

/* The input argument is the source file pointer. 
 * The function returns an address to a dynamically allocated BMP_Image only 
 * if the file * contains a valid image file 
 * Otherwise, return NULL
 * If the function cannot get the necessary memory to store the image, also 
 * return NULu
 * Any error messages should be printed to stderr
 */
BMP_Image *Read_BMP_Image(FILE* fptr) {

  // go to the beginning of the file
   
   fseek(fptr, 0, SEEK_SET); 
   BMP_Image *bmp_image = NULL;
   int i = 0;
  //Allocate memory for BMP_Image*;
   bmp_image = malloc(sizeof(BMP_Image));
   if (bmp_image == NULL)
   {
      fprintf(stderr, "Error allocating memory\n");
      return(NULL);
   }
  //Read the first 54 bytes of the source into the header
   if (fread(&(bmp_image -> header), 54, 1, fptr) != 1)
   {
      fprintf(stderr, "Error reading input file\n");
      free(bmp_image);
      return (NULL); 
   }	

  // if read successful, check validity of header
   i = Is_BMP_Header_Valid(&(bmp_image -> header), fptr);
   fseek(fptr, 54, SEEK_SET); 
   if (i == 0)
   {
      fprintf(stderr, "Error reading input file\n");
      free(bmp_image);
      return(NULL);
   }    
  // Allocate memory for image data
   bmp_image -> data = malloc(bmp_image->header.imagesize);
   if (bmp_image -> data == NULL)
   {
      fprintf(stderr, "Error allocating memory\n");
      free(bmp_image);
      return(NULL);
   }
  // read in the image data
 
   if (fread(bmp_image -> data, bmp_image -> header.imagesize, 1, fptr) != 1)
   {
      fprintf(stderr, "Error reading input file\n");
      free(bmp_image -> data);
      free(bmp_image);
      return(NULL);
   }
   return bmp_image;
}

int Write_BMP_Image(FILE* fptr, BMP_Image * image)
{
   fseek(fptr, 0, SEEK_SET);
   if (fwrite(&(image -> header), 54, 1, fptr) != 1)
   {
      fprintf(stderr, "Error writing to output file\n");
      return FALSE;
   }
   // write image data
   if (fwrite((image -> data), image -> header.imagesize, 1, fptr) != 1)
   {
      fprintf(stderr, "Error writing to output file\n");
      return FALSE;
   }
   return TRUE;
}

void Free_BMP_Image(BMP_Image* image) {
   free(image -> data);
   free(image);
}
int addcheck(int num)
{
   if (num > 255)
   {
      num = 255;
      return num;
   }
   else if (num < 0)
   {
      num = 0;
      return num;
   }
   return num;
}
BMP_Image *Convert_24_to_16_BMP_Image_with_Dithering(BMP_Image *image)
{
   int paddingw = 0;
   paddingw = ((image -> header.width) * (image -> header.bits))/8;
   if (paddingw % 4 != 0)
   {
      paddingw = 4 - (paddingw % 4);
      if (paddingw == 4)
      {
         paddingw = 0;
      }
   }
   else
   {
      paddingw = 0;
   }
   int rbytes = ((image -> header.width * image -> header.bits) / 8) + paddingw;
   int i = 0;
   int j = 0;
   int red = 0;
   int green = 0;
   int blue = 0;
   int red1 = 0;
   int green1 = 0;
   int blue1 = 0;
   uint16_t pixel = 0;
   int paddingx = 0;
   int rbytesn = 0;
   int cbytesn = 0;
   int quantr = 0;
   int quantb = 0;
   int quantg = 0;
   BMP_Image *image_n;
   BMP_Image *imagecopy;
   image_n = malloc(sizeof(BMP_Image));
   if (image_n == NULL)
   {
      return NULL;
   }
   //creating copy of original image
   imagecopy = malloc(sizeof(BMP_Image));
   imagecopy -> data = calloc(image -> header.imagesize,1);
   for (i = 0; i < image -> header.imagesize; i++)
   {
      imagecopy->data[i] = image -> data[i];
   }
   //calculating padding and parameters for 16 bit image
   paddingx = image -> header.width * 2;
   if (paddingx % 4 != 0)
   {
      paddingx = 4 - (paddingx % 4);
      if (paddingx == 4)
      {
         paddingx = 0;
      }
   }
   else
   {
      paddingx = 0;
   }
   image_n -> header = image -> header;
   image_n -> header.bits = 16;
   image_n -> header.imagesize = image -> header.height * ((image -> header.width * 2) + paddingx);
   image_n -> header.size = image_n -> header.imagesize + 54;
   image_n -> data = calloc(image_n -> header.imagesize, 1);
   rbytesn = 2 * image_n -> header.width + paddingx;
   cbytesn = 2 * image_n -> header.height;
   for (i = image -> header.height - 1; i >= 0; i--)
   {
      for (j = 0; j < image -> header.width; j++)
      {
         blue = imagecopy -> data[(i * rbytes) + j * 3];
         green = imagecopy -> data[(i * rbytes) + j * 3 + 1];
         red = imagecopy -> data[(i * rbytes) + j * 3 + 2];
         red1 = red >> 3;
         green1 = green >> 3;
         blue1 = blue >> 3;
	 quantr = red - ((red1 * 255)/31);
         quantg = green - ((green1 * 255)/31);
         quantb = blue - ((blue1 * 255)/31);
 	 // blue
 	 if (j + 1 < image -> header.width){
         imagecopy -> data[(i * rbytes) + (j + 1) * 3] = addcheck((16 * imagecopy -> data[(i * rbytes) + (j + 1) * 3] + quantb * 7)/16);
	 }
         if (i - 1 >= 0 && j - 1 >= 0){
         imagecopy -> data[(i-1) * rbytes + (j - 1) * 3] = addcheck((16 * imagecopy -> data[((i-1) * rbytes) + (j - 1) * 3] + quantb*3)/16); 
         }
         if (i - 1 >= 0){
	 imagecopy -> data[(i-1) * rbytes + j * 3] = addcheck((16 * imagecopy -> data[((i-1) * rbytes) + j * 3] + quantb*5)/16); 
         }
         if (i - 1 >= 0 && j + 1 < image -> header.width){
	 imagecopy -> data[(i-1) * rbytes + (j + 1) * 3] = addcheck((16 * imagecopy -> data[((i-1) * rbytes) + (j + 1) * 3] + quantb*1)/16);
         } 
         //green
         if (j + 1 < image -> header.width){
	 imagecopy -> data[(i * rbytes) + (j + 1) * 3 + 1] = addcheck((16 *imagecopy -> data[(i * rbytes) + (j + 1) * 3 + 1] + quantg * 7)/16);
         }
         if (i - 1 >= 0 && j - 1 >= 0){
	 imagecopy -> data[(i-1) * rbytes + (j - 1) * 3 + 1] = addcheck((16 *imagecopy -> data[((i-1) * rbytes) + (j - 1) * 3 + 1] + quantg*3)/16); 
         }
         if (i - 1 >= 0){
	 imagecopy -> data[(i-1) * rbytes + j * 3 + 1] = addcheck((16 * imagecopy -> data[((i-1) * rbytes) + j * 3 + 1] + quantg*5)/16); 
         }
         if (i - 1 >= 0 && j + 1 < image -> header.width){
	 imagecopy -> data[(i-1) * rbytes + (j + 1) * 3 + 1] = addcheck((16 * imagecopy -> data[((i-1) * rbytes) + (j + 1) * 3 + 1] + quantg*1)/16); 
         }
	 //red
	 if (j + 1 < image -> header.width){ 
	 imagecopy -> data[(i * rbytes) + (j + 1) * 3 + 2] = addcheck((16 *imagecopy -> data[(i * rbytes) + (j + 1) * 3 + 2] + quantr * 7)/16);
         }
 	 if (i - 1 >= 0 && j - 1 >= 0){
	 imagecopy -> data[(i-1) * rbytes + (j - 1) * 3 + 2] = addcheck((16 * imagecopy -> data[((i-1) * rbytes) + (j - 1) * 3 + 2] + quantr*3)/16); 
         }
         if (i - 1 >= 0){
	 imagecopy -> data[(i-1) * rbytes + j * 3 + 2] = addcheck((16 * imagecopy -> data[((i-1) * rbytes) + j * 3 + 2] + quantr*5)/16); 
         }
         if (i - 1 >= 0 && j + 1 < image -> header.width){
	 imagecopy -> data[(i-1) * rbytes + (j + 1) * 3 + 2] = addcheck((16 * imagecopy -> data[((i-1) * rbytes) + (j + 1) * 3 + 2] + quantr*1)/16); 
         }
	 pixel = (red1 << 10) | (green1 << 5) | blue1;
         image_n -> data[(i * rbytesn) + j * 2] =  pixel & 0x00ff;
         image_n -> data[(i * rbytesn) + j * 2 + 1] =  pixel >> 8;
      }
   }
   Free_BMP_Image(imagecopy);
   return image_n;

}


BMP_Image *Convert_16_to_24_BMP_Image(BMP_Image *image)
{
   int paddingw = 0;
   paddingw = ((image -> header.width) * (image -> header.bits))/8;
   if (paddingw % 4 != 0)
   {
      paddingw = 4 - (paddingw % 4);
      if (paddingw == 4)
      {
         paddingw = 0;
      }
   }
   else
   {
      paddingw = 0;
   }
   int rbytes = ((image -> header.width * image -> header.bits) / 8) + paddingw;
   int i = 0;
   int j = 0;
   uint32_t red = 0;
   uint32_t green = 0;
   uint32_t blue = 0;
   uint32_t pixel = 0;
   int paddingx = 0;
   int rbytesn = 0;
   int cbytesn = 0; 
   unsigned int byte1 = 0;
   unsigned int byte2 = 0;
   uint16_t bytenew = 0;
   BMP_Image *image_16;
   image_16 = malloc(sizeof(BMP_Image)); 
   if (image_16 == NULL)
   {
      return NULL;
   }
   paddingx = image -> header.width * 3;
   if (paddingx % 4 != 0)
   {
      paddingx = 4 - (paddingx % 4);
      if (paddingx == 4)
      {
         paddingx = 0;
      }
   }
   else
   {
      paddingx = 0;
   } 
   image_16 -> header = image -> header;
   image_16 -> header.bits = 24;
   image_16 -> header.imagesize = image -> header.height * ((image -> header.width * 3) + paddingx);
   image_16 -> header.size = image_16 -> header.imagesize + 54; 
   image_16 -> data = calloc(image_16 -> header.imagesize, 1);
   rbytesn = 3 * image_16 -> header.width + paddingx;
   cbytesn = 3 * image_16 -> header.height; 
   for (i = 0; i < image -> header.height; i++)
   {
      for (j = 0; j < image -> header.width; j++)
      {
         byte1 = (image -> data[(i * rbytes) + j * 2]);
         byte2 = (image -> data[(i * rbytes) + j * 2 + 1]);
         bytenew = byte2 << 8 | byte1; 
         red = (((bytenew & RED_MASK) >> 10) * 255)/31;
         green = (((bytenew & GREEN_MASK) >> 5)* 255)/31;
         blue = ((bytenew & BLUE_MASK) * 255) / 31;
         pixel = 0;
         pixel = pixel | (red << 16) | (green << 8) | blue;
         image_16 -> data[(i * rbytesn) + j * 3] = pixel & 0xff;
         image_16 -> data[(i * rbytesn) + j * 3 + 1] = pixel >> 8;
         image_16 -> data[(i * rbytesn) + j * 3 + 2] = pixel >> 16;            
      }
   }
   return image_16;      
         
}
