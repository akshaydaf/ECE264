#include<stdio.h>
#include<stdlib.h>
#include "pa_answer03.h"
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
   else if(argc == 4 && argv[1][0] == '-' && argv[1][1] == 'i') 
   {
      char *invert;
      invert = argv[2];
      char *output1;
      output1 = argv[3];
      double **invertm;
      double **augarr;
      int result = 0; 
      invertm = Read_matrix_from_file(argv[2], &size1);
      augarr = Invert_matrix(invertm, size1);
      if (augarr == NULL)
      {
         Deallocate_matrix_space(invertm, size1);
         fprintf(stderr,"The matrix is non-invertible!\n");
         return(EXIT_FAILURE);  
      }
      
      result = Write_matrix_to_file(argv[3], augarr, size1);
      Deallocate_matrix_space(invertm, size1);
      Deallocate_matrix_space(augarr, size1);
    }
    else if(argc == 3 && argv[1][0] == '-' && argv[1][1] == 'd')
    {
       char *inputfile;
       double **devmat;
       inputfile = argv[2];
       devmat = Read_matrix_from_file(argv[2], &size1);
       if (devmat == NULL) 
       {
          fprintf(stderr, "Error!\n");
          return(EXIT_FAILURE);
       } 
       printf("%e\n", Deviation_from_identity(devmat, size1));
       Deallocate_matrix_space(devmat, size1);  
    }
    else
    {
       fprintf(stderr, "Insufficient Amount of arguments!\n");
       return(EXIT_FAILURE);
    }
    return(EXIT_SUCCESS);
}
