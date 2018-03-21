#include<stdio.h>
#include<stdlib.h>

#include<string.h>
#include<pthread.h>

volatile char a;
volatile int exitRead = 0, printVal = 1, readVal = 0;
void* readThread(void* vargp)
{
  while(1)
  {
    while(!printVal);
    scanf("%c", &a);
    readVal = 1;
    printVal = 0;
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

  pthread_create(&readT, NULL, &readThread, NULL);
  pthread_create(&writeT, NULL, &writeThread, NULL);

  pthread_join(readT, NULL);
  pthread_join(writeT, NULL);

  return 0;
}

