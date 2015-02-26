#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int     count = 0;
int     thread_ids[3] = {0,1,2};
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

pthread_cond_t tSignals[NUM_THREADS];
pthread_cond_t done;
int running =1; //scheduling flag, 


void *inc_count(void *t) 
{
  int i,j,sum;
  long my_id = (long)t;

  for (i=0; i<TCOUNT; i++) {
    pthread_mutex_lock(&count_mutex);
    count++;

    /* 
    Check the value of count and signal waiting thread when condition is
    reached.  Note that this occurs while mutex is locked. 
    */
    if (count == COUNT_LIMIT) {
      pthread_cond_signal(&count_threshold_cv);
      printf("inc_count(): thread %ld, count = %d  Threshold reached.\n", 
             my_id, count);
      }
    printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", 
	   my_id, count);
    pthread_mutex_unlock(&count_mutex);

    /* Do some "work" so threads can alternate on mutex lock */
    for (j = 0; j<0xFFFF; j++ ){
        sum +=1;
    }
    }
  pthread_exit(NULL);
}

void *watch_count(void *t) 
{
  long my_id = (long)t;

  printf("Starting watch_count(): thread %ld\n", my_id);

  /*
  Lock mutex and wait for signal.  Note that the pthread_cond_wait 
  routine will automatically and atomically unlock mutex while it waits. 
  Also, note that if COUNT_LIMIT is reached before this routine is run by
  the waiting thread, the loop will be skipped to prevent pthread_cond_wait
  from never returning. 
  */
  pthread_mutex_lock(&count_mutex);
  while (count<COUNT_LIMIT) {
    pthread_cond_wait(&count_threshold_cv, &count_mutex);
    printf("watch_count(): thread %ld Condition signal received.\n", my_id);
    count += 125;
    printf("watch_count(): thread %ld count now = %d.\n", my_id, count);
    }
  pthread_mutex_unlock(&count_mutex);
  pthread_exit(NULL);
}

void spining(int inc) {
	int i;
	unsigned int sum;
	for (i=0;i<(0xFF*(1+inc));i++) {
		sum +=1;
	}
	printf("[%d] releasing. Tick=%d\n",inc,sum);
}

void * t_run(void * p) {
	unsigned int i;
	unsigned int sum=0;
	int pI = (int)p;
	clock_t start, finish;
  do {
	pthread_mutex_lock(&count_mutex);
	pthread_cond_wait(&tSignals[pI], &count_mutex); //>200ms delay between sig and exe
	start = clock();
	printf("start: %lu\n",start);
	printf("[%d] is executing\n",pI);
//	for (i=0;i<(0xFF*(1+pI));i++) {
//	pthread_mutex_lock(&count_mutex);
//	pthread_cond_wait(&tSignals[pI], &count_mutex);
//	printf("%d\n",pI);
//		sum +=1;
//	pthread_mutex_unlock(&count_mutex);
//	}
//	printf("[%d] releasing. Tick=%d\n",pI,sum);
	spining(pI);
	finish =clock();
	printf("finish: %lu\n",finish);
	pthread_mutex_unlock(&count_mutex);
}while(running);
	finish=clock();
	printf("[%d] got done flag: %lu \n",pI,finish);
  	pthread_exit(NULL);
}

void * sched_threads( void* num) {
  int i,j;
  unsigned int sum;
  clock_t start,finish;
  int numT = (int)num;
  unsigned int prev = -1;
  start =clock();
  for (i =2; i>= 0; i--) {
	pthread_mutex_lock(&count_mutex);
	//sig each thread to run
/*	if(prev != -1) {
		//release previous
		pthread_cond_signal(&tSignals[prev]);
	}
*/
	printf("%lu:: go thread %d\n",clock(),i);
	pthread_cond_signal(&tSignals[i]);
	pthread_mutex_unlock(&count_mutex);
	//printf("go thread %d\n",i);
	
	//pthread_mutex_unlock(&count_mutex);
        //wait sum time
	pthread_mutex_lock(&count_mutex);
	sum =0;
   	for (j=0; j<0xFFFF; j++) {
		sum +=1;
	}	
	pthread_mutex_unlock(&count_mutex);
  }
  running =0; //has to signal each thread once more to recognize running flag
	//scheduling some more
  for (i =0; i< 3; i++) {
        pthread_mutex_lock(&count_mutex);
        //sig each thread to run
        printf("go thread %d\n",i);
        pthread_cond_signal(&tSignals[i]);
        pthread_mutex_unlock(&count_mutex);
        //printf("go thread %d\n",i);

        //pthread_mutex_unlock(&count_mutex);
        //wait sum time
        pthread_mutex_lock(&count_mutex);
	sum =0;
        for (j=0; j<0xFFFF; j++) {
                sum +=1;
        }      
        pthread_mutex_unlock(&count_mutex);
  }

	finish=clock();
	printf("Done scheduling, sig done:%lu\n",finish);
//	running =0;
	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
  int rc;
  long i;
  long t1=1, t2=2, t3=3;
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  pthread_t scheduler;

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&count_mutex, NULL);
  //pthread_cond_init (&count_threshold_cv, NULL);
  for(i=0; i<NUM_THREADS; i++) {
    pthread_cond_init (&tSignals[i],NULL);
  }
  pthread_cond_init(&done,NULL); //quit cond
printf("conditions initiated\n");
  /* For portability, explicitly create threads in a joinable state */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  //pthread_create(&threads[0], &attr, watch_count, (void *)t1);
  //pthread_create(&threads[1], &attr, inc_count, (void *)t2);
  //pthread_create(&threads[2], &attr, inc_count, (void *)t3);
  for (i =0; i<NUM_THREADS;i++) {//task threads
    pthread_create(&threads[i], &attr, t_run, (void *)i);
  }
  pthread_create(&scheduler, &attr, sched_threads, (void*) NUM_THREADS);
  printf("Threads created\n");

  /* Wait for all threads to complete */
   pthread_join(scheduler,NULL); //first scheduler	
  for (i=0; i<NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("Main(): Waited on %d  threads. Done.\n", NUM_THREADS);

  /* Clean up and exit */
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&count_mutex);
  //pthread_cond_destroy(&count_threshold_cv);
  for (i=0; i<NUM_THREADS;i++) {
    pthread_cond_destroy(&tSignals[i]);
  }
  pthread_exit(NULL);

}
