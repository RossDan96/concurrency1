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

//check to see if the buffer is empty or not. buffer can only hold 32 items

//producer will need to wait a random time between 3-7 seconds

/*both the consumer and producer are not able to access the buffer
* at the same time. One will have to lcoked and the other is unlocked
*/


 struct Data{
   int number;
   int wait_time;
 };

void producer(){

}

void consumer(){

}
unsigned int randomminmax(unsigned int min, unsigned int max){
	unsigned int value;
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
	
	if(min > max){
		printf("IMPROPER RAND ARGS");
		value = 0;
		return value;
	}
	unsigned int range = (max - min)+1;
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
		value = (value%range)+min;
		return value;	
	}
	else{
		//use mt19937
	}
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
		return value;	
	}
	else{
		//use mt19937
	}

}

int main(){



}
