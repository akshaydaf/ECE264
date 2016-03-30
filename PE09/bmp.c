#include <stdlib.h>
#include <stdio.h>

#include "bmp.h"

/* check whether a header is valid
 * assume that header has been read from fptr
 * the position of the indicator of fptr is not certain
 * could be at the beginning of the file, end of the file or 
 * anywhere in the file
 * note that the check is only for this exercise/assignment
 * in general, the format is more complicated
 */

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
      fprintf(stderr, "Header is invalid!\n");
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

/* The input arguments are the destination file pointer, BMP_Image *image.
 * The function write the header and image data into the destination file.
 * return TRUE if write is successful
 * FALSE otherwise
 */
int Write_BMP_Image(FILE* fptr, BMP_Image* image) 
{
   // go to the beginning of the file
   fseek(fptr, 0, SEEK_SET);
   // write header
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

/* The input argument is the BMP_Image pointer. The function frees memory of 
 * the BMP_Image.
 */
void Free_BMP_Image(BMP_Image* image) {
   free(image -> data);
   free(image);
}

unsigned char **Allocate_maze_space(int size, int size1)
{
   unsigned char **array2d;
   int i = 0;
   array2d = (unsigned char **) malloc(sizeof(unsigned char *)* size);
   if (array2d == NULL){
      free(array2d);
      return(NULL);
   }
   for (i = 0; i < size; i++){
      array2d[i] = (unsigned char *) malloc(sizeof(unsigned char) * size1);
      if (array2d[i] == NULL){
         free(array2d[i]);
         free(array2d);
         return(NULL);
      }
   }
   return(array2d);
}

void Deallocate_matrix_space(unsigned char **matrix, int size)
{
   int i = 0;
   for (i = 0; i < size; i++)
   {
      free(matrix[i]);
   }
   free(matrix);
   return;
}


// Given a valid BMP_Image, create a new image that retains the image in the
// box specified by left_x, right_x, bottom_y, top_y
//
BMP_Image *Crop_BMP_Image(BMP_Image *image, int left_x, 
                          int right_x, int bottom_y, 
                          int top_y)
{
   // getting the box coordinates to stay within range
   if (left_x < 0)
   {
      left_x = 0;
   } 
   if (left_x > (image -> header.width) - 1)
   {
      left_x = (image -> header.width) - 1;
   }
   if (right_x < 0)
   {
      right_x = 0;
   }
   if (right_x > (image -> header.width) - 1)
   {
      right_x = (image -> header.width) - 1;
   }
   if (left_x > right_x)
   {
      fprintf(stderr, "Can't crop the image\n"); 
      return NULL;
   }
   if (bottom_y < 0)
   {
      bottom_y = 0;
   } 
   if (bottom_y > (image -> header.height) - 1)
   {
      bottom_y = (image -> header.height) - 1;
   }
   if (top_y < 0)
   {
      top_y = 0;
   }
   if (top_y > (image -> header.height) - 1)
   {
      top_y = (image -> header.height) - 1;
   }
   if (bottom_y > top_y)
   {
      fprintf(stderr, "Can't crop the image\n");
      return NULL;
   }
   // proceed only if left_x <= right_x and bottom_y <= top_y
   if (left_x > right_x)
   {
      fprintf(stderr, "Can't crop the image\n");
      return NULL;
   }
   if (bottom_y > top_y)
   {
      fprintf(stderr, "Can't crop the image\n");
      return NULL;
   }    
   // create a new image cropped from the given image
   BMP_Image *t_image = NULL;
   long int row = top_y - bottom_y + 1;
   long int width = right_x - left_x + 1;
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
	paddingw = 0;
   t_image = malloc(sizeof(BMP_Image)); 
   if (t_image == NULL)
   {
      free(t_image);
      return NULL;
   }
   t_image -> header = image -> header;
   t_image -> header.width = width;
   t_image -> header.height = row;
   int i = 0;
   int j = 0;
   int k = 0;
   unsigned char** array2d;
   array2d = Allocate_maze_space((image -> header.height), (((image -> header.width) * (image -> header.bits)) / 8) + paddingw ); 
   for (i = 0; i < (image -> header.height); i++)
   {
      for(j = 0; j < ((image -> header.width) * (image -> header.bits)/8) + paddingw; j++)
      {
            array2d[i][j] = image -> data[k];
            k++;
      }
   }
   
   paddingw = width * (t_image -> header.bits) / 8;
   if (paddingw % 4 != 0)
   {
      paddingw = 4 - (paddingw % 4); 
      if (paddingw == 4)
      {
         paddingw = 0;
      }
   }
   else 
      paddingw = 0;
   t_image -> header.imagesize = (((t_image -> header.width * t_image -> header.bits/ 8) + paddingw) * t_image -> header.height);
   t_image -> header.size = t_image -> header.imagesize + 54;
   t_image -> data = malloc(t_image -> header.imagesize);
   if (t_image -> data == NULL)
   {
      free(t_image -> data);
      free(t_image);
   }

   unsigned char** array2dcrop; 
   int a = 0;
   int b = 0;
   int c = 0; 
   int temppad = paddingw; 
   array2dcrop = Allocate_maze_space(row, width * (t_image -> header.bits / 8) + paddingw); 
   for(i = bottom_y; i <= top_y; i++)
   {
      for (j = (left_x * (image -> header.bits / 8)); j < ((right_x+1) * (image -> header.bits / 8)); j++)
      {
         array2dcrop[a][b] = array2d[i][j];
         b++;
      }
      while (temppad != 0)
      {
         array2dcrop[a][b] = 0;  
         b++; 
         temppad--;
      }
      temppad = paddingw;
      c = b;
      b = 0;
      a++;
   }
   k = 0; 
   for (i = 0; i < a; i++)
   {
      for(j = 0; j < c; j++)
      {
         t_image -> data[k] = array2dcrop[i][j];
         k++;
      }
   }
   Deallocate_matrix_space(array2d, image -> header.height);
   Deallocate_matrix_space(array2dcrop, t_image -> header.height);
   return t_image;
}
