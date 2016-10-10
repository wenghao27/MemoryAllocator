#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void my_initialize_heap(int);//to initialize a block of memory on heap
void* my_alloc(int);//to allocate memory on the block 
void my_free(void*);//to free allocated memory on the block
double compute_standard_deviation(int*, int);

struct Block {
   int block_size;
   struct Block *next_block;
};

const int overhead_size = sizeof(struct Block); //8
const int overhead_ptr = sizeof(void*);//4
struct Block *free_head;//head pointer on static

int main()
{
   my_initialize_heap(1000); //initialize 1000 bytes of memory
   int* a1 = my_alloc(sizeof(int));
   double* b1 = my_alloc(sizeof(double));
   char* c1 = my_alloc(sizeof(char));

   my_free(a1);
   my_free(b1);
   my_free(c1);

   int* a2 = my_alloc(sizeof(int));
   double* b2 = my_alloc(sizeof(double));
   char* c3 = my_alloc(sizeof(char));
   
}

//to initialize memory block on heap.
void my_initialize_heap(int size) {

   //validate size, must at least greater than 0
   if (size < 0){
      printf("The size of the memory block initialized `must > 0 bytes");
      exit(0);
   }
     
   //assign the memory on heap to free_head
   free_head = (struct Block*)malloc(size + overhead_size);
   //initialize head block 
   free_head->block_size = size;
   free_head->next_block = NULL;
}

void* my_alloc(int size) {

   //size cannot be negative
   if(size < 0)
      exit(0);

   //size_to_allocate will include waste spaces if there's any
   int size_to_allocate = size;
   if (size % overhead_ptr != 0)
      size_to_allocate = size + (overhead_ptr - size % overhead_ptr);

   //declare a void pointer(will be pointing to data portion later)
   void * pToData = NULL;

   //create two new blocks both point to the first/head block
   struct Block *current = free_head;
   struct Block *pred = free_head;
 
   //while current does not point to null, keep iterating the list
   while (current != NULL) {

      //if the current block has enough memory space to allocate
      if (current->block_size >= size_to_allocate){
         //start position of the current block(each size is 1 byte)
         char* startPos = (char*)current;

         //if current block is big enough to split...
         if (current->block_size >= (size_to_allocate + overhead_size + overhead_ptr)){
            //start position moves to the beginning of block that is going to be allocated
            startPos += (current->block_size) - size_to_allocate;
            //create a new block, its size = size to allocated, it will point to null(will not be in the list)
            struct Block *aBlock = (struct Block *)startPos;
            aBlock->block_size = size_to_allocate;
            aBlock->next_block = NULL;
            //reduce the available size of original block
            current->block_size -= (overhead_size + size_to_allocate);
            //return the pointer points to the beginning of the data postion
            pToData = startPos + overhead_size;
            return pToData;
          }
          else {
            //else if the block is not big enough to split   
            //in the case of only one block in the list, then memory is full(list is empty)
            if (current == pred && current->next_block == NULL ) {
               free_head = NULL;
            }
            //in the case of only two blocks in the list(current and pred will both in the first block)
            //free_head will point to the second block of the list,(first block is "removed")
            if (current == pred && current ->next_block != NULL){ 
               free_head = current->next_block;
            }    
            //in the case of more than two blocks in the list
            //pred block will point to next block of the current(current block is "removed")
            if (current != pred) {
               pred->next_block = current->next_block;
            }
            //the pointer of current block is set to null(take out of the list)
            current ->next_block = NULL;
            //data portion is right after the overheads.
            startPos += overhead_size;
            pToData = startPos;
            return pToData;       
         }//end of else     
      }//end of outer if
      else {
         //if the current block does not have enough memory to allocate
         //current will move to next block, pred will be the one right before current 
         pred = current;
         current = current->next_block;
      }
   }//end of while loop
   printf("\nNot enough space to allocate!");
   return 0;
}//end of method

void my_free(void *data) {
 
   char* temp = data;
   //go backward to the beginning of the block, derefence it to get value of it
   int size = *(temp - overhead_size);
   //initialize and add the freed block to be head of the list
   struct Block *nBlock = (struct Block *)(temp - overhead_size);
   nBlock -> block_size = size; 
   nBlock->next_block = free_head;
   free_head = nBlock;
   data = NULL;
}


double compute_standard_deviation(int* array, int size) {
   int i, mean;
   int sum = 0, square_sum = 0;
   double standard_deviation;
   for (i = 0; i < size; ++i) {
      sum += array[i];
   }
   mean = sum / size;
   for (i = 0; i < size; ++i) {
      square_sum += (array[i] - mean) * (array[i] - mean);
   }
   standard_deviation = sqrt(square_sum / size);
   return standard_deviation;
}