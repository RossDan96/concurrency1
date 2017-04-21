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

pthread_mutex_t lock;

struct Data{
   int number;
   int wait_time;
 };
 
struct Data buffer[32];

int buffer_empty(int idx){
	int out = 0;
	if (buffer[i].number == -1 || buffer[i].number == -1){
		out = 1;
	}
	return out;
}
 
void *producer(int* idx){
	unsigned int producer_wait = 0;
	while(1){
		if(idx >= 32){
			idx = 0;
		}
		producer_wait = 0;
		if(!(pthread_mutex_trylock(&lock))){
			if(buffer_empty(*idx)){
				buffer[*idx].number = randomval();
				buffer[*idx].wait_time = randomminmax(2,9);
				printf("Buffer Index: %u \n Value: %u \n Wait Time: %u \n", *idx, buffer[*idx].number, buffer[*idx].wait_time);
				producer_wait = randomminmax(3,7);
				printf("Producer waits for: %u \n", producer_wait);
			}
			pthread_mutex_unlock(&lock);
			sleep(producer_wait);
		}
		*idx = *idx + 1;	
	}
}

void *consumer(int* idx){
	unsigned int consumer_wait;
	while(1){
		if(*idx >= 32){
			*idx = 0;
		}
		consumer_wait = 0;
		if(!(pthread_mutex_trylock(&lock))){
			if(!(buffer_empty(*idx))){
				printf("Consumes value %u at index %d \n", buffer[*idx].number, *idx);
				printf("Consumer waits for %u at index %d \n", buffer[*idx].wait_time, *idx);
				consumer_wait = buffer[*idx].wait_time;
				buffer[*idx].number = -1;
				buffer[*idx].wait_time = -1;
			}
			pthread_mutex_unlock(&lock);
			sleep(consumer_wait);
		}
		*idx = *idx + 1;
	}
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
	value = (value%range)+min;
	return value;	
}
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

int main(int argc, char **argv){
	if(argc != 2){
		printf("Incorrect number of arguments");
		return 1;
	}
	int idx;
	int num_threads = atoi(argv[1]);
	for(int i = 0; i <32; i++){
		buffer[idx].number = -1;
		buffer[idx].wait_time = -1;
	}
	printf("idx %d \n", idx);
	pthread_t threads[atoi(argv[1])];
	for(int j = 0; j < num_threads; j++){
		if((j%3) == 0){
			pthread_create(&(threads[i]), NULL, *producer(), (int*)&idx);
		}else{
			pthread_create(&(threads[i]), NULL, *consumer(), (int*)&idx);
		}
	}
	return 0;
}
