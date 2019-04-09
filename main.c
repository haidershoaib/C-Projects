#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define N 5
#define think 0
#define hungry 1
#define eat 2
#define l (ph_num+4)%N
#define r (ph_num+1)%N

sem_t mutex;
sem_t S[N];

void * philospher(void *num);
void take_fork(int);
void put_fork(int);
void changestate(int);

int state[N];
int phil_num[N]={1,2,3,4,5};

int main()
{
    int i;
    pthread_t thread_id[N];
    sem_init(&mutex, 0, 1);
    for(i=0;i<N;i++)
        sem_init(&S[i], 0, 0);
    for(i=0;i<N;i++)
    {
        pthread_create(&thread_id[i],NULL,philospher,&phil_num[i]);
        printf("Philosopher %d in Thinking State\n", i);
    }
    for(i=0; i<N; i++)
        pthread_join(thread_id[i], NULL);
}

void *philospher(void *num)
{
    for (int i = 1; i <= 500 ; ++i) {


        {
            printf("....................\n");
            printf("\nIteration #%d:\n\n", i);
            int *i = num;
          //  sleep(1);
            take_fork(*i);
           // sleep(0);
            put_fork(*i);
        }
    }
}

void take_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = hungry;
    printf("Philosopher %d in Hungry State\n\n",ph_num);
    changestate(ph_num);
    sem_post(&mutex);
    sem_wait(&S[ph_num]);
    sleep(1);
}

void changestate(int ph_num)
{
    if (state[ph_num] == hungry && state[l] != eat && state[r] != eat)
    {
        state[ph_num] = eat;
        printf("Philosopher %d in Eating State\n\n",ph_num);
        sem_post(&S[ph_num]);
    }
}

void put_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = think;
    printf("Philosopher %d in Thinking State\n\n", ph_num);
    changestate(l);
    changestate(r);
    sem_post(&mutex);
}