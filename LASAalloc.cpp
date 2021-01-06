/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
* 		LASAalloc.h
* 		LASAalloc class declaration.  
* 
* 		Do not change this file other than to add local varaibles and Functions.
*    Make any changes only in the provided block.
* * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include<iostream>
#include<stdlib.h>
#include "LASAalloc.h"

// Defines for LASAalloc buffer simulation.  
// Keep it simple, no changes to program break
#define INITIAL_MALLOC_SIZE 100000
#define MAX_MALLOC_SIZE 100000


using namespace std;

//typedef unsigned char BYTE_t;

LASAalloc::LASAalloc(){
	brk(INITIAL_MALLOC_SIZE);
	
	// Point to where first node will be located.
	block* firstBlock = (block*)bufferBase;
	freeListHeader = firstBlock;
    beg = firstBlock;
	//currentBlock = firstBlock;
	previousBlock = nullptr;
	
	// Configure first node on freeList
	firstBlock->size = (INITIAL_MALLOC_SIZE); //firstBlock->size = (int)(bufferSize);
	//bufferBase = bufferBase + bufferSize;
	firstBlock->prev_block = nullptr;
	firstBlock->next_block = nullptr; 
	firstBlock->this_block_data = (void*)((long long int)bufferBase+(long long int)sizeof(block)); 
	
	firstBlock->freeFlag = true; 


	// Show initial statistics
	cout<<"buffer Allocation: "<< bufferBase << " - " << brk(0) << endl;
	cout<<"freeList: "<< freeListHeader << " - " << brk(0) << endl;
	cout<<"Block header size " << sizeof(block) << endl;
	cout<<"integer size " << sizeof(int) << endl;
	
	display_node(freeListHeader);

}

LASAalloc::~LASAalloc(){
    //delete(bufferBase);
}

 

void LASAalloc::display_node(struct block *p)
{
		cout << "Prev: " << p->prev_block;
		cout << "\tNext: " << p->next_block;
		cout << "\tFree: " << p->freeFlag;
		cout << "\tSize: " << p->size;
		cout << "\tThis: " << p->this_block_data << endl;
		cout << endl;	
}

void LASAalloc::display(){
	//int count = 0;
	block*p = beg;
	if(p==NULL){
		cout<<"List is empty\n";
		return;
	}

	cout<<"List is :\n";
	while(p!=nullptr){
		display_node(p);
		p = p->next_block;
		//count++;
		
	}
	cout<<"\n";
}


void* LASAalloc::lalloc(int size){

	
	if(size+sizeof(block) <= freeListHeader->size){
		split(freeListHeader, size);
		//block*next = freeListHeader->next_block;
		currentFreeList = (block*)((long long int)currentFreeList+(long long int)sizeof(block));
		return currentFreeList;
	}
	
  	else if(size+sizeof(block) > freeListHeader->size){
		
		if(freeListHeader->next_block == nullptr){
			
			return nullptr;
		}
		else{
			block*current = freeListHeader;
			while(current!=nullptr){
				if((current->freeFlag) == true && (current->size) > size+sizeof(block)){
					
					split(current,size);
					currentFreeList = (block*)((long long int)currentFreeList+(long long int)sizeof(block));
					return currentFreeList;
				}
				else{
					current = current->next_block;
				}
			}
		}
  	}
  return nullptr;
    
}

void* LASAalloc::lfree(void* userBlock){

	//backtrack from where data is stored to where metadata is stored
	block*toUse = (block*)((long long int)userBlock-(long long int)sizeof(block));
	block* nextBlock = toUse->next_block;
	block*prevBlock = toUse->prev_block;
	int newSize = toUse->size;
	//clear block and replace all metadata
	//toUse=NULL;
	toUse->prev_block = prevBlock;
	toUse->next_block = nextBlock;
	toUse->size = newSize;
	toUse->freeFlag = true;
	toUse->this_block_data = (void*)((long long int)((block*)userBlock)+(long long int)sizeof(block));


	//make all the free blocks point to each other (breaks rest of the program's logic)
	// while(current != nullptr){
	// 	if(current->freeFlag==true){
	// 		newFree->next_block=current;
	// 		break;
	// 	}
	// 	else{
	// 		current = current->next_block;
	// 	}
	// }
	
	
	//Coalesce all free blocks
	block* current = beg;
	block*next = (block*)(current->next_block);
	while(next!=nullptr){
		
		if(current->freeFlag==true && next->freeFlag==true){
			current->size = current->size + next->size + sizeof(block);
			current->next_block = next->next_block;
			//next = NULL;
			next = current->next_block;
		}
		else{
			current = next;
			next = next->next_block;
		}
	}

	//set freeListHeader to first free block
	current = beg;
	while(current!=nullptr){
	if(current->freeFlag == true){
		freeListHeader = current;
	 	break;
	}
	current = current->next_block;
	}
	return 0;
}


// void* LASAalloc::findFit(int size)
// {
	
// }

void* LASAalloc::split(block* target, int size){

block* allocated = target;
block* freeBlock = (block*)((long long int)(target) + size + (long long int)sizeof(block));
int originialSize = (target->size);

//add the size of metadata to the free block if this isn't the first time the buffer's being split as the first time the buffer is split there is no metadata there but
//every other time there's already 1 metadata slot for the freeList
if(originialSize != INITIAL_MALLOC_SIZE){
	originialSize+=sizeof(block);
}

block*originalNext = target->next_block;
block*originalPrev = target->prev_block;

allocated->prev_block = originalPrev;
allocated->next_block = freeBlock;
allocated->size = (int)size;
allocated->freeFlag = false;
allocated->this_block_data = (void*)((long long int)allocated + (long long int)sizeof(block));
currentFreeList = allocated;

freeBlock->prev_block = allocated;
freeBlock->next_block = originalNext;
freeListHeader = freeBlock;

freeBlock->size = (originialSize) - (size + 2*(int)sizeof(block));
freeBlock->freeFlag = true;
freeBlock->this_block_data = (void*)((long long int)freeBlock + (long long int)sizeof(block));

return allocated;
  
}
 


/*
 *   >>>>>>  DO NOT CHANGE THIS SECTION  <<<<<<<
 * 
 * brk()
 * Function to simulate the libc brk() function to allocate memory for a buffer
 * 
 */

void * LASAalloc::brk(int size)
{
	
	if (size !=0)
	{
		if (bufferBase == 0)
		{
			bufferBase = malloc(size);
			bufferSize = size;
		}
		else
		{
			cout<<"buffer already locked/n";
			return 0;
		}
		
	}
	return 0; //bufferSize + bufferBase;
}
