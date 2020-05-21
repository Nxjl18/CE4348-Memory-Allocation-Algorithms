/*********************************************************************
*File: memAlgorithmsCompact.c
*Author: Jose Lamuno
*Procedures:
*bestFitCompact		-Function to run best fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
*firstFitCompact	-Function to run first fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
*nextFitCompact		-Function to run next fit algorithm on a specified task stream into amemory partitioned to blocks amount of blocks
*worstFitCompact	-Function to run worst fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
**********************************************************************/
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include<sys/time.h>
#include "streamMake.h"
#include "memAlgorithmsCompact.h"

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
int bestFitCompact (task stream[], int memory[], int blocks)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed
    int bestBlock = -1;			//Tracks the current best block
	
	int temp[blocks];			//Initializes a temporary location for memory

	for(i=0; i<blocks; i++)		//Copies the memory and its blocks to a temporary location
		temp[i] = memory[i];

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[1000];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<1000; i++){
        track[i].used = 0;		//Tracks whether the block is in use
        track[i].taskTime = 0;	//Tracks how much time the process in the block has left
        track[i].taskNum = 0;	//Tracks the task number of the process inside the block
		track[i].block = 0;		//Tracks which block the task is in
    }

	//Main loop to place tasks into memory
    for(i=0; i<1000; i++){
        taskPlaced=0;	//Sets the task placed flag to 0
		bestBlock = -1;	//Sets the best block flag to -1

        //printf("\nTrying to place task %i of size %i", i, stream[i].size);

		//Loop to check all blocks in memory for a fit
        for(j=0; j<blocks; j++){
            if(stream[i].size <= temp[j]){	//If the block is the same size or larger than the process size it moves on to the next step
            	if(bestBlock < 0)			//If there is no current best block it sets the current block as the best block
                	bestBlock = j;
                else if(temp[bestBlock] > temp[j])	//If the current block is a better fit for the process then it becomes the best block
                	bestBlock = j;
            }
        }

		//If a best block has been found and the task has not been placed yet
        if(bestBlock > -1 && taskPlaced < 1){

			//printf("\nBest block for task %i is at %i", i, bestBlock+1);

            track[i].taskNum = i;				//Task i is tracked
            track[i].taskTime = stream[i].time;	//Task i has its time tracked
            track[i].used = 1;					//Task is using block bestBlock
			track[i].block = bestBlock;			//Block being used by task tracked
			temp[bestBlock] -= stream[i].size;	//Deducts the size of task i from the block size 
            taskPlaced = 1;						//Denotes the task as placed

            //printf("\nTask %i placed at block %i with time %i", i, bestBlock+1, stream[i].time);
			//printf("\nBlock %i now has %i space left", bestBlock+1, temp[bestBlock]);

        }

        //If statement to check if a task was placed or not
        if(taskPlaced < 1){		//If a task was not placed...

            //printf("\nTask %i not placed with time %i", i, stream[i].time);

            timeConsumed += 1;	//Increases time consumed
            i -= 1;				//Reduces i to retry placing the failed task

			//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            for(n=0; n<1000; n++){
                if(track[n].used > 0){		//If the block is in use
                    track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1

                    //printf("\nTask %i is in block %i with %i time left", track[n].taskNum, n+1, track[n].taskTime);

                }
                if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    track[n].used = 0;								//Remove the task from the block
					temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task

                    //printf("\nTask %i removed from memory", track[n].taskNum);
					//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                }
            }
        }

        //If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
            for(n=0; n<1000; n++)		//...for all tasks remaining in memory...
                timeConsumed += track[n].taskTime;	//...add their remaining time to the total time consumed

            //printf("\nTotal time: %i \n", timeConsumed);

            return timeConsumed;		//return the total time elapsed
        }
    }
}

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
int firstFitCompact (task stream[], int memory[], int blocks)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed
	
	int temp[blocks];			//Initializes a temporary location for memory

	for(i=0; i<blocks; i++)		//Copies the memory and its blocks to a temporary location
		temp[i] = memory[i];

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[1000];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<1000; i++){
        track[i].used = 0;		//Tracks whether the block is in use
        track[i].taskTime = 0;	//Tracks how much time the process in the block has left
        track[i].taskNum = 0;	//Tracks the task number of the process inside the block
		track[i].block = 0;		//Tracks which block the task is in
    }

	//Main loop to place tasks into memory
	for(i=0; i<1000; i++){
        taskPlaced=0;		//Sets the task placed flag to 0

        //printf("\nTrying to place task %i", i);

		//Loop to check all blocks in memory for a fit
        for(j=0; j<blocks; j++){
            if(stream[i].size <= temp[j]){				//If the block is the same size or larger than the process size it moves on to the next step
                if(taskPlaced < 1){	//If the task has not been placed..
                    track[i].taskNum = i;					//Places the task into block j
                    track[i].taskTime = stream[i].time;		//Tracks remaining task time
                    track[i].used = 1;						//Marks the block as in use
					track[i].block = j;						//Tracks the block in which task i was assigned
					temp[j] -= stream[i].size;				//Makes the block size memory smaller according to task i size
                    taskPlaced = 1;							//Marks the task as placed

                    //printf("\nTask %i placed into block %i with time %i", i, j, track[j].taskTime);

                    break;
                }
            }
        }

		//If statement to check if a task was placed or not
        if(taskPlaced < 1){		//If a task was not placed...

            //printf("\nTask %i not placed with time %i", i, stream[i].time);

            timeConsumed += 1;	//Increases time consumed
            i -= 1;				//Reduces i to retry placing the failed task

			//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            for(n=0; n<1000; n++){
                if(track[n].used > 0){		//If the block is in use
                    track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1

                    //printf("\nTask %i is in block %i with %i time left", track[n].taskNum, n+1, track[n].taskTime);

                }
                if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    track[n].used = 0;								//Remove the task from the block
					temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task

                    //printf("\nTask %i removed from memory", track[n].taskNum);
					//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                }
            }
        }

		//If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
            for(n=0; n<1000; n++)		//...for all tasks remaining in memory...
                timeConsumed += track[n].taskTime;	//...add their remaining time to the total time consumed

            //printf("\nTotal time: %i \n", timeConsumed);

            return timeConsumed;		//return the total time elapsed
        }
    }
}

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
int nextFitCompact (task stream[], int memory[], int blocks)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed
	
	int temp[blocks];			//Initializes a temporary location for memory

	for(i=0; i<blocks; i++)		//Copies the memory and its blocks to a temporary location
		temp[i] = memory[i];

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[1000];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<1000; i++){
        track[i].used = 0;		//Tracks whether the block is in use
        track[i].taskTime = 0;	//Tracks how much time the process in the block has left
        track[i].taskNum = 0;	//Tracks the task number of the process inside the block
		track[i].block = 0;		//Tracks which block the task is in
    }

	j=0; 						//Initializes j iterator to 0;	

	//Main loop to place tasks into memory
    for(i=0; i<1000; i++){
        taskPlaced=0;

        //printf("\nTrying to place task %i of size %i", i, stream[i].size);

		//Loop to check all blocks in memory for a fit
        for(n=0; n<blocks; n++){

			//printf("\nTest if task %i of size %i fits in block %i of size %i", i, stream[i].size, j+1, memory[j]);

            if(stream[i].size <= temp[j]){				//If the block is the same size or larger than the process size it moves on to the next step
                if(taskPlaced < 1){						//If the block has not been placed..
					track[i].taskNum = i;					//Places the task into block j
                    track[i].taskTime = stream[i].time;		//Tracks remaining task time
                    track[i].used = 1;						//Marks the block as in use
					track[i].block = j;						//Tracks the block in which task i was assigned
					temp[j] -= stream[i].size;				//Makes the block size memory smaller according to task i size
                    taskPlaced = 1;							//Marks the task as placed

                    //printf("\nTask %i placed into block %i with time %i", i, j+1, track[i].taskTime);
					//printf("\nBlock %i now has %i space left", j+1, temp[j]);

                    break;
                }
            }

			//printf("\nj = %i", j); 

            j = (j+1)%blocks;			//Moves to the next position in memory relative to the last placed block
        }

		//If statement to check if a task was placed or not
        if(taskPlaced < 1){		//If a task was not placed...

            //printf("\nTask %i not placed with time %i", i, stream[i].time);

            timeConsumed += 1;	//Increases time consumed
            i -= 1;				//Reduces i to retry placing the failed task

			//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            for(n=0; n<1000; n++){
                if(track[n].used > 0){		//If the block is in use
                    track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1

                    //printf("\nTask %i is in block %i with %i time left", track[n].taskNum, n+1, track[n].taskTime);

                }
                if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    track[n].used = 0;								//Remove the task from the block
					temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task

                    //printf("\nTask %i removed from memory", track[n].taskNum);
					//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                }
            }
        }

		//If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
            for(n=0; n<1000; n++)		//...for all tasks remaining in memory...
                timeConsumed += track[n].taskTime;	//...add their remaining time to the total time consumed

            //printf("\nTotal time: %i \n", timeConsumed);

            return timeConsumed;		//return the total time elapsed
        }
    }
}

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
int worstFitCompact (task stream[], int memory[], int blocks)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed
    int worstBlock = -1;			//Tracks the current worst block
	
	int temp[blocks];			//Initializes a temporary location for memory

	for(i=0; i<blocks; i++)		//Copies the memory and its blocks to a temporary location
		temp[i] = memory[i];

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[1000];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<1000; i++){
        track[i].used = 0;		//Tracks whether the block is in use
        track[i].taskTime = 0;	//Tracks how much time the process in the block has left
        track[i].taskNum = 0;	//Tracks the task number of the process inside the block
		track[i].block = 0;		//Tracks which block the task is in
    }

	//Main loop to place tasks into memory
    for(i=0; i<1000; i++){
        taskPlaced=0;	//Sets the task placed flag to 0
		worstBlock = -1;	//Sets the worst block flag to -1

        //printf("\nTrying to place task %i of size %i", i, stream[i].size);

		//Loop to check all blocks in memory for a fit
        for(j=0; j<blocks; j++){
            if(stream[i].size <= temp[j]){	//If the block is the same size or larger than the process size it moves on to the next step
            	if(worstBlock < 0)			//If there is no current worst block it sets the current block as the worst block
                	worstBlock = j;
                else if(temp[worstBlock] < temp[j])	//If the current block is a worse fit for the process then it becomes the worst block
                	worstBlock = j;
            }
        }

		//If a worst block has been found and the task has not been placed yet
        if(worstBlock > -1 && taskPlaced < 1){

			//printf("\nworst block for task %i is at %i", i, worstBlock+1);

            track[i].taskNum = i;				//Task i is tracked
            track[i].taskTime = stream[i].time;	//Task i has its time tracked
            track[i].used = 1;					//Task is using block worstBlock
			track[i].block = worstBlock;			//Block being used by task tracked
			temp[worstBlock] -= stream[i].size;	//Deducts the size of task i from the block size 
            taskPlaced = 1;						//Denotes the task as placed

            //printf("\nTask %i placed at block %i with time %i", i, worstBlock+1, stream[i].time);
			//printf("\nBlock %i now has %i space left", worstBlock+1, temp[worstBlock]);

        }

        //If statement to check if a task was placed or not
        if(taskPlaced < 1){		//If a task was not placed...

            //printf("\nTask %i not placed with time %i", i, stream[i].time);

            timeConsumed += 1;	//Increases time consumed
            i -= 1;				//Reduces i to retry placing the failed task

			//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            for(n=0; n<1000; n++){
                if(track[n].used > 0){		//If the block is in use
                    track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1

                    //printf("\nTask %i is in block %i with %i time left", track[n].taskNum, n+1, track[n].taskTime);

                }
                if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    track[n].used = 0;								//Remove the task from the block
					temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task

                    //printf("\nTask %i removed from memory", track[n].taskNum);
					//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                }
            }
        }

        //If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
            for(n=0; n<1000; n++)		//...for all tasks remaining in memory...
                timeConsumed += track[n].taskTime;	//...add their remaining time to the total time consumed

            //printf("\nTotal time: %i \n", timeConsumed);

            return timeConsumed;		//return the total time elapsed
        }
    }
}
