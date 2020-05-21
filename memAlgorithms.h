/*********************************************************************
*File: memAlgorithms.h
*Author: Jose Lamuno
*Procedures:
*bestFit	-Function to run best fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
*firstFit	-Function to run first fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
*nextFit	-Function to run next fit algorithm on a specified task stream into amemory partitioned to blocks amount of blocks
*worstFit	-Function to run worst fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
**********************************************************************/
#ifndef MEMALGORITHMS_H
#define MEMALGORITHMS_H
/*********************************************************************
*int bestFit (task stream[], int memory[], int blocks)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Loads a task stream into a given fixed partition memory through best fit choice while counting the time. It then returns the total time spent.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
**********************************************************************/
int bestFit (task stream[], int memory[], int blocks);

/*********************************************************************
*int firstFit (task stream[], int memory[], int blocks)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Loads a task stream into a given fixed partition memory through first fit choice while counting the time. It then returns the total time spent.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
**********************************************************************/
int firstFit (task stream[], int memory[], int blocks);

/*********************************************************************
*int nextFit (task stream[], int memory[], int blocks)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Loads a task stream into a given fixed partition memory through next fit choice while counting the time. It then returns the total time spent.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
**********************************************************************/
int nextFit (task stream[], int memory[], int blocks);

/*********************************************************************
*int worstFit (task stream[], int memory[], int blocks)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Loads a task stream into a given fixed partition memory through worst fit choice while counting the time. It then returns the total time spent.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
**********************************************************************/
int worstFit (task stream[], int memory[], int blocks);

#endif
