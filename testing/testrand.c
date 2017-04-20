
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <limits.h>
 #include "mt19937ar.h"
 #include <time.h>

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
    printf ("asm /n");
		__asm__("rdrand  %[value]"
			   : [value] "=r" (value)
			   :
			   : "cc"
			   );
	}
	else{
    unsigned long seed = (unsigned long)time(NULL);
		//use mt19937
    printf ("mt19937 \n");
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
	value = (value%range)+min;
	return value;
}

int main(){
	unsigned int a,b,c;
	a = randomval();
	b = randomminmax(2,9);
	c = randomminmax(3,7);
	printf("a: %u \n b: %u \n c: %u \n",a,b,c);
	return 0;
}
