/*
	* DKU Operating System Lab
	* Lab1 (Scheduler Algorithm Simulator)
	* Student id :32197454
	* Student name : LeeDongHyun
	*
	* lab1_sched.c :
	* - Lab1 source file.
	* - Must contains scueduler algorithm function'definition.
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
	#include <math.h>
	
	#include "lab1_sched_types.h"
	
	/*
	* you need to implement FCFS, RR, SPN, HRRN, MLFQ scheduler.
	*/
// **OS의 Scheduling 방식**



//FCFS(FIFO) 먼저도착한 것을 실행.

void FCFS(char process[], int arrival_time[], int service_time[], int n) {

	Queue q;

	QueueInit(&q);

	arrive_sort(process, arrival_time, service_time, n); //도착시간 순서로 정렬해둔다.



	int time = 0;

	int current = 0;

	int flag = 0;

	int count = 0;

	int sum = 0;

	for (int i = 0; i < n; i++) { // 모든 service_time의 합 계산.

		sum += service_time[i];

	}

	while (time != arrival_time[0]) //가장 먼저 도착한 process까지 time을 증가시킨다.

		time++;



	while (1) {

		if (time == arrival_time[count]) { //process의 도착

			Enqueue(&q, process[current]);

			count++;

		}



		if (flag == 0) { //수행하고 있는 작업 없을 때

			Dequeue(&q);

			printf("%c ", process[current]);

			service_time[current]--;

			time++;

			flag = 1;

			continue;

		}

		else { //수행중인 작업이 있을 때

			printf("%c ", process[current]);

			service_time[current]--;

			if (service_time[current] == 0) {

				current++;

				flag = 0;

			}

			time++;

		}

		if (time == sum) //종료

			break;

	}

}



//SJF방식. 먼저도착한 것을 먼저 실행하지만, 만약 Queue에 여러개 있으면 수행 시간 짧은 것 부터 수행한다.

void SJF(char process[], int arrival_time[], int service_time[], int n) {

	Queue q;

	QueueInit(&q);

	arrive_sort(process, arrival_time, service_time, n); //도착순서 정렬



	int time = 0;

	int sum = 0;

	int count = 0;

	int current = 0;

	int flag = 0;



	for (int i = 0; i < n; i++) { //서비스 시간 총 합

		sum += service_time[i];

	}

	while (time != arrival_time[0]) //time과 처음 process 도착 시간 동일하게.

		time++;



	while (1) {

		if (time == arrival_time[count]) { //새 process 도착.

			Enqueue(&q, process[count]);

			count++;

		}



		if (flag == 0) { //수행하고 있는 작업 없을 때

			Dequeue(&q);

			printf("%c ", process[current]);

			service_time[current]--;

			time++;

			flag = 1;

			continue;

		}

		else { //수행중인 작업이 있을 때

			printf("%c ", process[current]);

			service_time[current]--;

			if (service_time[current] == 0) {

				current++;

				flag = 0;



				if (count - current > 1) { //queue에 여러개 있으면 service time순서로 정렬하여 짧은게 먼저 실행되게끔.

					service_sort(process, arrival_time, service_time, n, current, count);

					while (!QIsEmpty(&q))

						Dequeue(&q);

					for (int i = current; i < count; i++) {

						Enqueue(&q, process[i]);

					}

				}

			}

			time++;

		}

		if (time == sum)

			break;

	}



}



//STCF. 선점이 가능하다. 실행하다가 process 도착했는데, service_time 더 짧으면 원래 수행하는거 다시 ready상태로.(queue)

