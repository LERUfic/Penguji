#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>


int array[50];
//int threads;


void* factorialProcess(void *arg){
    unsigned int factorialResult=1;
    int x = *((int *)arg);
	for(int i=1; i<=array[x]; ++i){
            factorialResult *= i; 
        }
    printf("%d! = %u \n",array[x],factorialResult);
}



int main(int argumentCount, char** values){
    int i,j;
    int threads = 0;
    void *p = &threads;
	int err, swap;
    pthread_t tid[50];
	for (i=0; i<argumentCount-1; i++){
		array[i]=atoi(values[i+1]);
	}
    for (i=0; i<argumentCount-1; ++i){
    	for (j=0; j<argumentCount-i-1; ++j){
    		if (array[j] < array[j-1]){
    		    swap = array[j-1];
    		    array[j-1]= array[j];
    		    array[j]= swap;
		    }
	    }
	}

	
	while(threads<argumentCount-1){
		err=pthread_create(&(tid[threads]),NULL,&factorialProcess,(void*) p);
		if(err!=0) 
		{
			printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
            //printf("\n create thread success\n");
            printf("\n");
		}
		//pthread_join(tid[threads],NULL);
        threads++;

	}
    while(threads--){
        pthread_join(tid[threads],NULL);
    }
    //pthread_join(tid[0],NULL);
    //pthread_join(tid[1],NULL);
    //exit(0);
	return 0;

}