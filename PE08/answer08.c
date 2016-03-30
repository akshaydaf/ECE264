#include<stdio.h>
#include<stdlib.h>
#include "answer08.h"

double **Allocate_maze_space(int size)
{
   double **array2d;
   int i = 0;
   array2d = (double **) malloc(sizeof(double *)* size);
   if (array2d == NULL){
      free(array2d);
      return(NULL);
   }
   for (i = 0; i < size; i++){
      array2d[i] = (double *) malloc(sizeof(double) * size);
      if (array2d[i] == NULL){
         free(array2d[i]);
         free(array2d);
         return(NULL);
      }
   }
   return(array2d);
}

double **Read_matrix_from_file(char *filename, int *size)
{
   FILE *fptr = fopen(filename, "rb");
   double** array2d; 
   int i = 0;
   if (fptr == NULL) 
   {
      fprintf(stderr, "Not able to open matrix file.\n");
      return NULL;
   }
   fseek(fptr,0,SEEK_SET);
   fread(size,sizeof(int),1,fptr);
   int temp = *size;
   array2d = Allocate_maze_space(temp);
   for (i = 0; i < *size; i++)
   {
      fread(array2d[i],sizeof(double),*size,fptr);
   }
   fclose(fptr);
   return(array2d);
}
int Write_matrix_to_file(char *filename, double **matrix, int size)
{
   FILE *fptr2 = fopen(filename, "wb");
   int i = 0;
   if (fptr2 == NULL)
   {
      return(0); 
   }
   fseek(fptr2,0,SEEK_SET);
   fwrite(&size, sizeof(int), 1, fptr2);
   for (i = 0; i < size; i++)
   {
      fwrite(matrix[i],sizeof(double), size, fptr2);
      if (fptr2 == NULL)
      {
         return 0;
      }
   }
   fclose(fptr2);
   return(1); 
}
double **Matrix_matrix_multiply(double **matrixa, double **matrixb, int size)
{
   int i = 0;
   int j = 0;
   int k = 0; 
   double **matrixc = Allocate_maze_space(size); 
   double sum = 0;  
   for (i = 0; i < size; i++)
   {
      for (j = 0; j < size; j++)
      {
         for (k = 0; k < size; k++)
         {
            matrixc[i][j] = matrixa[i][k]*matrixb[k][j];
            sum += matrixc[i][j];
         }
         matrixc[i][j] = sum;
         sum = 0;   
      }
   }
   return(matrixc);
   
}
double Matrix_matrix_difference(double **matrixa, double **matrixb, int size)
{
   int i = 0;
   int j = 0;
   double **matrixd = Allocate_maze_space(size);
   double sum = 0;  
   for (i = 0; i < size; i++)
   {
      for(j = 0; j < size; j++)
      {
         matrixd[i][j] = matrixa[i][j] - matrixb[i][j];
         if (matrixd[i][j] < 0)
         {
            matrixd[i][j] *= -1;   
         }   
      }
   }
   for (i = 0; i < size; i++)
   {
      for (j = 0; j < size; j++)
      {
         sum += matrixd[i][j];    
      }
   }
   Deallocate_matrix_space(matrixd, size);  
   return(sum);
}
void Deallocate_matrix_space(double **matrix, int size)
{
   int i = 0;
   for (i = 0; i < size; i++)
   {
      free(matrix[i]);
   }
   free(matrix);
   return;
}
