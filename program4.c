/*********************************************************************
*File: program4.c
*Author: Jose Lamuno
*Procedures:
*main	-test program which initializes data elements, memory, then calls on the memory allocation algorithms. Results are then printed for the user.
**********************************************************************/

#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include<sys/time.h>
#include "memoryMake.h"
#include "streamMake.h"
#include "memAlgorithms.h"
#include "memAlgorithmsCompact.h"

/*********************************************************************
*void main()
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Generates a task stream, generates a memory, runs allocation algorithms and then prints the results.
**********************************************************************/
void main() 
{ 
    system("clear");	//Clears the terminal for better view

    int Time[4] = {0,0,0,0};	//Initialize array to hold times for all four algorithms

    int i = 0;	//Initialize iterator
    
    task* stream = makeStream();   //Initialize task stream to 1000 tasks with random sizes and times

    //Ask the user whether they would like random memory block assignment or predetermined and initialize predetermined memory
    int blocks = 6;		//6 blocks for predetermined memory
    int choice = 0;		//User choice for predetermined or random partitions
	int compact = 0;	//User choice for compactment or no
    int* memory = malloc(10*sizeof(int));	//Allocates space for a memory of up to 10 partitions

	//Prints two prompts for the user to input their choices
    printf("Enter 0 for randomized fixed allocation memory. \nOr anything else for a predetermined fixed allocation memory.\n");
    scanf("%i", &choice);

	printf("\nEnter 0 for compactment or anything else for no compactment\n(note compactment will take longer real time)\n");
    scanf("%i", &compact);

	//Checks the user choice for memory
    if(choice < 1){
        memory = randomMem(&blocks);	//Generates a memory with random partitions and partition size
    }
    else{		//Initializes pretermined fixed partition memory if user didnt want random and lets the user know what the memory looks like
        memory[0] = 4;
        memory[1] = 4;
        memory[2] = 8;
        memory[3] = 12;
        memory[4] = 12;
        memory[5] = 16;
        printf("\nMemory allocated with %i blocks of sizes: ", blocks);
        for(i=0; i<blocks; i++)
            printf("\nBlock %i of size %i", (i+1),memory[i]);
    }

	printf("\nLoading... 0%% ");
	fflush(stdout);

    //Main experimental loop that will run 1000 times
	if(compact < 1){	//If the user chose compaction run the loop using algorithms with compaction
		for(i=0; i<1000; i++)
    	{
        	Time[0] += bestFitCompact(stream, memory, blocks);		//Runs the best fit algorithm with compaction

        	//printf("\nTime elapsed for best fit: %i", Time[0]);

        	Time[1] += firstFitCompact(stream, memory, blocks);		//Runs the first fit algorithm with compaction

        	//printf("\nTime elapsed for first fit: %i", Time[1]);

        	Time[2] += nextFitCompact(stream, memory, blocks);		//Runs the next fit algorithm with compaction

        	//printf("\nTime elapsed for next fit: %i \n", Time[2]);

        	Time[3] += worstFitCompact(stream, memory, blocks);		//Runs the worst fit algorithm with compaction
        	stream = makeStream();		//Makes a new stream with random tasks
			if(i%100 == 0 && i>0){
				printf("\b\b\b%i%%", i/10);
				fflush(stdout);
			}
		}
	}
	else{	//If the user didn't choose compaction runs the loop using algorithms without compaction
		for(i=0; i<1000; i++)
    	{
        	Time[0] += bestFit(stream, memory, blocks);		//Runs the best fit algorithm

        	//printf("\nTime elapsed for best fit: %i", Time[0]);

        	Time[1] += firstFit(stream, memory, blocks);	//Runs the first fit algorithm

        	//printf("\nTime elapsed for first fit: %i", Time[1]);

        	Time[2] += nextFit(stream, memory, blocks);		//Runs the next fit algorithm

        	//printf("\nTime elapsed for next fit: %i \n", Time[2]);

        	Time[3] += worstFit(stream, memory, blocks);		//Runs the worst fit algorithm
        	stream = makeStream(); 		//Makes a new stream with random tasks
			if(i%100 == 0 && i>0){
				printf("\b\b\b%i%%", i/10);
				fflush(stdout);
			}
    	}
	}   
	
	printf("\b\b\b100%%");

	//Prints the average times for all the algorithms
    printf("\n\nAverage time for best-fit: %i \n", (Time[0]/1000));
    printf("Average time for first-fit: %i \n", (Time[1]/1000));
    printf("Average time for next-fit: %i \n", (Time[2]/1000));
    printf("Average time for worst-fit: %i \n", (Time[3]/1000));
}
