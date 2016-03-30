#include <stdio.h>
#include <stdlib.h>

#include "list_tree.h"
#include "huffman.h"

// return NULL if the tree cannot be constructed
// return the constructed huffman tree
//
// you should re-use the function written in PE11 if it is correct
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

// if you successfully decode, return 1
// otherwise return 0
// The definition of success: the third unsigned integer specifies the 
// number of characters to be decoded.  If you decoded all characters as 
// specified and there are no more characters left in the encoded file
// for you to decode, it is a success.
//
// Even if decoding fails, whatever you have written into outfptr 
// should remain.  You just have to return 0.
//
int Huffman_decoding(tnode *huffman, FILE *infptr, FILE *outfptr)
{
   // first you should get the number of characters to be decoded
   // that number is the third unsigned int in the input file
   // you have to advance beyond the header section to access
   // the encoded data
   fseek(infptr, 0, SEEK_SET); 
   unsigned int charnum, hnum, fnum;
   fread(&fnum, sizeof(unsigned int), 1, infptr);
   fread(&hnum, sizeof(unsigned int), 1, infptr);
   fread(&charnum, sizeof(unsigned int), 1, infptr);
   fseek(infptr, hnum + 12, SEEK_SET); 

   // you should have a tnode *curr that is first initialized 
   // to point to the root of the tree, i.e., huffman

   tnode *curr = huffman;
   unsigned char byte;
   int count = 0;   
   int next_bit = 0; // you have to decide how to get the next bit
   // now while the number of characters to be decoded 
   // is greater than 0, you iterate

   while (charnum > 0) {  /* put in the right condition */
      // if curr is a leaf node
      // you have decoded one character
      // you should print that character, and decrement the number
      // of character to be decoded
      // reset curr to point to huffman
      if (is_leaf_node(curr)) {
      fputc(curr -> value, outfptr);
      charnum -= 1;
      curr = huffman; // restart

      } else {
         // decide on the next direction to go
         // get the next bit from the input file (from most significant 
         // position to the least significant in a byte) 
         // however, you can only read in byte-by-byte from a file
         // therefore, you probably need to have a variable to store
         // a byte read from the file and a variable to store the 
         // the position you are currently at in that byte
         // whenever you finish processing a bit, you should update
         // your position in the byte
         // you have to think of when you have to read in a new byte
         // and restart at the most significant position
         // of course, if you encounter end of file while the 
         // the number of characters to be decoded is still > 0, decoding
         // should fail
         // if next bit is 0, go left, otherwise, go right

         if (count == 0)
	 {
 	    fread(&byte, sizeof(unsigned char), 1, infptr); 
 	 }
	 next_bit = ((byte << count) & 128) >> 7;
         count += 1;
         if (next_bit == 0) {
            curr = curr->left;
         } else {
            curr = curr->right;
         }
         if (count > 7)
	 {
 	    count = 0;
	 }
      }
   }

   // clean up at the end
   // have to decide whether you have successfully decoded 
   // all characters specified by the third unsigned int in the input file
   // decoded
   // 
   // if you want to check whether the input file has been corrupted
   // 1.  check that no more characters left in the input file to be decoded
   // 2.  all remaining bits in the char you are processing when you decoded
   // the last character are 0

   return 1;
}
