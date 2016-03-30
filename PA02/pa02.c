#include<stdio.h>
#include<stdlib.h>
#include"pa_answer02.h"

int main(int argc, char** argv)
{

   printf("Welcome to ECE264, we are working on PA02.\n\n");
   
   if (argc == 4 && argv[1][0] == '-' && argv[1][1] == 'm')
   { 
      char * mazefile; 
      mazefile = argv[2];
      char * directionfile;
      directionfile = argv[3];
      Get_mowing_directions(argv[2], argv[3]);
   }
   else if (argc == 5 && argv[1][0] == '-' && argv[1][1] == 's')
   {
      int value = 0; 
      char * mazefile;
      mazefile = argv[2];
      char * directionfile;
      directionfile = argv[3];
      char * mowedfile;
      mowedfile = argv[4]; 
      value = Simulate_mowing(argv[2],argv[3],argv[4]);
      printf("%d\n", value); 
   }
   else
   {
      fprintf(stderr, "Insufficient input arguments. \n"); 
      return EXIT_FAILURE;
   }
   return (0);
}
