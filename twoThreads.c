#include<stdio.h>
#include<stdlib.h>

#include<string.h>
#include<pthread.h>

volatile char a;
volatile int exitPrinting = 0;
pthread_mutex_t mutexLock;
pthread_cond_t cv;

void* readThread(void* vargp)
{
  while(1)
  {
    pthread_mutex_lock(&mutexLock);
    printf("\n Inside readThread, enter a: ");

    scanf("%c", &a);

    /* Wait until read value got printed */
    pthread_cond_wait(&cv, &mutexLock);

    if(a =='e')
    {
      /* Exit upon entering 'e'*/
      pthread_mutex_unlock(&mutexLock);
      exitPrinting = 1;
      return NULL;
    }
    pthread_mutex_unlock(&mutexLock);
    usleep(700000);
  }
}

void* writeThread(void* vargp)
{
  while(!exitPrinting)
  {
    pthread_mutex_lock(&mutexLock);

    printf("\n Inside writeThread : ");
    printf("%c\n",a);

    pthread_cond_signal(&cv);

    pthread_mutex_unlock(&mutexLock);
    sleep(1);
  }
}

int main()
{
  pthread_t readT, writeT;

  pthread_mutex_init(&mutexLock, NULL);
  pthread_cond_init(&cv, NULL);

  pthread_create(&readT, NULL, &readThread, NULL);
  pthread_create(&writeT, NULL, &writeThread, NULL);

  pthread_join(readT, NULL);
  pthread_join(writeT, NULL);

  printf("\n Exiting both threads\n");

  pthread_cond_destroy(&cv);
  pthread_mutex_destroy(&mutexLock);

  return 0;
}

