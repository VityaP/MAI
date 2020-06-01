#include <sys/mman.h> 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>

#define MemoryForMmap 20
#define MAX_EXPR_L 128 // max length of expression 
#define MAX_DIGS_C 15 // max digits count for int4


char* file_name = "lab4test.txt";

int find( char *str ,  int *num ){

	int k = 0;

	char temp[ MAX_DIGS_C ] = { 0 };

	while( isdigit( str[ k ] ) || str[ k ] == '-' ){

		temp[ k ] = str[ k ];

		++k;

	}

	*num = atoi( temp );

	return k;

};

char* parse( char *text ){

    char *error = "error:incorrect input data";

	int res = 0;

	int i = 0;

    int operand;

	char sign;

    char *answer = malloc( MAX_DIGS_C*sizeof( char ) );

    if(  text[ 0 ] == '*' || text[ 0 ] == '/'  ){

        return error;
    }

    for(  i = 0 ; i < strlen( text ) ; i++ ){

        if(  (  text[ i ] > '9' || text[ i ] < '0'  ) && text[ i ] != '*' && text[ i ] != '/' && text[ i ] != '-'  ){

            return error;
        }

    }

	i = find( text ,  &res );

	while( text[ i ] != '\0' ){

		sign = text[ i ];

		++i;

		i += find( text + i ,  &operand );

		if( sign == '*' ){

	    	res *= operand;

	    }

	    else{

	    	if( operand == 0 ){

	    		perror( "computation::division_by_zero" );

	    		exit( EXIT_FAILURE );

	    	}

	    	res /= operand;

	    }

	}

    sprintf( answer , "%d" , res );

    return answer;

};

int main( int argc , char* argv[  ] ){

    int pipefd[ 2 ];

    int fd;

    char action;

    void* ptr;

	pid_t childPid;

    fd = open(  file_name ,  O_RDWR | O_CREAT  );

    if(  fd < 0  ){

        perror( "error open file" );

        exit( EXIT_FAILURE );    

    }

    ptr = mmap (  NULL  ,  MemoryForMmap  ,  PROT_READ | PROT_WRITE  ,  MAP_SHARED  ,  fd  ,  0  );

    if(  ptr == MAP_FAILED  ){

        perror( "error mmap" );

        exit( EXIT_FAILURE );    

    }

    printf( "%s\n" ,  "Enter an expression with / or * operations or exit" );

    while( 1 ){

    if(  pipe( pipefd ) < 0  ){

        perror( "error pipe" );

        exit( EXIT_FAILURE );    

    }

    if(  (  childPid = fork(  )  ) < 0  ){

        perror( "error fork" );

        exit( EXIT_FAILURE );

    }

	if (  childPid == 0  ){

        //Child process

        char text[ 40 ];

        int signal_from_parent;

        close(  pipefd[ 1 ]  );

        if(  read(  pipefd[ 0 ]  ,  &signal_from_parent  ,  sizeof( int )  ) != sizeof( int )  ){

        perror( "error read" );

        exit( EXIT_FAILURE );    

        };

        strcpy(  text  , ptr  );

        strcpy(  ptr  ,  parse( text )  );

        exit( 1 );

	}

	else if (  childPid !=0  ){

        //Parent process

        char buf[ MemoryForMmap ];

        int signal_for_child = 1;

        if(  write( fd , "\0" , 1 ) != 1  ){

        perror( "error write" );

        exit( EXIT_FAILURE );    

        }

        scanf( "%s" , buf );

        close( pipefd[ 0 ] );

        strcpy( ptr , buf );

        if(  !strcmp(  buf  ,  "exit"  )  ){

            if(  write(  pipefd[ 1 ]  ,  &signal_for_child  ,  sizeof( int )  ) != sizeof( int )  ){

            perror( "error write" );

            exit( EXIT_FAILURE );    

            }

            if(  munmap(  ptr  ,  MemoryForMmap  ) != 0  ){

            perror( "error munmap" );

            exit( EXIT_FAILURE );    

            }//free memory given by mmap

            if(  close(  fd  ) < 0  ){

            perror( "error close" );

            exit( EXIT_FAILURE );  
            
            }

            if(  remove(  file_name  ) != 0  ){

            perror( "error remove" );

            exit( EXIT_FAILURE );  
            
            }

            exit( 1 ); 
        }

        if(  write( pipefd[ 1 ] , &signal_for_child , sizeof( int ) ) != sizeof( int )  ){

        perror( "error write" );

        exit( EXIT_FAILURE );    

        }//send signal for child

        if(  wait( NULL ) == -1  ){

        perror( "error wait" );

        exit( EXIT_FAILURE );    

        }

        strcpy( buf , ptr );//cope answer from memory

        if( strcmp( buf , "error" ) ) {

            printf( "result : %s\n" , buf );
        }

        else {

            printf( "%s\n" , buf );
        }

	}

    printf( "%s\n" ,  "Enter an expression with / or * operations or exit" );

    }

	return 0;

}
