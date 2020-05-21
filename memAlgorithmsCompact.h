/*********************************************************************
*File: memAlgorithmsCompact.h
*Author: Jose Lamuno
*Procedures:
*bestFitCompact		-Function to run best fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
*firstFitCompact	-Function to run first fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
*nextFitCompact		-Function to run next fit algorithm on a specified task stream into amemory partitioned to blocks amount of blocks
*worstFitCompact	-Function to run worst fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
**********************************************************************/
#ifndef MEMALGORITHMSCOMPACT_H
#define MEMALGORITHMSCOMPACT_H

/*********************************************************************
*int bestFitCompact (task stream[], int memory[], int blocks)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Loads a task stream into a given fixed partition memory through best fit choice while counting the time. It then returns the total time spent. This version performs compaction.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
**********************************************************************/
int bestFitCompact (task stream[], int memory[], int blocks);

/*********************************************************************
*int firstFitCompact (task stream[], int memory[], int blocks)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Loads a task stream into a given fixed partition memory through first fit choice while counting the time. It then returns the total time spent. This version performs compaction.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
**********************************************************************/
int firstFitCompact (task stream[], int memory[], int blocks);

/*********************************************************************
*int nextFitCompact (task stream[], int memory[], int blocks)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Loads a task stream into a given fixed partition memory through next fit choice while counting the time. It then returns the total time spent. This version performs compaction.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
**********************************************************************/
int nextFitCompact (task stream[], int memory[], int blocks);

/*********************************************************************
*int worstFitCompact (task stream[], int memory[], int blocks)
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Loads a task stream into a given fixed partition memory through worst fit choice while counting the time. It then returns the total time spent. This version performs compaction.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
**********************************************************************/
int worstFitCompact (task stream[], int memory[], int blocks);

#endif
