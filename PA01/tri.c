
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int *array[5]; 
  //array = malloc(sizeof(int *) * 5);
  int i = 0;
  for(i =0; i< 5; i++)
  {
    array[i] = malloc(sizeof(int) * 5);
  }

  int j = 0;
  for(i =0; i<5; i++)
  {
    for(j = 0; j<5; j++)
    {
      array[i][j] = 1;
    }
  }
  for(i =0; i<5; i++)
  {
    for(j = 0; j<5; j++)
    {
      printf("%d",array[i][j]);
    }
    printf("\n");
  }
 
  return 0;
}
