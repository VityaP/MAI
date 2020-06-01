#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_EXPR_L 128 // max length of expression 
#define MAX_DIGS_C 11 // max digits count for int4

int find(char *str, int *num){

	int k = 0;

	char temp[MAX_DIGS_C] = { 0 };

	while(isdigit(str[k]) || str[k] == '-'){

		temp[k] = str[k];

		++k;

	}

	*num = atoi(temp);

	return k;

};

int main (void){

	int p1[2]; // from parent to child

	int p2[2]; // from child to parent

	if(pipe(p1)){

		perror("pipe::p1");

    	exit(-1);

	}

	if(pipe(p2)){

		perror("pipe::p2");

    	exit(-1);

	}

    while(1){

    	pid_t child = fork();

	    if(child == -1){

	    	perror("fork");

	    	exit(-1);

	    }

	    else if(child > 0){

	        //Parent process

	        printf("Parent process: pid %d\n", getpid());

		    char expr[MAX_EXPR_L];

			printf("%s\n", "Enter an expression with / or * operations or exit");

			scanf("%s", expr);

			if(!strcmp(expr,"exit")){

				break;

			}

			printf("Input expression: %s\n", expr);

		    int len =  strlen(expr)+1;

		    if(write(p1[1], &len, sizeof(int)) != sizeof(int)){

		    	perror("pipe::p1::write");

		    	exit(-1);

		    }

		    if(write(p1[1], expr, len) != len){

		    	perror("pipe::p1::write");

		    	exit(-1);

		    }

		    int status;

		    waitpid(child, &status, 0);

		    if (WIFSIGNALED(status)){

		    	perror("child::signalled");

		    	fprintf(stderr, "signal: %d\n", WTERMSIG(status));

		    	exit(-1);

		    }

		    else if(WIFEXITED(status)){

		    	char reason = WEXITSTATUS(status);

		    	if(reason != 0){

			    	perror("child::exited");

			    	fprintf(stderr, "status: %d\n", reason);

				    exit(-1);

				}

		    }

		    int res;

		    if(read(p2[0], &res, sizeof(int)) != sizeof(int)){

		    	perror("pipe::p2::read");

		    	exit(-1);

		    }

		    	printf("Result: %d\n", res);

	    }

	    else{

	    	//Child process

	    	char expr2comp[MAX_EXPR_L];

	    	int len;

	    	if(read(p1[0], &len, sizeof(int)) != sizeof(int)){

				perror("pipe::p1::read::str_length");

	    		exit(-1);

	    	}

			printf("Child process: pid %d\n", getpid());

	    	if(read(p1[0], expr2comp, MAX_EXPR_L) != len){

	    	 	perror("pipe::p1::read::str");

	    	 	exit(-1);

	    	}

	    	close(p1[0]);

	    	int res = 0;

	    	int i = 0;

	    	int operand;

	    	char sign;

	    	i = find(expr2comp, &res);

	    	while(expr2comp[i] != '\0'){

	    		sign = expr2comp[i];

	    		++i;

	    		i += find(expr2comp + i, &operand);



	    		if(sign == '*'){

	    			res *= operand;

	    		}

	    		else{

	    			if(operand == 0){

	    				perror("computation::division_by_zero");

	    				exit(-1);

	    			}

	    			res /= operand;

	    		}

	    	}

	    	if(write(p2[1], &res, sizeof(int)) != sizeof(int)){

	    		perror("pipe::p2::write");

	    		exit(-1);

	    	}

	    	return 0;

	    }

	}

	close(p1[0]);

	close(p1[1]);

	close(p2[0]);

	close(p2[1]);

	return 0;

}
