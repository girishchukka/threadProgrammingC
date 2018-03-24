#include<stdio.h>
#include<stdlib.h>

#include<pthread.h>

pthread_mutex_t mL;
pthread_cond_t cv;

void* condWait()
{
  pthread_mutex_lock(&mL);

  printf("\n Inside condWait() thread \n");

  printf("\nWaiting for pthread condition\n");
  /* Here, cond_wait(), waits for cond_signal
   * Until it gets a signal, it keeps waiting
   * On wait, Mutex will be unlocked and thread
   * will be added to condition variable wait
   * queue to wakeup
   */
  pthread_cond_wait(&cv, &mL);
  printf("\nGot signal for pthread condition\n");

  pthread_mutex_unlock(&mL);
  usleep(600000);
}

void* condSignal()
{
  sleep(1);
  pthread_mutex_lock(&mL);

  /* If a thread is waiting for condition, then
   * it keeps waiting till some other signals
   * and wakes it up. If no other thread wakes it
   * up, then it keeps waiting
   */
  printf("\n Inside condSignal() thread \n");
  /* without this signal, condWait() waits at pthread_cond_wait() */
  pthread_cond_signal(&cv);
  printf("\n pthread cond is signalled\n");

  pthread_mutex_unlock(&mL);
  sleep(1);
}

int main()
{
  pthread_t cond_wait, cond_signal;

  pthread_mutex_init(&mL, NULL);
  pthread_cond_init(&cv, NULL);

  pthread_create(&cond_wait, NULL, &condWait, NULL);
  pthread_create(&cond_signal, NULL, &condSignal, NULL);

  pthread_join(cond_wait, NULL);
  pthread_join(cond_signal, NULL);

  printf("both threads joined");

  pthread_cond_destroy(&cv);
  pthread_mutex_destroy(&mL);

  return 0;
}
