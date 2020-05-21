/*********************************************************************
*File: memoryMake.c
*Author: Jose Lamuno
*Procedures:
*comparator	-Function to sort the blocks generated from smallest to largest. Used as input to qsort()
*randomMem	-Function to generate a fixed partition memory with a random amount of partitions and partition sizes. One partition is guaranteed to be of size 16 to accomodate the largest possible task size.
**********************************************************************/
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include<sys/time.h>
#include "memoryMake.h"

/*********************************************************************
*int comparator (const void *x, const void *y)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Compares two elements of an array and returns input for qsort() to sort from smaller to larger
*
*Parameters:
*	*x		I/P		void	Left element of array
*	*y		I/P		void	Right element of array
**********************************************************************/
int comparator(const void *x, const void *y)
{
    int l = *(const int *)x;	//Loads left value into l
    int r = *(const int *)y;	//Loads right value into r

    return (l-r);				//Returns positive if l>r, negative if l<r
}

/*********************************************************************
*int* randomMem(int* blocks)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Generate a fixed partition memory with a random amount of partitions and partition sizes. One partition is guaranteed to be of size 16 to accomodate the largest possible task size. Returns a memory array.
*
*Parameters:
*	blocks		O/P		int				The number of blocks within the memory
**********************************************************************/
int* randomMem(int* blocks)
{
    //Initialize variables needed for memory
    static int temp[14];		//Temporary array to create memory
    temp[0] = 16;				//Creates a block of size 16
    int block = 1;				//Starts memory with 1 block
    int availMemory = 40;		//Leftover memory 56-16 = 40

    int i;						//Iterator

    srand(time(0));				//Sets seed to the current time for as close to random as possible
    
    while(1){		//While there is still space left in memory to partition...
        temp[block] = (((rand()%13)+4)/4)*4;	//Generate a  block of size 4-16. Note that blocks can only be multiples of 4.

        if(temp[block] < 1 ) 					//If for some reason the block ends up being of size 0, sets it to size 1
            temp[block] = 1;

        while(temp[block] > availMemory){			//If the block size ended up being larger than available memory, rerolls until its within bounds
            temp[block] = (((rand()%13)+4)/4)*4;

            if(temp[block] < 1 ) 			//If for some reason the block ends up being of size 0, sets it to size 1
                temp[block] = 1;
        }

        availMemory -= temp[block];		//Reduces available memory by the new block created
        block += 1;						//Increases block count
            if (availMemory == 0)		//If memory is fully partitioned break from loop
                break;
    }

    qsort((void*)temp, block, sizeof(int), comparator);	//Sorts the memory partitions from smallest to largest

	//Prints the final memory to let the user know what it looks like
    printf("\nMemory allocated with %i blocks of sizes: \n", block);
    
    for(i=0; i<block; i++)
        printf("Block %i of size %i \n", (i+1),temp[i]);

    *blocks = block;	//Returns the amount of blocks the new random memory has back to main program
    return temp;		//Returns the fixed partition memory array
}