void STCF(char process[], int arrival_time[], int service_time[], int n) {

	Queue q;

	QueueInit(&q);

	arrive_sort(process, arrival_time, service_time, n); //도착 순서로 정렬



	int time = 0;

	int sum = 0;

	int count = 0;

	int current = 0;

	int flag = 0;



	for (int i = 0; i < n; i++) { //서비스 시간 총 합

		sum += service_time[i];

	}

	while (time != arrival_time[0]) //time과 첫 도착 process 시간 동일하게.

		time++;



	while (1) {

		if (time == arrival_time[count]) { //새 process 도착

			Enqueue(&q, process[count]);



			if (service_time[current] > service_time[count]) { //도착한 process의 service_time이 더 작을 때. (선점)

				if (flag != 0) {

					Dequeue(&q);

					Enqueue(&q, process[current]);

				}

				if (count - current > 0) {

					service_sort(process, arrival_time, service_time, n, current, count+1);

				}

				flag = 0;

			}

			count++;

		}



		if (flag == 0) { //수행하고 있는 작업 없을 때

			Dequeue(&q);

			printf("%c ", process[current]);

			service_time[current]--;

			time++;

			flag = 1;

			continue;

		}

		else { //수행중인 작업이 있을 때

			printf("%c ", process[current]);

			service_time[current]--;

			if (service_time[current] == 0) {

				current = QPeek(&q) - 65;

				flag = 0;

				if (count - current > 1) { //SJF와 동일한 방식. Queue에 여러개 있으면 서비스 시간 순서로 sorting

					service_sort(process, arrival_time, service_time, n, current, count);

					while (!QIsEmpty(&q))

						Dequeue(&q);

					for (int i = current; i < count; i++) {

						Enqueue(&q, process[i]);

					}

				}

			}

			time++;

		}

		if (time == sum)

			break;

	}

}



//Round-Robin 방식. timeslice만큼 작업을 수행시킨다.

void RR(char process[], int arrival_time[], int service_time[], int n, int timeslice) {

	Queue q;

	QueueInit(&q);

	arrive_sort(process, arrival_time, service_time, n); //도착 순으로 정렬



	int ts = timeslice;

	int time = 0;

	int sum = 0;

	int count = 0;



	for (int i = 0; i < n; i++) {

		sum += service_time[i];

	}

	while (time != arrival_time[0])

		time++;

	char temp = NULL;



	while (1) {

		while (timeslice != 0) { //time slice만큼 반복

			if (time == arrival_time[count]) {

				Enqueue(&q, process[count]);

				count++;

			}

			if (service_time[temp - 65] != 0 && temp != NULL) {

				Enqueue(&q, temp);

				temp = NULL;

			}



			printf("%c ", QPeek(&q));

			service_time[QPeek(&q) - 65]--; //QPeek()에는 A,B,C 들어있어서 index값 뽑아내기 위해 -65 진행.

			time++;

			timeslice--;

			if (service_time[QPeek(&q) - 65] == 0) { //서비스 시간 다 썼을 때

				Dequeue(&q);

				temp = NULL;

				break;

			}

			if (service_time[QPeek(&q) - 65] != 0 && timeslice == 0) { //서비스 시간은 0이 아닌데 주어진 time slice 다 썼을 때

				temp = QPeek(&q);

				Dequeue(&q);



			}



		}

		timeslice = ts;

		if (time == sum)

			break;

	}

}



//HRRN. Response Ratio가 큰 것부터 scheduling한다.

void HRRN(char process[], int arrival_time[], int service_time[], int n) {

	Queue q;

	QueueInit(&q);

	arrive_sort(process, arrival_time, service_time, n);



	int time = 0;

	int sum = 0;

	int count = 0;

	int current = 0;

	int flag = 0;

	int q_count = 0;



	for (int i = 0; i < n; i++) { //서비스 시간 총합

		sum += service_time[i];

	}

	while (time != arrival_time[0])

		time++;



	while (1) {

		if (time == arrival_time[count]) {  //새로운 process 도착

			Enqueue(&q, process[count]);

			count++;

		}



		if (flag == 0) { //수행하고 있는 작업 없을 때

			Dequeue(&q);

			printf("%c ", process[current]);

			service_time[current]--;

			time++;

			flag = 1;

			continue;

		}

		else { //수행중인 작업이 있을 때

			printf("%c ", process[current]);

			service_time[current]--;

			if (service_time[current] == 0) {

				current++;

				flag = 0;

				if (count - current > 1) { //큐에 여러개 있을 때, response ratio 계산하여 priority 부여

					priority_sort(process, arrival_time, service_time, n, current, count, time+1);

					while (!QIsEmpty(&q))

						Dequeue(&q);

					for (int i = current; i < count; i++) {

						Enqueue(&q, process[i]);

						q_count++;

					}

				}

			}

			time++;

		}

		if (time == sum)

			break;

	}

}



//MLFQ (time quantum = 1)

