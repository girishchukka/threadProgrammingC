#include<stdio.h>
#include<stdlib.h>

#include<string.h>
#include<pthread.h>

volatile char a;
volatile int exitRead = 0, printVal = 1, readVal = 0;
pthread_mutex_t mutexLock;

void* readThread(void* vargp)
{
  while(1)
  {
    while(!printVal);
    scanf("%c", &a);
    readVal = 1;
    printVal = 0;
    if(a=='e')
      break;
  }
  exitRead = 1;
}

void* writeThread(void* vargp)
{
  while(!exitRead)
  {
    while(!readVal);
    printf("\t%c",a);
    printVal = 1;
    readVal = 0;
  }
}

int main()
{
  pthread_t readT, writeT;

  pthread_mutex_init(&mutexLock, NULL);

  pthread_create(&readT, NULL, &readThread, NULL);
  pthread_create(&writeT, NULL, &writeThread, NULL);

  pthread_join(readT, NULL);
  pthread_join(writeT, NULL);

  pthread_mutex_destroy(&mutexLock);

  return 0;
}

