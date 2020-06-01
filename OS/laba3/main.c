#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct{

        int rowN;

        int rowcurrent;
	
        int** arrayOFDATA; 
	
        int* resArr;

    } Sizedata;
 
void* threadFunc(void* thread_data){

	    Sizedata *data = (Sizedata*) thread_data;

        printf("\n");

        int a , b , p ;

        a = 0;

        b = data->rowN;

        p = data->rowcurrent;
        
        printf("We are in %d thread \n",p+1);

        printf("\n");

        for(int j = a; j < b ; j++){

            data->resArr[ p ] = data->resArr[ p ] + data->arrayOFDATA[ j ][ p ] ;

            printf("%d\t", data->resArr[ p ]);

        }

        printf("\n");

        printf("\n");
     
	    return NULL;

    }
 
int main(){

    int N , M;
    

    printf("How many arrays ?\n");

    scanf("%d",&N);

    printf("Print their size \n");

    scanf("%d",&M);   

    printf("\n");



	int** Matrix = (int**) malloc(N * sizeof(int*));

	int* resultMatrix = (int*) malloc(M * sizeof(int*));

	for(int i = 0; i < N; i++){

		Matrix[i] = (int*) malloc(M * sizeof(int));

	}







    srand(42);

    int r;

	for(int i = 0; i < N; i++){

		for(int j = 0; j < M; j++){

            r = rand() % 1000;

			Matrix[i][j] = r;

			resultMatrix[i] = 0;

		}

	}





    for(int i = 0; i < N; i++){

		for(int j = 0; j < M; j++){

			printf("Matrix[%d][%d] = %d\n",i,j,Matrix[i][j]);

		}

	}



    printf("\n");

    printf("\n");



    for(int i = 0; i < N; i++){

        printf("\n");

		for(int j = 0; j < M; j++){

			printf("%d\t",Matrix[i][j]);

		}

	}

    printf("\n");

    printf("\n");

	pthread_t* arrayOFTHREADS = (pthread_t*) malloc(M * sizeof(pthread_t));

	Sizedata* DATA = (Sizedata*) malloc(M * sizeof(Sizedata));

	for(int i = 0; i < M; i++){

        DATA[i].rowN = N;

        DATA[i].rowcurrent = i;

		DATA[i].arrayOFDATA = Matrix;

		DATA[i].resArr = resultMatrix;

		pthread_create(&(arrayOFTHREADS[i]), NULL, threadFunc, &DATA[i]);
	}

	for(int i = 0; i < M; i++){

		pthread_join(arrayOFTHREADS[i], NULL);
    }

    for(int i = 0; i < M; i++){

		printf("resultMatrix[%d] = %d\n",i,resultMatrix[i]);

	}

    printf("\n");

    printf("\n");

    for(int i = 0; i < M; i++){

		printf("%d\t",resultMatrix[i]);

	}

    printf("\n");

    printf("\n");

	free(arrayOFTHREADS);

	free(DATA);

	for(int i = 0; i < N; i++){

		free(Matrix[i]);

	}

	free(Matrix);

	free(resultMatrix);

	return 0;

}