void MLFQ1(char process[], int arrival_time[], int service_time[], int n, int q) {

	int q_count;

	printf("queue개수 입력: "); //queue 개수 입력받기.

	scanf("%d", &q_count);

	Queue* queue = (Queue*)malloc(sizeof(Queue) * q_count);

	int* qindex = malloc(sizeof(int) * q_count); //qindex[0]은 A의 queue위치, ... , qindex[n]은 'A+n'의 queue위치

	q = 1;





	for (int i = 0; i < q_count; i++) {

		QueueInit(&queue[i]);

		qindex[i] = 0;

	}

	arrive_sort(process, arrival_time, service_time, n);

	int ts = q + 1;

	int time = 0;

	int sum = 0;

	int count = 0;

	int current = 0;



	for (int i = 0; i < n; i++) {

		sum += service_time[i];

	}

	while (time != arrival_time[0])

		time++;



	char temp = NULL;



	while (1) {

		while (q != 0) {

			if (time == arrival_time[count]) { //새 process 도착

				Enqueue(&queue[0], process[count]);

				count++;

			}



			for (int i = 0; i < q_count; i++) {

				if (QIsEmpty(&queue[i])) {  //여기서 몇번째 queue를 실행시킬지 정한다.

					continue;

				}

				if (!QIsEmpty(&queue[i])) {

					current = QPeek(&queue[i]) - 65;

					printf("%c ", QPeek(&queue[qindex[current]]));

					service_time[QPeek(&queue[qindex[current]]) - 65]--;

					time++;

					q--;

					if (service_time[QPeek(&queue[qindex[current]]) - 65] != 0 && q == 0) {

						temp = QPeek(&queue[qindex[current]]);

						Dequeue(&queue[qindex[current]]);



						//Queue에 아무것도 없으면 앞서 출력했던 것(temp) 출력

						int c = 0;

						for (int i = 0; i < q_count; i++) {

							if (QIsEmpty(&queue[i])) {

								c++;

							}

							else

								break;

						}

						if (c == q_count && arrival_time[count] != time) { //queue에 아무것도 없으면서 다음시간에 도착하는거 없을 때

							Enqueue(&queue[qindex[current]], temp);

						}



						if(c!= q_count || arrival_time[count] == time){ //일반적인 경우

							qindex[current]++;

							Enqueue(&queue[qindex[current]], temp);

						}

					}

					if (service_time[QPeek(&queue[qindex[current]]) - 65] == 0) {

						Dequeue(&queue[qindex[current]]);

						break;

					}

					break;

				}

			}

		}

		q = ts - 1;

		if (time == sum)

			break;

	}

}



//MLFQ (Time Quantum = 2^i)

void MLFQ2(char process[], int arrival_time[], int service_time[], int n) {

	int q_count;

	printf("queue개수 입력: ");

	scanf("%d", &q_count);

	Queue* queue = (Queue*)malloc(sizeof(Queue) * q_count);

	int* qindex = malloc(sizeof(int) * q_count); //qindex[0]은 A의 queue위치, ... , qindex[n]은 'A+n'의 queue위치

	int* qtime_slice = malloc(sizeof(int) * q_count);  //각 queue마다 time quantum 값 다르므로 각각 정해줌.


	int two = 1;
	for (int i = 0; i < q_count; i++) {

		QueueInit(&queue[i]);

		queue[i].q = two; //2^i의 time quantum 부여.
		two = two * 2;
		qindex[i] = 0;

	}



	arrive_sort(process, arrival_time, service_time, n);

	int time = 0;

	int sum = 0;

	int count = 0;

	int current = 0;

	int quan_count = 0;

	int tempi = 0;

	char late_enqueue = NULL;



	for (int i = 0; i < n; i++) {

		sum += service_time[i];

	}

	while (time != arrival_time[0])

		time++;

	char temp = NULL;



	while (1) {

		while (queue[tempi].q != 0) { //여기서는 현재 수행중인 queue의 time quantum만큼 while문이 반복된다.

			if (time == arrival_time[count] && quan_count == 0) {

				Enqueue(&queue[0], process[count]);

				count++;

			}



			for (int i = 0; i < q_count; i++) {

				tempi = i;

				if (QIsEmpty(&queue[i])) {  //해당 큐 정하기

					continue;

				}

				if (time == arrival_time[count] && quan_count != 0) { //time quantum을 아직 다 쓰지 않았을 때는 도착한 것을 늦게 enqueue한다.

					late_enqueue = process[count];                   //이유는 위의 QIsEmpty()에 걸리지 않게 해야하므로

					Enqueue(&queue[0], late_enqueue);

					count++;

				}



				if (!QIsEmpty(&queue[i])) { //큐가 비어있지 않을 때

					current = QPeek(&queue[i]) - 65;

					printf("%c ", QPeek(&queue[qindex[current]]));

					service_time[QPeek(&queue[qindex[current]]) - 65]--;

					time++;

					queue[tempi].q--;

					quan_count++;

					if (service_time[QPeek(&queue[qindex[current]]) - 65] != 0 && queue[tempi].q == 0) {

						temp = QPeek(&queue[qindex[current]]);

						Dequeue(&queue[qindex[current]]);



						//Queue에 아무것도 없으면 앞서 출력했던 것 출력

						int c = 0;

						for (int i = 0; i < q_count; i++) {

							if (QIsEmpty(&queue[i])) {

								c++;

							}

							else

								break;

						}

						if (c == q_count && arrival_time[count] != time) { //큐가 비어있을 때 temp enqueue.

							Enqueue(&queue[qindex[current]], temp);

						}



						if (c != q_count || arrival_time[count] == time) { //일반적인 경우

							qindex[current]++;

							Enqueue(&queue[qindex[current]], temp);

						}

					}

					if (service_time[QPeek(&queue[qindex[current]]) - 65] == 0) { //서비스 시간 다 쓴 경우

						while (queue[tempi].q != 0) {

							queue[tempi].q--;

							quan_count++;

						}

						Dequeue(&queue[qindex[current]]);

						break;

					}

				}

				break;

			} //for

			if (queue[tempi].q == 0)

				break;

		}//while

		//초기화 작업들

		late_enqueue = NULL;

		queue[tempi].q += (quan_count);

		quan_count = 0;



		if (time == sum) //종료

			break;

	}

}



