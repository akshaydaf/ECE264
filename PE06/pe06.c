#include <stdio.h>
#include <stdlib.h>
#include "answer06.h"

int main(int argc, char * argv[])
{
   FILE *fptr = fopen("sample","r");
   int nrow = 0;
   int ncol = 0;
   char grass_corn;
   int open_loc = 0;
   int grass_loc = 0;
   char* filename = "Sample2";
   int numchar = 0;
   Find_maze_dimensions(fptr,&nrow,&ncol);
   printf("nrow = %d, ncol = %d\n", nrow, ncol);
   open_loc = Find_opening_location(fptr);
   printf("OPEN LOC: %d\n", open_loc);
   grass_loc = Count_grass_locations(fptr);
   printf("GRASS LOC: %d\n", grass_loc);
   grass_corn = Get_location_type(fptr,2,2);
   printf("TYPE: %c\n", grass_corn);
   numchar = Represent_maze_in_one_line(filename,fptr);
   printf("NUM CHAR: %d\n", numchar); 
   fclose(fptr);
   return(0);
}
