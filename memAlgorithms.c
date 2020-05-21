/*********************************************************************
*File: memAlgorithms.c
*Author: Jose Lamuno
*Procedures:
*bestFit	-Function to run best fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
*firstFit	-Function to run first fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
*nextFit	-Function to run next fit algorithm on a specified task stream into amemory partitioned to blocks amount of blocks
*worstFit	-Function to run worst fit algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
**********************************************************************/

#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include<sys/time.h>
#include "streamMake.h"
#include "memAlgorithms.h"

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
int bestFit (task stream[], int memory[], int blocks)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed
    int bestBlock = -1;			//Tracks the current best block

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[blocks];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<blocks; i++){
        track[i].used = 0;		//Tracks whether the block is in use
        track[i].taskTime = 0;	//Tracks how much time the process in the block has left
        track[i].taskNum = 0;	//Tracks the task number of the process inside the block
    }

	//Main loop to place tasks into memory
    for(i=0; i<1000; i++){
        taskPlaced=0;	//Sets the task placed flag to 0
		bestBlock = -1;	//Sets the best block flag to -1

        //printf("\nTrying to place task %i of size %i", i,stream[i].size);

		//Loop to check all blocks in memory for a fit
        for(j=0; j<blocks; j++){
            if(stream[i].size <= memory[j]){	//If the block is the same size or larger than the process size it moves on to the next step
                if(track[j].used < 1){			//If the block is not in use then it is considered a best block
                    if(bestBlock < 0)			//If there is no current best block it sets the current block as the best block
                        bestBlock = j;
                    else if(memory[bestBlock] >= memory[j])	//If the current block is a better fit for the process then it becomes the best block
                        bestBlock = j;
                }
            }
        }

		//If a best block has been found and it is not in use the task is put into memory
        if(bestBlock > -1){

			//printf("\nBest block for task %i is at %i", i, bestBlock+1);

            track[bestBlock].taskNum = i;					//Task i is tracked in block bestBlock
            track[bestBlock].taskTime = stream[i].time;		//Task time is tracked
            track[bestBlock].used = 1;						//bestBlock block is marked as in use
            taskPlaced = 1;									//Flag enoting a task has been placed is raised

            //printf("\nTask %i placed at block %i with time %i", i, bestBlock+1, stream[i].time);

        }

		//If statement to check if a task was placed or not
        if(taskPlaced < 1){		//If a task was not placed...

            //printf("\nTask %i not placed with time %i", i, stream[i].time);

            timeConsumed += 1;	//Increases time consumed
            i -= 1;				//Reduces i to retry placing the failed task

			//Loop to decrement remaining time for tasks in memory and remove them if time is now 0
            for(n=0; n<blocks; n++){
                if(track[n].used > 0){		//If the block is in use
                    track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1

                    //printf("\nTask %i is in block %i with %i time left", track[n].taskNum, n+1, track[n].taskTime);

                }
                if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    track[n].used = 0;								//Remove the task from the block

                    //printf("\nTask %i removed from memory", track[n].taskNum);
          
                }
            }
        }

		//If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
            for(n=0; n<blocks; n++)		//...for all tasks remaining in memory...
                timeConsumed += track[n].taskTime;	//...add their remaining time to the total time consumed

            //printf("\nTotal time: %i \n", timeConsumed);

            return timeConsumed;		//return the total time elapsed
        }
    }
}

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
int firstFit (task stream[], int memory[], int blocks)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[blocks];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<blocks; i++){
        track[i].used = 0;		//Tracks whether the block is in use
        track[i].taskTime = 0;	//Tracks how much time the process in the block has left
        track[i].taskNum = 0;	//Tracks the task number of the process inside the block
    }

	//Main loop to place tasks into memory
    for(i=0; i<1000; i++){
        taskPlaced=0;		//Sets the task placed flag to 0

        //printf("\nTrying to place task %i", i);

		//Loop to check all blocks in memory for a fit
        for(j=0; j<blocks; j++){
            if(stream[i].size <= memory[j]){				//If the block is the same size or larger than the process size it moves on to the next step
                if(track[j].used < 1 && taskPlaced < 1){	//If the block is not being used and the task has not been placed..
                    track[j].taskNum = i;					//Places the task into block j
                    track[j].taskTime = stream[i].time;		//Tracks remaining task time
                    track[j].used = 1;						//Marks the block as in use
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

			//Loop to decrement remaining time for tasks in memory and remove them if time is now 0
            for(n=0; n<blocks; n++){
                if(track[n].used > 0){		//If the block is in use
                    track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1

                    //printf("\nTask %i is in block %i with %i time left", track[n].taskNum, n+1, track[n].taskTime);

                }
                if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    track[n].used = 0;								//Remove the task from the block

                    //printf("\nTask %i removed from memory", track[n].taskNum);
                
                }
            }
        }

		//If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
            for(n=0; n<blocks; n++)		//...for all tasks remaining in memory...
                timeConsumed += track[n].taskTime;	//...add their remaining time to the total time consumed

            //printf("\nTotal time: %i \n", timeConsumed);

            return timeConsumed;		//return the total time elapsed
        }
    }
}

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
int nextFit (task stream[], int memory[], int blocks)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[blocks];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<blocks; i++){
        track[i].used = 0;		//Tracks whether the block is in use
        track[i].taskTime = 0;	//Tracks how much time the process in the block has left
        track[i].taskNum = 0;	//Tracks the task number of the process inside the block
    }

    j=0;	//Initializes j to 0

	//Main loop to place tasks into memory
    for(i=0; i<1000; i++){
        taskPlaced=0;		//Sets the task placed flag to 0

        //printf("\nTrying to place task %i", i);

		//Loop to check all blocks in memory for a fit
        for(n=0; n<blocks; n++){
            if(stream[i].size <= memory[j]){				//If the block is the same size or larger than the process size it moves on to the next step
                if(track[j].used < 1 && taskPlaced < 1){	//If the block is not being used and the task has not been placed..
                    track[j].taskNum = i;					//Places the task into block j
                    track[j].taskTime = stream[i].time;		//Tracks remaining task time
                    track[j].used = 1;						//Marks the block as in use
                    taskPlaced = 1;							//Marks the task as placed

                    //printf("\nTask %i placed into block %i with time %i", i, j, track[j].taskTime);

					j = (j+1)%blocks;						//If a task was placed, saves the location in memory for next task
                    break;
                }
            }
			j = (j+1)%blocks;								//Moves to the next position in memory relative to the last placed block
        }



		//If statement to check if a task was placed or not
        if(taskPlaced < 1){		//If a task was not placed...

            //printf("\nTask %i not placed with time %i", i, stream[i].time);

            timeConsumed += 1;	//Increases time consumed
            i -= 1;				//Reduces i to retry placing the failed task

			//Loop to decrement remaining time for tasks in memory and remove them if time is now 0
            for(n=0; n<blocks; n++){
                if(track[n].used > 0){		//If the block is in use
                    track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1

                    //printf("\nTask %i is in block %i with %i time left", track[n].taskNum, n+1, track[n].taskTime);

                }
                if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    track[n].used = 0;								//Remove the task from the block

                    //printf("\nTask %i removed from memory", track[n].taskNum);
                
                }
            }
        }

		//If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
            for(n=0; n<blocks; n++)		//...for all tasks remaining in memory...
                timeConsumed += track[n].taskTime;	//...add their remaining time to the total time consumed

            //printf("\nTotal time: %i \n", timeConsumed);

            return timeConsumed;		//return the total time elapsed
        }
    }
}

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
int worstFit (task stream[], int memory[], int blocks)
{
    //Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed
    int worstBlock = -1;			//Tracks the current worst block

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[blocks];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<blocks; i++){
        track[i].used = 0;		//Tracks whether the block is in use
        track[i].taskTime = 0;	//Tracks how much time the process in the block has left
        track[i].taskNum = 0;	//Tracks the task number of the process inside the block
    }

	//Main loop to place tasks into memory
    for(i=0; i<1000; i++){
        taskPlaced=0;	//Sets the task placed flag to 0
		worstBlock = -1;	//Sets the worst block flag to -1

        //printf("\nTrying to place task %i of size %i", i,stream[i].size);

		//Loop to check all blocks in memory for a fit
        for(j=0; j<blocks; j++){
            if(stream[i].size <= memory[j]){	//If the block is the same size or larger than the process size it moves on to the next step
                if(track[j].used < 1){			//If the block is not in use then it is considered a worst block
                    if(worstBlock < 0)			//If there is no current worst block it sets the current block as the worst block
                        worstBlock = j;
                    else if(memory[worstBlock] <= memory[j])	//If the current block is a better fit for the process then it becomes the worst block
                        worstBlock = j;
                }
            }
        }

		//If a worst block has been found and it is not in use the task is put into memory
        if(worstBlock > -1 && track[worstBlock].used < 1){

			//printf("\nworst block for task %i is at %i", i, worstBlock+1);

            track[worstBlock].taskNum = i;					//Task i is tracked in block worstBlock
            track[worstBlock].taskTime = stream[i].time;		//Task time is tracked
            track[worstBlock].used = 1;						//worstBlock block is marked as in use
            taskPlaced = 1;									//Flag enoting a task has been placed is raised

            //printf("\nTask %i placed at block %i with time %i", i, worstBlock+1, stream[i].time);

        }

		//If statement to check if a task was placed or not
        if(taskPlaced < 1){		//If a task was not placed...

            //printf("\nTask %i not placed with time %i", i, stream[i].time);

            timeConsumed += 1;	//Increases time consumed
            i -= 1;				//Reduces i to retry placing the failed task

			//Loop to decrement remaining time for tasks in memory and remove them if time is now 0
            for(n=0; n<blocks; n++){
                if(track[n].used > 0){		//If the block is in use
                    track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1

                    //printf("\nTask %i is in block %i with %i time left", track[n].taskNum, n+1, track[n].taskTime);

                }
                if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    track[n].used = 0;								//Remove the task from the block

                    //printf("\nTask %i removed from memory", track[n].taskNum); 
               
                }
            }
        }

		//If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
            for(n=0; n<blocks; n++)		//...for all tasks remaining in memory...
                timeConsumed += track[n].taskTime;	//...add their remaining time to the total time consumed

            //printf("\nTotal time: %i \n", timeConsumed);

            return timeConsumed;		//return the total time elapsed
        }
    }
}
