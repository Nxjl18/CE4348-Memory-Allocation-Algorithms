/*********************************************************************
*File: streamMake.c
*Author: Jose Lamuno
*Procedures:
*uniform	-Function to generate a uniform random number
*normal		-Function to generate a normalized random number
*makeStream	-Function to generate a stream of 1000 tasks with random size and duration
**********************************************************************/
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include<sys/time.h>
#include "streamMake.h"

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
int uniform( int lo, int hi )
{
    int x, y=hi-lo+1, z=RAND_MAX/y;

    while ((x=random()) > y*z);
        return x/z+lo;
}

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
int normal ( int loops, int ulimit )
{
    int x=0;
    int i;

    for(i=0; i< loops; i++) {
        x += uniform(0, ulimit);
    }
    return x;
} 

/*********************************************************************
*struct task* makeStream()
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Generate a stream of 1000 tasks with random size and duration then return it to main program as struct of type task
*
*Parameters:
**********************************************************************/
task* makeStream()
{
	//Initialize iterator and stream variable of size 1000
    int i=0;
    static task stream[1000];
	
	srand(time(0));    //Set the seed to the current time for as close to real randomness as possible

	//Main loop to generate tasks
    for(i=0; i<1000; i++)
    {
        stream[i].size = normal(4,4);	//Randomly generates task size

        if( stream[i].size < 1 ) 		//If task size is smaller than 1, set it to 1
            stream[i].size = 1;

        stream[i].time = normal(4,4);	//Randomly generates task duration

        if( stream[i].time < 1 ) 		//If task duration is smaller than 1, set it to 1
            stream[i].time = 1;
        
        //printf("Task %i created with size %i and %i time\n", i, stream[i].size, stream[i].time);

    }

    return stream;		//Returns the now full task stream
}
