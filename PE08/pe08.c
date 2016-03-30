#include<stdio.h>
#include<stdlib.h>
#include "answer08.h"

int main(int argc, char ** argv)
{
   int size1 = 0;
   int size2 = 0; 
   if(argc == 5 && argv[1][0] == '-' && argv[1][1] == 'm')
   {
      char * matrixaf;
      matrixaf = argv[2];
      char * matrixbf;
      matrixbf = argv[3];
      char * output;
      output = argv[4];
      double ** matrixc;
      double **matrixa;
      double **matrixb;
      int fileresult = 0;
      matrixa = Read_matrix_from_file(argv[2], &size1);
      matrixb = Read_matrix_from_file(argv[3], &size2); 
      if (size1 != size2)
      {
         Deallocate_matrix_space(matrixa, size1);
         Deallocate_matrix_space(matrixb, size2); 
         fprintf(stderr, "Mismatched matricies!\n"); 
	 return(EXIT_FAILURE); 
      }
      matrixc = Matrix_matrix_multiply(matrixa,matrixb,size1); 
      if (matrixc == NULL)
      {
         Deallocate_matrix_space(matrixa, size1);
         Deallocate_matrix_space(matrixb, size2);
         return(EXIT_FAILURE); 
      }
      fileresult = Write_matrix_to_file(argv[4], matrixc, size1);
      Deallocate_matrix_space(matrixa, size1); 
      Deallocate_matrix_space(matrixb, size2); 
      Deallocate_matrix_space(matrixc, size1);  
   }
   else if (argc == 4 && argv[1][0] == '-' && argv[1][1] == 'd')
   {
      char * matrixaf;
      matrixaf = argv[2];
      char * matrixbf;
      matrixbf = argv[3];
      double **matrixa;
      double **matrixb;
      double sum = 0;
      matrixa = Read_matrix_from_file(argv[2], &size1); 
      matrixb = Read_matrix_from_file(argv[3], &size2); 
      if (size1 != size2)
      {
         Deallocate_matrix_space(matrixa, size1); 
         Deallocate_matrix_space(matrixb, size2); 
         fprintf(stderr, "Mismatched Matricies!\n"); 
         return(EXIT_FAILURE); 
      }
      sum = Matrix_matrix_difference(matrixa, matrixb, size1); 
      printf("%e\n", Matrix_matrix_difference(matrixa, matrixb, size1)); 
      Deallocate_matrix_space(matrixa, size1); 
      Deallocate_matrix_space(matrixb, size2);
   }
   else
   {
      fprintf(stderr,"Insufficient Input Arguments!\n"); 
      return EXIT_FAILURE;
   }
   return(EXIT_SUCCESS); 
}
