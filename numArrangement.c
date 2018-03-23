#include<stdio.h>
#include<stdlib.h>

#include<string.h>
#include<pthread.h>

#define DEBUG_PRINTS
#undef DEBUG_PRINTS

#define SIDE_3

#if defined(SIDE_3)
#define ARR_LEN 3
#elif defined(SIDE_5)
#define ARR_LEN 5
#endif

#define UP  5
#define DOWN 6
#define LEFT 7
#define RIGHT 8

volatile char a;
volatile int exitPrinting = 0;
pthread_mutex_t mutexLock;
pthread_cond_t cv;

#if defined(SIDE_3)
volatile int arr_vals[ARR_LEN][ARR_LEN] = {0,1,2,3,5,7,8,6,4};
#elif defined(SIDE_5)
volatile int arr_vals[ARR_LEN][ARR_LEN] = {0,1,2,3,5,7,8,6,4,9,13,11,14,12,10,18,23,24,19,17,15,16,20,22,21};
#endif

int getIdx()
{
  int i=0,j=0, exitLoop = 0;
  for(i=0; i<ARR_LEN && !exitLoop; i++)
    for(j=0; j<ARR_LEN && !exitLoop; j++)
      if(arr_vals[i][j] == 0)
      {
        exitLoop = 1;
        j--; i--;
      }
  return (i | j<<16);
}

void applyOP(int dir)
{
  int i,j,temp;
  int idx = getIdx();

  i = idx & 0xff;
  j = idx>>16;

  if(dir == LEFT && j!= ARR_LEN-1)
  {
#ifdef DEBUG_PRINTS
    printf("\n LEFT OPERATION ");
#endif
    arr_vals[i][j] = arr_vals[i][j+1];
    arr_vals[i][j+1] = 0;
  }
  else if(dir == UP && i!= ARR_LEN-1)
  {
#ifdef DEBUG_PRINTS
    printf("\n UP OPERATION ");
#endif
    arr_vals[i][j] = arr_vals[i+1][j];
    arr_vals[i+1][j] = 0;
  }
  else if(dir == DOWN && i!=0)
  {
#ifdef DEBUG_PRINTS
    printf("\n DOWN OPERATION ");
#endif
    arr_vals[i][j] = arr_vals[i-1][j];
    arr_vals[i-1][j] = 0;
  }
  else if(dir == RIGHT && j!=0)
  {
#ifdef DEBUG_PRINTS
    printf("\n RIGHT OPERATION ");
#endif
    arr_vals[i][j] = arr_vals[i][j-1];
    arr_vals[i][j-1] = 0;
  }
  else
  {
#ifdef DEBUG_PRINTS
    printf("Invalid operation");
#endif
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
#ifdef DEBUG_PRINTS
    default: printf("\n Invalid option \n");
#endif
  }
}

void printArray()
{
  int i,j;
#ifdef DEBUG_PRINTS
  printf("\n ARRAY IS :\n");
#endif
  /* Clear screen */
  printf("\033[H\033[J");

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
#ifdef DEBUG_PRINTS
    printf("\n Inside readThread, enter a: ");
#endif

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
    usleep(50000); /* Sleep for 50ms */
  }
}

void* writeThread(void* vargp)
{
  while(!exitPrinting)
  {
    pthread_mutex_lock(&mutexLock);

#ifdef DEBUG_PRINTS
    printf("\n Inside writeThread : ");
    printf("%c\n",a);
#endif

    updateArray(a);
    printArray();

    pthread_cond_signal(&cv);

    pthread_mutex_unlock(&mutexLock);
    usleep(100000); /* Sleep for 100ms */
  }
}

int main()
{
  pthread_t readT, writeT;

  pthread_mutex_init(&mutexLock, NULL);
  pthread_cond_init(&cv, NULL);

  /* print array */
  printArray();

  pthread_create(&readT, NULL, &readThread, NULL);
  pthread_create(&writeT, NULL, &writeThread, NULL);

  pthread_join(readT, NULL);
  pthread_join(writeT, NULL);

  printf("\n Exiting both threads\n");

  pthread_cond_destroy(&cv);
  pthread_mutex_destroy(&mutexLock);

  return 0;
}

