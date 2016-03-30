#include <stdio.h>
#include <stdlib.h>
#include "answer07.h"

// if you want to declare and define new functions, put them here
// or at the end of the file


// do not remove #ifndef and #endif in this file, otherwise the evaluation
// would fail and you would definitely get 0
// any new functions you want to create and use in this file 
// should appear above these comments or at the end of this file

#ifndef NTEST_MEM

// allocate space for nrow x ncol maze, if allocation fails, return NULL
// if allocation fails, you are also responsible for freeing the memory
// allocated before the failure
// may assume that nrow and ncol are > 0

char **Allocate_maze_space(int nrow, int ncol)
{
   char **array2d;
   int i = 0;
   array2d = (char**) malloc(sizeof(char*)*nrow);
   if (array2d == NULL)
   {
      free(array2d); 
      return NULL;
   }
   for (i = 0; i < nrow; i++)
   {
      array2d[i] = (char *)malloc(sizeof(char)*ncol);
      if (array2d[i] == NULL)
      {
         free(array2d[i]);
         free(array2d);  
         return(NULL);
      }
   }
   return(array2d);
}

// free the memory used for the maze
// you may assume that maze is not NULL, and all memory addresses are valid

void Deallocate_maze_space(char **maze, int nrow) 
{
   int i = 0; 
   for (i = 0; i < nrow; i++)
   {
      free(maze[i]);
   }
   free(maze);
   return;
}

#endif /* NTEST_MEM */

#ifndef NTEST_READ

/* Read maze in a multi-line file into a 2D array of characters */
/* you may assume that the maze has odd row and column counts */

char **Read_maze_from_2Dfile(FILE *fptr, int *nrow, int *ncol)
{
   fseek(fptr,0,SEEK_SET);
   *nrow = *ncol = 0;
   int c = 0;
   int i = 0; 
   int j = 0; 
   char** maze;
   int rowval = 0;
   int colval = 0;   
   while (c != EOF && c != '\n')
   {
      c = fgetc(fptr); 
      *ncol += 1;
   }
   *ncol -= 1;
   fseek(fptr, 0, SEEK_SET); 
   while (c != EOF)
   {
      c = fgetc(fptr); 
      if (c == '\n')
      {
         *nrow += 1; 
      }
   }
   rowval = *nrow; 
   colval = *ncol; 
   fseek(fptr,0,SEEK_SET);
   maze = Allocate_maze_space(rowval,colval);
   for (i = 0; i < *nrow; i++)
   { 
      for (j = 0; j < *ncol; j++)
      { 
         if ((c = fgetc(fptr))  != '\n'){
           maze[i][j] = c;
	 }
         
      }
      c = fgetc(fptr); 
   }
   return maze;
}

#endif /* NTEST_READ */

#ifndef NTEST_WRITE

/* Write maze in a 2D array of characters into a multi-line file */
/* you may assume that the maze has odd row and column counts */

int Write_maze_to_2Dfile(char *filename, char **maze, int nrow, int ncol) 
{
   FILE *fptr2 = fopen(filename, "w"); 
   int i = 0; 
   int j = 0;
   int c = 0; 
   int count = 0;  
   for (i = 0; i < nrow; i++)
   {
      for(j = 0; j < ncol; j++)
      {
         c = maze[i][j]; 
	 fputc(c,fptr2);
         count++;   
      }
      c = '\n';
      fputc(c,fptr2);
      count++;
   }
   fclose(fptr2);
   return count;
}

#endif /* NTEST_WRITE */

#ifndef NTEST_ROW

/* Expand the maze from nrow x ncol to (2nrow - 1) x ncol */
/* the top half of the maze is the same as the original maze */
/* the bottom half of the maze is a reflection of the original maze */
/* *nrow and *rcol have to be updated with the new row and column counts */
/* You have to create a new maze */
/* if you can't expand the maze because of memory issue, NULL should be */
/* returned, and both *nrow and *rcol set to 0 */
/* the original maze has to stay intact */
/* you may assume that the original maze has odd row and column counts */

char **Expand_maze_row(char **maze, int nrow, int ncol, int *rrow, int *rcol)
{
   *rrow = *rcol = 0;
   *rrow = (2 * nrow) - 1;
   *rcol = ncol;
   char** newmaze;
   int temprow = *rrow; 
   int tempcol = *rcol;  
   newmaze = Allocate_maze_space(temprow,tempcol);
   if (newmaze == NULL)
   {
     *rrow = 0;
     *rcol = 0;
     return(NULL); 
   }  
   int i = 0; 
   int j = 0; 
   for (i = 0; i < nrow; i++) 
   {
      for(j = 0; j < ncol; j++)
      {
         newmaze[i][j] = maze[i][j];
      }
   }
   i = nrow;  
   j = 0;
   int c = nrow - 2; 
   for (i = nrow; i < *rrow; i++)
   {
      for(j = 0; j < *rcol; j++)
      {
         newmaze[i][j] = maze[c][j]; 
      }
      c -= 1;
   }  
   return newmaze;
}

#endif /* NTEST_ROW */

#ifndef NTEST_COL 

/* Expand the maze from nrow x ncol to nrow x 2ncol - 1 */
/* the left half of the maze is the same as the original maze */
/* the right half of the maze is a reflection of the original maze */
/* moreover, you have to create an opening between the left and right halves */
/* the opening should be at the middle row of the maze */
/* the opening must also be accessible from one of the grass-paths in the */
/* new maze */
/* to do that, you have to convert some locations with corns to grass */
/* starting from the new opening to the left and to the right until you reach */
/* a location that is adjacent to a location that has grass */
/*                                                                       */
/* *crow and *ccol have to be updated with the new row and column counts */
/* You have to create a new maze */
/* if you can't expand the maze because of memory issue, NULL should be */
/* returned, and both *crow and *ccol set to 0 */
/* the original maze has to stay intact */
/* you may assume that the original maze has odd row and column counts */

char **Expand_maze_column(char **maze, int nrow, int ncol, int *crow, int *ccol)
{
   *crow = *ccol = 0;
   *crow = nrow;
   *ccol = (2 * ncol) - 1;
   char** widemaze; 
   int temprow = *crow; 
   int tempcol = *ccol; 
   widemaze = Allocate_maze_space(temprow, tempcol); 
   int i = 0; 
   int j = 0; 
   for (i = 0; i < nrow; i++)
   {
      for(j = 0; j < ncol; j++)
      {
         widemaze[i][j] = maze[i][j];
      }
   }
   i = 0; 
   j = ncol;
   int c = ncol - 2;  
   for (i = 0; i < nrow; i++)
   {
      for(j = ncol; j < *ccol; j++)
      {
         widemaze[i][j] = maze[i][c];
         c = c - 1; 
      }
      c = ncol - 2;  
   }    
   return widemaze;
}

#endif /* NTEST_COL */

// if you want to declare and define new functions, put them here
// or at the beginning of the file