//lottery함수. ticket 할당량을 가지고 scheduling을 한다.

void lottery(char process[], int arrival_time[], int service_time[], int n) {

	int current = 0;

	int totaltickets;

	int assign;

	int* ticket = malloc(sizeof(int) * n);

	int t = 0;

	int sum = 0;

	int winner;

	for (int i = 0; i < n; i++) {

		sum += service_time[i];

	}

	printf("Total Tickets(정수)을 입력하세요: ");

	scanf("%d", &totaltickets); //전체 ticket 입력

	printf("*****Ticket 할당 (Sum of each ticket assign have to be Total Ticket)*****\n");



	for (int i = 0; i < n; i++) {  //각 process의 ticket 할당

		printf("%c ticket assign: ", process[i]);

		scanf("%d", &assign);

		ticket[i] = assign;

	}

	srand(time(NULL));

	int counter = 0;

	while (1) {

		winner = rand() % totaltickets;  //랜덤넘버로 winner번호 생성

		while (1) {

			counter = counter + ticket[current];

			if (counter > winner)  //이때의 current가 scheduling 된다.

				break;

			current += 1;

		}

		printf("%c ", process[current]);

		t++;

		current = 0;

		counter = 0;

		if (t == sum)

			break;

	}

}
//도착시간 순으로 정렬

void arrive_sort(char process[], int arrival_time[], int service_time[], int n) {

	int temp1, temp2, temp3;

	for (int i = 0; i < n; i++) {

		for (int j = i + 1; j < n; j++)

			if (arrival_time[i] > arrival_time[j]) {

				temp1 = arrival_time[j];

				arrival_time[i] = arrival_time[j];

				arrival_time[j] = temp1;

 

				temp2 = process[i];

				process[j] = process[i];

				process[j] = temp2;

 

				temp3 = service_time[i];

				service_time[j] = service_time[i];

				service_time[j] = temp3;

			}

	}

}

// 서비스 시간 순서로 정렬 (선점을 위해 필요)

void service_sort(char process[], int arrival_time[], int service_time[], int n, int cur, int count) {

	int temp1, temp2, temp3;

 

	for (int i = cur; i < count; i++) {

		for (int j = i + 1; j < count; j++) {

			if (service_time[i] == service_time[j]) {

				if (process[i] > process[j]) {

					temp1 = service_time[i];

					service_time[i] = service_time[j];

					service_time[j] = temp1;

 

					temp2 = process[i];

					process[i] = process[j];

					process[j] = temp2;

 

					temp3 = arrival_time[i];

					arrival_time[i] = arrival_time[j];

					arrival_time[j] = temp3;

				}

			}

			if (service_time[i] > service_time[j]) {

				temp1 = service_time[i];

				service_time[i] = service_time[j];

				service_time[j] = temp1;

 

				temp2 = process[i];

				process[i] = process[j];

				process[j] = temp2;

 

				temp3 = arrival_time[i];

				arrival_time[i] = arrival_time[j];

				arrival_time[j] = temp3;

			}

		}

	}

}

