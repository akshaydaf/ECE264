nclude<stdio.h>
#include<stdlib.h>
#include"pa_answer02.h"

char **Allocate_maze_space(int nrow, int ncol)
{
   char **array2d;
   int i = 0;
   array2d = (char **) malloc(sizeof(char*)*nrow);
   if (array2d == NULL){
      free(array2d);
      return(NULL);
   }
   for (i = 0; i < nrow; i++){
      array2d[i] = (char *) malloc(sizeof(char) * ncol);
      if (array2d[i] == NULL){
         free(array2d[i]);
         free(array2d);
         return(NULL);
      }
   }
   return(array2d);
}
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

void Get_mowing_directions(char *mazefile, char *directionfile)
{
   FILE *fptr = fopen(*mazefile,"r");
   FILE *fptr2 = fopen(*directionfile, "r");
   char **maze;
   char **mazedir;
   int nrow = 0;
   int ncol = 0;
   int openloc = 0;
   maze = Read_maze_from_2Dfile(fptr, &nrow, &ncol);
   mazedir = Read_maze_from_2Dfile(fptr, &nrow, &ncol);
   openloc = Find_opening_location(fptr);




}


int Simulate_mowing(char *mazefile, char *directionfile, char *mowedfile)
{






}
  
