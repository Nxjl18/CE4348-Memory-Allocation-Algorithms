# CE4348-Memory-Allocation-Algorithms

Program to recreate memory allocation algorithms and measure computing time consumed by each approach. A fixed partition approach is taken for sake of simplicity. A choice is given to the user as to whether to use predetermined memory partition block sizes or to randomize them as well as whether the user would like to use compaction or not.

# Problem definition

The system has 56 units of memory available.

The algorithms being tested are: best fit, first fit, next fit, and worst fit.

Scheduling is to work as follows:
1. Until a task cannot be placed, place the next task in the stream
2. Increment the time consumed by one.
3. Decrement time remaining for all tasks in memory by one.
4. If tasks remain in the stream, go back to 1.
5. Increment the time consumed by the largest time remaining for tasks in memory.
6. Return the amount of time consumed by the stream

The program runs for 1000 trials and returns the average times obtained in the process. 
	
# Task info stream

The task info stream consists of 1000 elements of type "task", each of which contains a size variable of type int and a time variable of type int. Both of those variables are generated randomly through a normal random number generator which uses the current time as the seed.
