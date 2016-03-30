#include<stdlib.h>
#include<stdio.h>
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

void recursion(int openloc,int row, char ** maze, FILE *fptr2, int nrow, int ncol)
{
   if ( !(nrow-1==row) && maze[row + 1][openloc] == ' ')
   {
      maze[row][openloc] = '.';
      fputc('S',fptr2);
      recursion(openloc, row + 1, maze, fptr2, nrow, ncol);
      fputc('N', fptr2);  
   }
   if (row != 0 && maze[row - 1][openloc] == ' ')
   {
      maze[row][openloc] = '.';
      fputc('N',fptr2); 
      recursion(openloc, row - 1, maze, fptr2, nrow, ncol);
      fputc('S',fptr2);
   }
   if (!(ncol - 1 == openloc) && maze[row][openloc + 1] == ' ')
   {
      maze[row][openloc] = '.';
      fputc('E',fptr2);
      recursion(openloc + 1, row, maze, fptr2, nrow, ncol);
      fputc('W',fptr2);
   }
   if (openloc != 0 && maze[row][openloc - 1] == ' ')
   {
      maze[row][openloc] = '.';
      fputc('W',fptr2);
      recursion(openloc - 1, row, maze, fptr2, nrow, ncol);
      fputc('E',fptr2);
   }
   return; 
}
void Get_mowing_directions(char *mazefile, char *directionfile)
{
   FILE *fptr = fopen(mazefile,"r");
   if (fptr == NULL)
   {
      fclose(fptr);
      return; 
   }
   FILE *fptr2 = fopen(directionfile, "w");
   fseek(fptr2,0,SEEK_SET);  
   char **maze; 
   int nrow = 0;
   int ncol = 0;
   int openloc = 0;
   int row = 0;   
   maze = Read_maze_from_2Dfile(fptr, &nrow, &ncol);
   fseek(fptr,0,SEEK_SET); 
   openloc = Find_opening_location(fptr);
   recursion(openloc, row, maze, fptr2, nrow, ncol);
   fclose(fptr);
   fclose(fptr2);
   Deallocate_maze_space(maze,nrow); 
   return; 
}

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

void Write_maze_to_2Dfile(char *filename, char **maze, int nrow, int ncol)
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
   return;
}

int Simulate_mowing(char *mazefile, char *directionfile, char *mowedfile)
{
   FILE *fptra = fopen(mazefile, "r");
   if (fptra == NULL)
   {
      return -1;
   }
   FILE *fptrb = fopen(directionfile, "r"); 
   if (fptrb == NULL)
   {
      return -1; 
   }
   int nrow1 = 0;
   int ncol1 = 0; 
   int openloc = 0; 
   int row = 0; 
   char **maze2;
   int c = 0;
   int savedcol = 0;
   int grasslocations = 0; 
   maze2 = Read_maze_from_2Dfile(fptra, &nrow1, &ncol1); 
   fseek(fptra, 0, SEEK_SET); 
   openloc = Find_opening_location(fptra);
   savedcol = openloc;
   fseek(fptrb, 0, SEEK_SET);   
   while (c != EOF)
   {
      c = fgetc(fptrb);
      if (!(nrow1-1 == row) && (maze2[row + 1][openloc] == ' ' || maze2[row + 1][openloc] == '.') && c == 'S')
      {
         maze2[row][openloc] = '.';
         row++; 
      }
      else if((nrow1 - 1 == row || maze2[row + 1][openloc] == 'X') && c == 'S' )
      {
         Write_maze_to_2Dfile(mowedfile, maze2, nrow1, ncol1);
         fclose(fptra);
         fclose(fptrb);
         Deallocate_maze_space(maze2, nrow1); 
         return -1; 
      }    
      if (row != 0 && (maze2[row - 1][openloc] == ' ' || maze2[row - 1][openloc] == '.') && c == 'N')
      { 
         maze2[row][openloc] = '.';
         row--; 
      }
      else if((row == 0 || maze2[row - 1][openloc] == 'X') && c == 'N')
      {
         Write_maze_to_2Dfile(mowedfile, maze2, nrow1, ncol1);
         fclose(fptra);
         fclose(fptrb);
         Deallocate_maze_space(maze2, nrow1); 
         return -1;
      }          
      if (!(ncol1 - 1 == openloc) && (maze2[row][openloc + 1] == ' ' || maze2[row][openloc + 1] == '.') && c == 'E')
      {
         maze2[row][openloc] = '.';
         openloc++;
      }
      else if((ncol1 - 1 == openloc || maze2[row][openloc + 1] == 'X') && c == 'E')
      {
         Write_maze_to_2Dfile(mowedfile, maze2, nrow1, ncol1);
         fclose(fptra);
         fclose(fptrb);
         Deallocate_maze_space(maze2, nrow1); 
         return -1;
      }          
      if (openloc != 0 && (maze2[row][openloc - 1] == ' ' || maze2[row][openloc - 1] == '.') && c == 'W')
      {
         maze2[row][openloc] = '.'; 
         openloc--;
      }
      else if((openloc == 0 || maze2[row][openloc - 1] == 'X') && c == 'W')
      {
         Write_maze_to_2Dfile(mowedfile, maze2, nrow1, ncol1);
         fclose(fptra);
         fclose(fptrb);
         Deallocate_maze_space(maze2, nrow1); 
         return -1;
      }          
      if(c != 'W' && c != 'E' && c != 'N' && c != 'S')
      {
         if (c != EOF){    
            Write_maze_to_2Dfile(mowedfile, maze2, nrow1, ncol1);
            fclose(fptra);
            fclose(fptrb);
            Deallocate_maze_space(maze2, nrow1); 
            return -1;
         }
      }
   } 
   if (maze2[row][openloc] == maze2[row][savedcol])
   {
       Write_maze_to_2Dfile(mowedfile, maze2, nrow1, ncol1);
       FILE *fptrc = fopen(mowedfile,"r");
       grasslocations = Count_grass_locations(fptrc);
       fclose(fptra);
       fclose(fptrb);
       fclose(fptrc);
       Deallocate_maze_space(maze2, nrow1); 
       return(grasslocations); 
   }
   else 
   {
      Write_maze_to_2Dfile(mowedfile, maze2, nrow1, ncol1);
      Deallocate_maze_space(maze2, nrow1);
      fclose(fptra);
      fclose(fptrb); 
      return -1; 
   }
   fclose(fptra);
   fclose(fptrb);
   Deallocate_maze_space(maze2, nrow1); 
   return(grasslocations);  
}