//HRRN 구현을 위해 Response_ratio의 순서로 sorting하는 작업이 필요

void response_sort(double response_ratio[], char process[], int arrival_time[], int service_time[], int cur, int count) {

	int temp1, temp2, temp3;

	for (int i = cur; i < count; i++) {

		for (int j = i + 1; j < count; j++) {

			if (response_ratio[i] < response_ratio[j]) {

				temp1 = service_time[i];

				service_time[i] = service_time[j];

				service_time[j] = temp1;

 

				temp2 = process[i];

				process[i] = process[j];

				process[j] = temp2;

 

				temp3 = arrival_time[i];

				arrival_time[i] = arrival_time[j];

				arrival_time[j] = temp3;

			}

		}

	}

}

//HRRN의 response_time 계산 및 sorting

void priority_sort(char process[], int arrival_time[], int service_time[], int n, int cur, int count, int time) {
	double *response_ratio = malloc(sizeof(double) * n);

	double *wait_time = malloc(sizeof(double)*n);

 

	for (int i = cur; i < count; i++) {

		wait_time[i] = (double)time - (double)arrival_time[i];

		response_ratio[i] = (wait_time[i] + (double)service_time[i]) / (double)service_time[i];

	}

	response_sort(response_ratio, process, arrival_time, service_time, cur, count);
}


//node, queue based on linked list
void QueueInit(Queue* pq) //큐의 초기화

{

	pq->front = NULL; //처음 큐는 비어 있는 상태이므로 front와 rear 모두 null을 가리킨다. 

	pq->rear = NULL;

}

 

int QIsEmpty(Queue* pq)      //큐가 비어있는지 확인

{

	if (pq->front == NULL)     //front가 null을 가리키고 있으면 큐가 비어있는 상태임.(front가 빼낼 때 쓰이는데 빼낼것이 null이므로)

		return TRUE;          //비어있으면 TRUE(1) 반환

	else

		return FALSE;         //아니면 FALSE(0) 반환

}

 

void Enqueue(Queue* pq, Data data)  //Enqueue 연산

{

	Node* newNode = (Node*)malloc(sizeof(Node));     //새 노드의 생성

	newNode->next = NULL;     //새 노드는 null을 가리킨다.

	newNode->data = data;     //새 노드에 data 삽입

 

	if (QIsEmpty(pq))         //첫 번째 노드를 삽입할 때(큐가 비어있을 때)

	{

		pq->front = newNode;   //front와 rear 모두 새 노드를 가리켜야한다. 

		pq->rear = newNode;

	}

	else                    //두 번째 노드의 삽입부터는 front는 고정이다.

	{

		pq->rear->next = newNode;     //노드의 연결을 위해 rear의 next는 새 노드를 가리키게 한다. 

		pq->rear = newNode;           //rear는 이제 새 노드를 가리킨다. 

	}

}

 

Data Dequeue(Queue* pq)     //Dequeue 연산

{

	Node* delNode;    //삭제할 노드의 포인터 변수

	Data retData;      //삭제할 노드의 데이터 저장할 변수

 

	if (QIsEmpty(pq))        //큐가 비어있으면 삭제할 것이 없으므로 에러

	{

		printf("Queue Memory Error!");

		exit(-1);

	}

 

	delNode = pq->front;     //삭제할 노드는 front로 제어한다.(먼저 들어온것이 먼저 나가는 구조이므로)

	retData = delNode->data;  //삭제할 노드의 데이터 retData에 저장

	pq->front = pq->front->next;   //삭제를 했으니 front가 front->next를 가리키게 하여 다음 노드로 넘어간다. 

 

	free(delNode);      //노드 삭제

	return retData;     //삭제한 노드의 데이터 반환

}

 

Data QPeek(Queue* pq)     //Peek 연산. 삭제와 동일한데 삭제하지는 않고 데이터만 반환.

{

	if (QIsEmpty(pq))

	{

		printf("Queue Memory Error!");

		exit(-1);

	}

 

	return pq->front->data;

}
