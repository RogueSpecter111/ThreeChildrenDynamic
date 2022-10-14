#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	int numOfElements = argc-1;
	int argArray[numOfElements];
	
	int forks[numOfElements];
	int vals[numOfElements];
	int sum;

	for(int i=1; i<argc; i++){
		 argArray[i-1] = atoi(argv[i]);	 
	}

	int pipes[numOfElements][2];
	for (int i=0; i<numOfElements; i++){
		if(pipe(pipes[i]) == -1){
			perror("pipe");
			exit(1);
		}
	}

	int fib(int n)
	{
		if (n <= 1)
			return n;
		return fib(n - 1) + fib(n - 2);
	}


//--------------------------------------------------------------------------------------------------------------


	printf("Parent PID is %d\n", (int)getpid());
    for(int i=0;i<numOfElements;i++) {
    	forks[i]=fork();
     
        if (forks[i] < 0) {
           // fork failed; exit
            fprintf(stderr, "fork failed\n");
    			exit(1);
        } 
    	else if (forks[i] == 0) {	
    		int n = argArray[i];
    		int _fib = fib(n);
    
            	close(pipes[i][0]);
    		write(pipes[i][1], &_fib, sizeof(_fib));
    		printf("Child process %d with PID %d sending %d to parent process\n",i,(int)getpid(),_fib);
		close(pipes[i][1]);
    		exit(0);
        }
        else {
            close(pipes[i][1]);
            read(pipes[i][0], &vals[i], sizeof(sum));
    		close(pipes[i][0]);
        }
    }
    for (int i=0;i<numOfElements;i++) {
		sum += vals[i];
	}
	printf("Final result is: %d\n", sum);	
	return 0;
}

