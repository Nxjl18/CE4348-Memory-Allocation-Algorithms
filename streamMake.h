/*********************************************************************
*File: streamMake.c
*Author: Jose Lamuno
*Procedures:
*uniform	-Function to generate a uniform random number
*normal		-Function to generate a normalized random number
*makeStream	-Function to generate a stream of 1000 tasks with random size and duration
**********************************************************************/
#ifndef STREAMMAKE_H
#define STREAMMAKE_H

/*********************************************************************
*typedef struct task
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Defines a struct of type task which contains variables used to define a task 
*
*Parameters:
*	time		int		Duration of the task
*	size		int		Size of the task
**********************************************************************/
typedef struct{
    int size, time;
}task;

/*********************************************************************
*typedef struct tracker
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Defines a struct of type tracker which contains variables used to track processes in specific blocks of memory 
*
*Parameters:
*	taskTime	int		Tracks time of the task inside the block
*	taskNum		int		Tracks the number of the task inside the block
*	used		int		Tracks whether the block is in use
*	block		int		Tracks which block this is tracking
**********************************************************************/
typedef struct{
    int taskTime,taskNum, used, block;
}tracker;

/*********************************************************************
*struct task* makeStream()
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Generate a stream of 1000 tasks with random size and duration then return it to main program as struct of type task
*
*Parameters:
**********************************************************************/
task* makeStream();

/*********************************************************************
*int uniform( int lo, int hi )
*Author: Richard Goodrum
*Date: April 19 2020
*Description: Generates a uniform random number
*
*Parameters:
*	lo		I/P		int		Lower bound
*	hi		I/P		int		Upper bound
**********************************************************************/
int uniform(int lo, int hi);

/*********************************************************************
*int normal( int loops, int ulimit )
*Author: Richard Goodrum
*Date: April 19 2020
*Description: Generates a normalized random number
*
*Parameters:
*	loops		I/P		int		Number of loops
*	ulimit		I/P		int		Upper bound
**********************************************************************/
int normal (int loops, int ulimit);

#endif
