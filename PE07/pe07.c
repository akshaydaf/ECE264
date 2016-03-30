#include <stdio.h>
#include <stdlib.h>
#include "answer07.h"

int main(int argc, char * argv[])
{
   char** maze;
   int nrow = 0;
   int ncol = 0;
   int count = 0; 
   FILE *fptr = fopen("sample0", "r");
   char *filename = "SampleNew";
   int i = 0; 
   int j = 0; 
   char **newmaze;
   char **newmaze1;
   int rrow = 0; 
   int rcol = 0;
   int crow = 0;
   int ccol = 0;   
   maze = Read_maze_from_2Dfile(fptr,&nrow,&ncol);
   for (i = 0; i < nrow; i++) 
   {
     for(j = 0; j<ncol; j++)
     {
        printf("%c", maze[i][j]);
        count++;
     }
     printf("\n");
   }
   printf("\n");
   printf("nrow = %d\n ncol = %d\n count = %d\n", nrow,ncol,count);
   count = Write_maze_to_2Dfile(filename,maze,nrow,ncol); 
   newmaze = Expand_maze_row(maze,nrow,ncol,&rrow,&rcol);
   for (i = 0; i< rrow; i++)
   {
      for(j = 0; j <rcol; j++)
      {
         printf("%c", newmaze[i][j]); 
      }
      printf("\n");
   }
   printf("RROW: %d\n RCOL: %d\n", rrow, rcol);
   newmaze1 = Expand_maze_column(maze,nrow,ncol,&crow,&ccol);
   for (i = 0; i < crow; i++)
   {
      for(j = 0; j < ccol; j++)
      {
         printf("%c", newmaze1[i][j]);
      }
      printf("\n");
   }
   printf("\n");  
   Deallocate_maze_space(maze,nrow);
   Deallocate_maze_space(newmaze,rrow);  
   Deallocate_maze_space(newmaze1,crow); 
   fclose(fptr); 
   return(0);
}
