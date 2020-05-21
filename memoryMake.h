/*********************************************************************
*File: memoryMake.h
*Author: Jose Lamuno
*Procedures:
*comparator	-Function to sort the blocks generated from smallest to largest. Used as input to qsort()
*randomMem	-Function to generate a fixed partition memory with a random amount of partitions and partition sizes. One partition is guaranteed to be of size 16 to accomodate the largest possible task size.
**********************************************************************/
#ifndef MEMORYMAKE_H
#define MEMORYMAKE_H

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
int comparator(const void *x, const void *y);

/*********************************************************************
*int* randomMem(int* blocks)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Generate a fixed partition memory with a random amount of partitions and partition sizes. One partition is guaranteed to be of size 16 to accomodate the largest possible task size. Returns a memory array.
*
*Parameters:
*	blocks		O/P		int				The number of blocks within the memory
**********************************************************************/
int* randomMem(int* blocks);

#endif
