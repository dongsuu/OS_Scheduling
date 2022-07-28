/*
 *
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32197454
*	    Student name : Lee Dong Hyun
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){
		int num;

	int init = 65;

	int* arrival_time;

	int* service_time;

	int select;

	int tq;

	int timeslice;

	printf("입력할 process의 개수:");

	scanf("%d", &num);

	char* process = malloc(sizeof(char) * num);

	arrival_time = malloc(sizeof(int) * num);

	service_time = malloc(sizeof(int) * num);

	for (int i = 0; i < num; i++) {

		process[i] = init;

		init++;

	}

	printf("\n Write each processes Arrival time\n");

	for (int i = 0; i < num; i++) {

		printf("%c arrival time: ", process[i]);

		scanf("%d", &arrival_time[i]);

	}

	printf("\n Write each processes Service time\n");

	for (int i = 0; i < num; i++) {

		printf("%c service time: ", process[i]);

		scanf("%d", &service_time[i]);

	}

	

		printf("\n\n");

		printf("Select OS Scheduling method\n");

		printf("1. FCFS    2. SJF    3. STCF    4. RR\n");

		printf("5. HRRN    6. MLFQ   7. lottery    \n");

		printf("write number of scheduling method: ");

		scanf("%d", &select);

 

		switch (select) {

		case 1:

			FCFS(process, arrival_time, service_time, num);

			break;

		case 2:

			SJF(process, arrival_time, service_time, num);

			break;

		case 3:

			STCF(process, arrival_time, service_time, num);

			break;

		case 4:

			printf("Time Slice: ");

			scanf("%d", &timeslice);

			RR(process, arrival_time, service_time, num, timeslice);

			break;

		case 5:

			HRRN(process, arrival_time, service_time, num);

			break;

		case 6:

			printf("1. Time Quantum = 1     2. Time Quantum = 2^i\n");

			printf("select number of time quantunm: ");

			scanf("%d", &tq);

			if (tq == 1)

				MLFQ1(process, arrival_time, service_time, num, 1);

			else {

				MLFQ2(process, arrival_time, service_time, num);

			}

			break;

		case 7:

			lottery(process, arrival_time, service_time, num);

			break;

		}

 

	return 0;

}
