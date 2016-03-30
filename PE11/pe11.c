#include<stdio.h>
#include<stdlib.h>
#include "huffman.h"
#include "list_tree.h"

int main(int argc, char ** argv)
{
   if (argc == 3)
   {
      FILE *infptr = fopen(argv[1], "r");
      if (infptr == NULL) 
      {
         return EXIT_FAILURE; 
      }
      FILE *outfptr = fopen(argv[2],"w");
      tnode *tree;
      tree = Build_huffman_tree(infptr);
      if(tree == NULL)
      {
         fclose(infptr);
         fclose(outfptr);
         return EXIT_FAILURE;
      }
      Post_order_print(tree, outfptr);
      fclose(infptr);
      fclose(outfptr);
      tree_destruct(tree); 
      return EXIT_SUCCESS; 
   }
   else 
   {
      return EXIT_FAILURE;
   }
   return 0;
}
