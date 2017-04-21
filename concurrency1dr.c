/******************************************************************************
 * Concurrency Assignment #1
 * CS444
 * Creators: Bradley Imai & Daniel Ross
 * 4/20/2017
 ******************************************************************************/
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <pthread.h>
 #include <limits.h>
 #include "mt19937ar.h"

//check to see if the buffer is empty or not. buffer can only hold 32 items

//producer will need to wait a random time between 3-7 seconds

/*both the consumer and producer are not able to access the buffer
* at the same time. One will have to lcoked and the other is unlocked
*/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t stopproducer,stopconsumer;
int idx = 0;

struct Data{
   int number;
   int wait_time;
 };

struct Data buffer[32];

unsigned int randomval(){
	unsigned int value;
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;

	char vendor[13];

	eax = 0x01;

	__asm__ __volatile__(
	                     "cpuid;"
	                     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
	                     : "a"(eax)
	                     );

	if(ecx & 0x40000000){
		__asm__("rdrand  %[value]"
			   : [value] "=r" (value)
			   :
			   : "cc"
			   );
	}
	else{
		//use mt19937
		unsigned long seed = (unsigned long)time(NULL);
		init_genrand(seed);
		unsigned long rand = genrand_int32();
		value = rand & UINT_MAX;
	}
	return value;
}

unsigned int randomminmax(unsigned int min, unsigned int max){
	unsigned int value;
	if(min > max){
		printf("IMPROPER RAND ARGS");
		value = 0;
		return value;
	}
	unsigned int range = (max - min)+1;
	value = randomval();
	value = value%range;
  value = value+min;
	return value;
}

int buffer_empty(int idx){
	int out = 0;
	if (buffer[idx].number == -1 || buffer[idx].number == -1){
		out = 1;
	}
	return out;
}

void *producer(){
	unsigned int producer_wait = 0;
	while(1){
		if(idx >= 32){
			pthread_cond_wait(&stopproducer,&lock);
		}else{
      producer_wait = 0;
      pthread_mutex_lock(&lock);
        if(buffer_empty(idx)){
          buffer[idx].number = randomval();
          buffer[idx].wait_time = randomminmax(2,9);
          printf("Buffer Index: %u \n Value: %u \n Wait Time: %u \n", idx, buffer[idx].number, buffer[idx].wait_time);
          producer_wait = randomminmax(3,7);
          printf("Producer waits for: %u \n", producer_wait);
          pthread_cond_signal(&stopconsumer);
        pthread_mutex_unlock(&lock);
        sleep(producer_wait);
      }
      idx++;
    }
	}
}

void *consumer(){
	unsigned int consumer_wait;

	while(1){
    int grabbed = idx;
		consumer_wait = 0;
		pthread_mutex_lock(&lock);
		if(idx < 1){
			pthread_cond_wait(&stopconsumer,&lock);
		}
		pthread_mutex_unlock(&lock);
      pthread_mutex_lock(&lock);
        if(!(buffer_empty(grabbed))){
          printf("Consumes value %u at index %d \n Consumer waits for %u at index %d \n",buffer[grabbed].number, grabbed, buffer[grabbed].wait_time, grabbed);

          consumer_wait = buffer[grabbed].wait_time;
          buffer[grabbed].number = -1;
          buffer[grabbed].wait_time = -1;
          idx--;
          pthread_cond_signal(&stopproducer);
        }
        pthread_mutex_unlock(&lock);
        sleep(consumer_wait);

    }
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Incorrect number of arguments");
		return 1;
	}
	int i,j;
	int num_threads = atoi(argv[1]);
	for(i = 0; i <32; i++){
		buffer[i].number = -1;
		buffer[i].wait_time = -1;
	}
	printf("idx %d \n", idx);
	pthread_t threads[atoi(argv[1])];
	for(j = 0; j < num_threads; j++){
		if((j%5) == 0){
			pthread_create(&(threads[j]), NULL, producer, NULL);
		}else{
			pthread_create(&(threads[j]), NULL, consumer, NULL);
		}
	}
  while(1){

  }
	return 0;
}
