#include <stdio.h>
#include <stdlib.h>
#include "answer06.h"

/* Determine the dimensions of the maze contain in file. */
/* The maze will always of the correct format */
/* the number of rows is stored at location pointed to by nrow */ 
/* the number of ncol is stored at location pointed to by ncol */

void Find_maze_dimensions(FILE *fptr, int *nrow, int *ncol)
{
   fseek(fptr,0,SEEK_SET);
   *nrow = *ncol = 0;
   int c = 0;
   while (c != EOF && c != '\n'){
      c = fgetc(fptr);
      *ncol += 1;
   }
   *ncol -= 1;
   fseek(fptr,0,SEEK_SET);
   while (c != EOF)
   {
      c = fgetc(fptr);
      if (c == '\n')
      {
         *nrow += 1;
      }
   }
   return;
}

/* Determine the column location of the top opening */

int Find_opening_location(FILE *fptr)
{
   fseek(fptr,0,SEEK_SET);
   int i = 0;
   int sum = 0;
   while (i != EOF && i != '\n')
   {
      i = fgetc(fptr);
      sum += 1;
      if (i == GRASS)
      {
         return sum - 1; 
      }
   }
   return sum - 1;
}

/* Count the number of locations with grass */
/* A location has grass if it is a space, ' ', or GRASS */

int Count_grass_locations(FILE *fptr)
{
   fseek(fptr,0,SEEK_SET);
   int i = 0;
   int sum = 0;
   while (i != EOF)
   {
      i = fgetc(fptr);
      if (i == GRASS)
      {
         sum += 1;
      }
   }
   return sum;
}

/* Return the type of location: CORN or GRASS, i.e. 'X' or ' ' */
/* In other words, return the character in the file for that location */ 
/* The location is specified by its row-column coordinates. */
/* The coordinates are specified by row and col */
/* These coordinates will always be valid for the given maze */
/* You do not have to worry about the coordinates being out of range */

char Get_location_type(FILE *fptr, int row, int col) {
   fseek(fptr,0,SEEK_SET);
   int i = 0;
   int current_row = 0; 
   while (i != EOF){
     i = fgetc(fptr);
     if (i == '\n'){
         current_row += 1;
     }
     if (current_row == row)
     {
        fseek(fptr,col,SEEK_SET);
	i = fgetc(fptr);
        i = (char) i;
	return(i);
     }
   }
   i = (char) i;
   return(i);  
}

 
/* Given a filename, re-represent the maze in the file pointer fptr */
/* in a single line */
/* The return value should be the number of characters written into the */
/* the new file */
/* if the writing to a file fails, you should return -1 */ 

int Represent_maze_in_one_line(char *filename, FILE *fptr)
{
   FILE *fptr2 = fopen(filename, "w");
   if (fptr2 == NULL){
      return(-1);
   }
   fseek(fptr, 0, SEEK_SET);
   fseek(fptr2, 0, SEEK_SET);
   int i = 0;
   int sum = 0;
   while ((i = fgetc(fptr)) != EOF)
   {
      if (i != '\n')
      {
        fputc(i,fptr2);  
      }
      sum += 1;
   }
   fclose(fptr2);
   return(sum);
}
