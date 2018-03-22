#include<stdio.h>
#include<stdlib.h>

#include<string.h>
#include<pthread.h>

#define ARR_LEN 3
#define UP  5
#define DOWN 6
#define LEFT 7
#define RIGHT 8

volatile char a;
volatile int exitPrinting = 0;
pthread_mutex_t mutexLock;
pthread_cond_t cv;

volatile int arr_vals[ARR_LEN][ARR_LEN] = {0,1,2,3,5,7,8,6,4};

void applyOP(int dir)
{
  if(dir == LEFT)
  {

  }
  else if(dir == UP)
  {

  }
  else if(dir == DOWN)
  {

  }
  else if(dir == RIGHT)
  {

  }
  else
  {
    printf("Invalid operation");
  }
}

void updateArray(char op)
{
  char temp;
  switch(op)
  {
    case 'a':
    case 'A': applyOP(LEFT);
              break;
    case 'w':
    case 'W': applyOP(UP);
              break;
    case 's':
    case 'S': applyOP(DOWN);
              break;
    case 'd':
    case 'D': applyOP(RIGHT);
              break;
    default: printf("\n Invalid option \n");
  }
}

void printArray()
{
  int i,j;
  printf("\n ARRAY IS :\n");
  for(i=0; i<ARR_LEN; i++)
  {
    for(j=0; j<ARR_LEN; j++)
    {
      printf("%4d", arr_vals[i][j]);
    }
    printf("\n");
  }
}

void* readThread(void* vargp)
{
  while(1)
  {
    pthread_mutex_lock(&mutexLock);
    printf("\n Inside readThread, enter a: ");

    /* change std terminal read property
     * In raw mode, enter is not needed to
     * finish reading character
     */
    system("/bin/stty raw");
    a = getchar();
    system("/bin/stty cooked");

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

    updateArray(a);
    printArray();

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

