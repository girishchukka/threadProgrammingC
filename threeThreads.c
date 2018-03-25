#include<stdio.h>
#include<stdlib.h>

#include<pthread.h>

#define MAX_VAL 30

volatile int val=0;
pthread_mutex_t mL;
pthread_cond_t cv;

typedef struct threadInfo
{
  pthread_t tid;
  /* this field is used during thread waiting
   * so making it volatile forces the CPU to
   * access it everytime during read/write
   * operation
   */
  volatile int data;
}threadInfo_t;

void* threadPrint(void* vargp)
{
  volatile threadInfo_t* localT = (threadInfo_t*)vargp;

  while(1)
  {
    pthread_mutex_lock(&mL);
    printf("\nMutex locked by thread = %d", localT->data);
    while(val%3 != localT->data)
    {
      printf("\n waiting, thread = %d, val = %d", localT->data, val);
      pthread_cond_wait(&cv, &mL);
      printf("\n wokeup, thread = %d, val = %d", localT->data, val);
    }

    printf("\n printing Thread = %d, val = %d", localT->data, val);
    val++;
    printf("\n wakeup signal from thread = %d, val=%d", localT->data, val);
    pthread_cond_signal(&cv);
    if(val>MAX_VAL)
    {
      pthread_mutex_unlock(&mL);
      return NULL;
    }
    pthread_mutex_unlock(&mL);
    printf("\nMutex released by thread = %d", localT->data);

    /* If the following sleep time is not inserted,
     * then all values are not getting printed. Why?
     * Answer : localT->data is declared as non volatile
     * So, code is using stale data (read in another
     * thread). Declaring threadInfo.data as volatile
     * forces the CPU to read everytime which rules
     * out the race conditions
     */
    usleep(100);
  }
}

/* This program has race condition
 * Compile this code and run executable in loop
 * at one point it is getting stuck
 * Need to root cause it and fix it
 */
int main()
{
  int i, val=0;
  threadInfo_t t[3];

  pthread_mutex_init(&mL, NULL);
  pthread_cond_init(&cv, NULL);

  for(i = 0; i<3; i++)
  {
    t[i].data = i;
    pthread_create(&t[i].tid, NULL, &threadPrint, (void *)&t[i] );
  }

  for(i=0; i<3; i++)
    pthread_join(t[i].tid, NULL);

  pthread_cond_destroy(&cv);
  pthread_mutex_destroy(&mL);

  return 0;
}
