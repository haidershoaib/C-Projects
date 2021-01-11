#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <mach/boolean.h>

#define MAX 100
#define MIN 100


struct element{
    pid_t pid;
    int time;
};

struct element intArray[MAX];
struct element intArray1[MIN];

int front = 0;
int front1 = 0;
int rear = -1;
int rear1 = -1;
int itemCount = 0;
int itemCount1 = 0;
boolean_t flag = FALSE;

int sum; /* this data is shared by the thread(s) */
void *runner(void *param); /* threads call this function */
void *runner1(void *param);

struct element peek();
struct element peek1();
bool isEmpty();
bool isEmpty1();
bool isFull();
bool isFull1();
int size();
int size1();
void insert(struct element data);
void insert1(struct element data);
struct element dequeue();
struct element dequeue1();



int main(int argc, char *argv[])
{
    struct element e;
    for (int i = 1; i < 101; i++) {
        e.pid = i;
        e.time = 1 + rand() % (30 + 1 - 1);
        insert(e);

    }
    printf("Original Queue with 100 elements:\n");
    struct element e1;
    for (int r = 0; r < 100 ; r++) {
        e1 = intArray[r];
        printf("PID: %d time: %d\n", e1.pid, e1.time);

    }


    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of thread attributes */
/* set the default attributes of the thread */
    pthread_attr_init(&attr);
    pthread_t tid1; /* the thread identifier */
    pthread_attr_t attr1; /* set of thread attributes */
/* set the default attributes of the thread */
    pthread_attr_init(&attr1);
/* create the thread */

        while (flag == FALSE){
            pthread_create(&tid, &attr, runner, argv[1]);
            pthread_create(&tid1, &attr1, runner1, argv[1]);
        }







/* wait for the thread to exit */


    pthread_join(tid, NULL);
    pthread_join(tid1, NULL);

    printf("\nFinal array:\n");
    for (int q = 0; q < 100; q++)
     {
        struct element n = intArray[q];

        printf("PID: %d time: %d\n", n.pid, n.time);
    }


}
/* The thread will execute in this function */
void *runner(void *param)
{
    int j = 0;
    struct element temp1;
    while(size1() < 5){
        temp1 = dequeue();
        if (temp1.time == 0){
            printf("Element with PID: %d has time zero\n", temp1.pid);
        }
        else{
            insert1(temp1);
        }


        j++;
    }

    pthread_exit(0);
}

void *runner1(void *param){

    struct element temp;

    if(!isEmpty1()) {
        while (!isEmpty1()) {
            temp = dequeue1();

            if (temp.time == 1) {
                temp.time = temp.time - 1;
            } else {
                temp.time = temp.time - 2;

            }
            insert(temp);

        }
    }
    else if(isEmpty1() && intArray[99].time == 0){
        flag = TRUE;
    }

    pthread_exit(0);
}


struct element peek() {

    return intArray[front];
}

struct element peek1() {

    return intArray1[front1];
}

bool isEmpty() {
    return itemCount == 0;
}

bool isEmpty1() {
    return itemCount1 == 0;
}

bool isFull() {
    return itemCount == MAX;
}

bool isFull1() {
    return itemCount1 == MIN;
}

int size() {
    return itemCount;
}

int size1() {
    return itemCount1;
}

void insert(struct element data) {

    if(!isFull()) {

        if(rear == MAX-1) {
            rear = -1;
        }

        intArray[++rear] = data;
        itemCount++;
    }
}

void insert1(struct element data) {

    if(!isFull1()) {

        if(rear1 == MIN-1) {
            rear1 = -1;
        }

        intArray1[++rear1] = data;
        itemCount1++;
    }
}

struct element dequeue() {
    struct element data = intArray[front++];

    if(front == MAX) {
        front = 0;
    }

    itemCount--;
    return data;
}

struct element dequeue1() {
    struct element data = intArray1[front1++];

    if(front1 == MIN) {
        front1 = 0;
    }

    itemCount1--;
    return data;
}