#include "pa_answer03.h"
#include <stdlib.h>
#include <stdio.h>
// Do not change this function
// otherwise, your matrices may not match what we are expecting

int Is_zero(double value)
{
   if (value < 0) {
      value = -value;
   }
   if (value < TOL) {
      return 1;
   } else {
      return 0;
   }
}
double **Allocate_maze_space(int size, int size1)
{
   double **array2d;
   int i = 0;
   array2d = (double **) malloc(sizeof(double *)* size);
   if (array2d == NULL){
      free(array2d);
      return(NULL);
   }
   for (i = 0; i < size; i++){
      array2d[i] = (double *) malloc(sizeof(double) * size1);
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
   array2d = Allocate_maze_space(temp, temp);
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
   double **matrixc = Allocate_maze_space(size, size);
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
void dividerow (double **matrix, int i, int j, int size) 
{
   int tempj = 0;
   double tempval = matrix[i][j]; 
   for (tempj = 0; tempj < 2 * size; tempj++)
   {
      if (Is_zero(tempval) != 1 && matrix[i][tempj] != 0)
      {
         matrix[i][tempj] = matrix[i][tempj] / tempval;
      }
   }
   return;                
}
int swaprow (double **augarr, int size, int i, int j)
{
   int temp = i;
   int tempj = 0;
   double temp_val = 0;
   while (temp < size)
   {
      if(Is_zero(augarr[temp][j]) != 1)
      {
         for (tempj = 0; tempj < 2 * size; tempj++)
         {
            temp_val = augarr[i][tempj];
            augarr[i][tempj] = augarr[temp][tempj];
	    augarr[temp][tempj] = temp_val;
	 }
         temp = size;
         return(1); 
      }
      temp++;
   }
   return(0);
}
void replacerow(double **matrix, int i, int j, int size)
{
   int tempj = 0;
   double temp_value = matrix[i][j];
   if(Is_zero(matrix[i][j]) != 1)
   {
      for (tempj = 0; tempj < 2 * size; tempj++)
      {
         matrix[i][tempj] -= (temp_value * matrix[j][tempj]); 
      }
      return;
   }
   return;
}  
double **Invert_matrix(double **matrix, int size)
{
   int i = 0;
   int j = 0;
   double **augarr;
   int temp = 0;
   augarr = Allocate_maze_space(size, 2 * size);
   for (i = 0; i < size; i++)
   {
      for (j = 0; j < size; j++)
      {
         augarr[i][j] = matrix[i][j];
      }
   }
   for (i = 0; i < size; i++)
   {    
       for(j = size; j < 2 * size; j++)
       {
          if(i + size == j)
          {
             augarr[i][j] = 1;
          }
          else
          {
             augarr[i][j] = 0;
          }
       }
   }
   for (j = 0; j < size; j++)
   {
      i = j; 
      if (Is_zero(matrix[i][j] == 1))
      {
         temp = swaprow(augarr,size,i,j);
         if (temp == 0)
         {
            Deallocate_matrix_space(augarr, size); 
            return(NULL);
         }
         dividerow(augarr,i,j,size);
      }
      else
      {
         dividerow(augarr,i,j,size);
      }
      for (i = 0; i < size; i++)
      {
         if (i != j)
         {
            replacerow(augarr,i,j,size);   
         }
      }
    }
    double **finarray;
    finarray = Allocate_maze_space(size, size);
    for (i = 0; i < size; i++)
    {
       for(j = 0; j < size; j++)
       {
          finarray[i][j] = augarr[i][j+size];
       }
    }
    Deallocate_matrix_space(augarr,size); 
    return(finarray);                      
}

double Deviation_from_identity(double **matrix, int size)
{
   int i = 0; 
   int j = 0;
   double **ident;
   double sum = 0; 
   ident = Allocate_maze_space(size, size);  
   for (i = 0; i < size; i++)
   {
      for (j = 0; j < size; j++)
      {
         if (i == j)
         {
            ident[i][j] = 1;    
         }
         else{
            ident[i][j] = 0;
         }
      }
   }
   for (i = 0; i < size; i++)
   {
      for (j = 0; j < size; j++)
      {
         if (matrix[i][j] < 0)
         {
            matrix[i][j] *= -1;
         }
         matrix[i][j] -= ident[i][j];
         sum += matrix[i][j];
      }
   }
   Deallocate_matrix_space(ident, size); 
   return(sum); 
}

