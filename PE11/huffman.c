#include <stdio.h>
#include <stdlib.h>

#include "list_tree.h"
#include "huffman.h"

// return NULL if the tree cannot be constructed
// return the constructed huffman tree
// if you want to check for corruption of input file
// check whether you have fully exhausted all bytes in
// the header section to build the tree
//
tnode *Build_huffman_tree(FILE *infptr)
{
   // you have to use to fseek to get to the right location of the file
   fseek(infptr,0,SEEK_SET);
   unsigned int fsize, hsize,csize;
   // to build the Huffman coding tree
   // First unsigned int in the file is the file size
   fread(&fsize,sizeof(unsigned int),1,infptr);
   // second unsigned int is the header size
   fread(&hsize,sizeof(unsigned int),1,infptr);
   // third unsigned int is the number of characters in the original file
   fread(&csize,sizeof(unsigned int),1,infptr);



   // initialize the stack, here, we have a dummy called stack
   // the code in list_tree.c assumes the presence of a dummy
   // the size of the stack is 0

   lnode stack;
   stack.next = NULL;
   set_stack_size(&stack, 0);
   int token;  // token to be read from the infptr
   int next = 0;
   tnode *tree;
   lnode *nodetree;
   lnode *elementl;
   lnode *elementr;
   tnode *treefin;  
   int size = 0;
   // while we are in the header region
   // break from the loop when you cannot continue to build a tree
   // e.g. encountering EOF, asked to build a tree from two trees
   // on the stack, but the stack contains 0 or 1 tree.
   int count = 0;  
   while (1) { // replace if necessary the correct condition
      token = fgetc(infptr); // get a character from the infptr
      count++;
      
      if (token == '1') {
	// what follows should be a character
        // read the character
        // build a tree that has a single tree node for that character
        // construct a list node with that tree node
        // push the list node onto the stack
        next = fgetc(infptr);
        tree = tree_construct(next, NULL, NULL);
	nodetree = node_construct(tree);
	push(&stack, nodetree);

      } else if (token == '0') {  
         // You have to build a bigger tree from two trees in the list nodes
         // popped from the stack
         // After that, you have to push a list node containing the bigger
         // tree onto the stack
         // Beware of:  (1) You can do the construction if there are 
         // 2 or more items in the stack 
         // (2) which of the two trees popped from the stack
         // is left and which is right of the bigger tree
         // (3) cleaning up so that you do not leak memory
         if (stack_size(&stack) < 2)
         {
            break;
         }
	 if ((size = stack_size(&stack)) >= 2)
	 {
            elementr = pop(&stack);
	    elementl = pop(&stack);	
	    treefin = tree_construct(0,elementl->tree,elementr->tree);
	    nodetree = node_construct(treefin);
	    push(&stack, nodetree);
	    free(elementr);
            free(elementl);
	 }	  
	 
      }
   }

   // check for the conditions that says that you have successfully
   // constructed a huffman coding tree:
   // you have exhausted all bytes in the header region of the file
   // you have exactly 1 item on the stack
   // in that case, the constructed huffman is contained in the 
   // only list node on the stack, get it, and return the constructed tree
   // otherwise, return NULL
   // always clean up memory to avoid memory leak
   tnode *newtree = stack.next -> tree;
   free(stack.next);
   return newtree;
}

// suggest that you write a recursive function which is called
// by this function to print the huffman codes of the ascii characters
// that appear in the tree in postorder fashion
//

void Post_order_print(tnode *root, FILE *outfptr)
{  
   static int i = 0;
   static int array[256];
   if (root -> left == NULL && root -> right == NULL)
   {
      int x = 0;
      fputc(root->value, outfptr);
      fputc(':', outfptr);
      for (x = 0; x < i; x++)
      {
         fputc(array[x] + '0',outfptr);
      }
      fputc('\n',outfptr);
   }
   if (root -> left != NULL)
   {
      array[i] = 0;
      i++;
      Post_order_print(root -> left, outfptr);
   }
   if (root -> right != NULL)
   {
      array[i] = 1;
      i++;
      Post_order_print(root -> right, outfptr);
   }
   i--;
   return;
}
