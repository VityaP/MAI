// #include "cuda_runtime.h"
// #include "device_launch_parameters.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>
#include "mpi.h"
using namespace std;

#define CSC(call)  													\
do {																\
	cudaError_t res = call;											\
	if (res != cudaSuccess) {										\
		fprintf(stderr, "ERROR in %s:%d. Message: %s\n",			\
				__FILE__, __LINE__, cudaGetErrorString(res));		\
		exit(0);													\
	}																\
} while(0)

// Индексация внутри блока
#define _i(i, j, k) ( ((k) + 1) * (blockY + 2) * (blockX + 2) + ((j) + 1) * (blockX + 2) + (i) + 1 )
#define _ix(id) ( ( (id) % (blockX + 2) ) - 1)
#define _iy(id) ( ( ( (id) % ((blockY + 2) * (blockX + 2)) ) / (blockX + 2) ) - 1)
#define _iz(id) ( ( (id) / ((blockY + 2) * (blockX + 2)) ) - 1)

// Индексация по блокам (процессам)
#define _ib(i, j, k) ( (k) * (gridX * gridY) + (j) * gridX + (i) )
#define _ibx(id) (( (id) % (gridX * gridY) ) % gridX)
#define _iby(id) (( (id) % (gridX * gridY) ) / gridX)
#define _ibz(id) (  (id) / (gridX * gridY) )


int main(int argc, char *argv[]) {

	double downU, upU, leftU, rightU, frontU, backU;
	int gridX, gridY, gridZ;
    int blockX, blockY, blockZ;
    
	int stringLen;
	string outputFile;
    double epsilon;

	double lx, ly, lz;
    double startU;

	int id, ib, jb, kb;
	int i, j, k, step;
	int numproc;
	double *data, *temp, *next, *infoBuffer;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	
	if (id == 0) {

		cin >> gridX >> gridY >> gridZ;
		cin >> blockX >> blockY >> blockZ;
		cin >> outputFile;
		cin >> epsilon;
		cin >> lx >> ly >> lz;
		cin >> downU >> upU >> leftU >> rightU >> frontU >> backU;
		cin >> startU;
		stringLen = outputFile.length();
	}

	MPI_Bcast(&blockX, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&blockY, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&blockZ, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&gridX, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&gridY, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&gridZ, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&lx, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&ly, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&lz, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&epsilon, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&downU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&upU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&leftU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&rightU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&frontU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&backU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&startU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&stringLen, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if( id != 0){
		outputFile.resize(stringLen);
	}

	char x;
	for(i = 0; i < stringLen; ++i){
		if (id == 0) {
			x = outputFile[i];
		}
		MPI_Bcast(&x, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
		outputFile[i] = x;
	}
	
	ib = _ibx(id);
	jb = _iby(id);
	kb = _ibz(id);
	
	double hx = (double) (gridX * blockX) / lx;	
	double hy = (double) (gridY * blockY) / ly;
	double hz = (double) (gridZ * blockZ) / lz;

	data = (double *)malloc(sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2));	
	next = (double *)malloc(sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2));
	
	int sizeOfBuff = blockX * blockY;
	sizeOfBuff = max(sizeOfBuff, blockX * blockZ);
	sizeOfBuff = max(sizeOfBuff, blockY * blockZ);
	infoBuffer = (double *)malloc(sizeof(double) * sizeOfBuff);

	int buffer_size = 12 * sizeOfBuff * sizeof(double) + 12 * MPI_BSEND_OVERHEAD;	// Выделение буфера для буферезированной отправки сообщений
	double *buffer = (double *)malloc(buffer_size);
	MPI_Buffer_attach(buffer, buffer_size);

	for(i = -1; i <= blockX; ++i){					// Инициализация блока
		for(j = -1; j <= blockY; ++j){
			for(k = -1; k <= blockZ; ++k){
				data[_i(i, j, k)] = startU;
			}
		}
	}

	bool work = true;
	double localMax = 0.0;
	double* globalMaxes = (double *) malloc(sizeof(double) * numproc);
	int tmpSize1 = blockY * blockZ;
	int tmpSize2 = blockX * blockZ;
	int tmpSize3 = blockX * blockY;

	double localConst1 = (hx * hx);
	double localConst2 = (hy * hy);
	double localConst3 = (hz * hz);

	double divisor = ( ( hx * hx ) + ( hy * hy ) + ( hz * hz ));

	while(work == true) {

		localMax = 0.0;

		if (ib + 1 < gridX) {					
			for(j = 0; j < blockY; ++j){
				for(k = 0; k < blockZ; ++k){
					infoBuffer[j * blockZ + k] = data[_i(blockX - 1, j, k)];
				}
			}
			
			MPI_Bsend(infoBuffer, tmpSize1, MPI_DOUBLE, _ib(ib + 1, jb, kb), id, MPI_COMM_WORLD);
		}

		if (jb + 1 < gridY) {
			for(i = 0; i < blockX; ++i){
				for(k = 0; k < blockZ; ++k){
					infoBuffer[i * blockZ + k] = data[_i(i, blockY - 1, k)];
				}
			}
			
			MPI_Bsend(infoBuffer, tmpSize2, MPI_DOUBLE, _ib(ib, jb + 1, kb), id, MPI_COMM_WORLD);
		}

		if (kb + 1 < gridZ) {
			for(i = 0; i < blockX; ++i){
				for(j = 0; j < blockY; ++j){
					infoBuffer[i * blockY + j] = data[_i(i, j, blockZ - 1)];
				}
			}
			
			MPI_Bsend(infoBuffer, tmpSize3, MPI_DOUBLE, _ib(ib, jb, kb + 1), id, MPI_COMM_WORLD);
		}
	
		if (ib > 0) {
			for(j = 0; j < blockY; ++j){
				for(k = 0; k < blockZ; ++k){
					infoBuffer[j * blockZ + k] = data[_i(0, j, k)];
				}
			}
			
			MPI_Bsend(infoBuffer, tmpSize1, MPI_DOUBLE, _ib(ib - 1, jb, kb), id, MPI_COMM_WORLD);
		}
		
		if (jb > 0) {
			for(i = 0; i < blockX; ++i){
				for(k = 0; k < blockZ; ++k){
					infoBuffer[i * blockZ + k] = data[_i(i, 0, k)];
				}
			}
			
			MPI_Bsend(infoBuffer, tmpSize2, MPI_DOUBLE, _ib(ib, jb - 1, kb), id, MPI_COMM_WORLD);
		}

		if (kb > 0) {
			for(i = 0; i < blockX; ++i){
				for(j = 0; j < blockY; ++j){
					infoBuffer[i * blockY + j] = data[_i(i, j, 0)];
				}
			}
			
			MPI_Bsend(infoBuffer, tmpSize3, MPI_DOUBLE, _ib(ib, jb, kb - 1), id, MPI_COMM_WORLD);
		}

		// Прием данных

		if (ib > 0) {
			MPI_Recv(infoBuffer, tmpSize1, MPI_DOUBLE, _ib(ib - 1, jb, kb), _ib(ib - 1, jb, kb), MPI_COMM_WORLD, &status);
			for(j = 0; j < blockY; ++j){
				for(k = 0; k < blockZ; ++k){
					data[_i(-1, j, k)] = infoBuffer[j * blockZ + k];
				}
			}
		}
		else {
			for(j = 0; j < blockY; ++j){
				for(k = 0; k < blockZ; ++k){
					data[_i(-1, j, k)] = leftU;
				}
			}
		}

		if (jb > 0) {
			MPI_Recv(infoBuffer, tmpSize2, MPI_DOUBLE, _ib(ib, jb - 1, kb), _ib(ib, jb - 1, kb), MPI_COMM_WORLD, &status);
			for(i = 0; i < blockX; ++i){
				for(k = 0; k < blockZ; ++k){
					data[_i(i, -1, k)] = infoBuffer[i * blockZ + k];
				}
			}
		}
		else {
			for(i = 0; i < blockX; ++i){
				for(k = 0; k < blockZ; ++k){
					data[_i(i, -1, k)] = frontU;
				}
			}
		}

		if (kb > 0) {
			MPI_Recv(infoBuffer, tmpSize3, MPI_DOUBLE, _ib(ib, jb, kb - 1), _ib(ib, jb, kb - 1), MPI_COMM_WORLD, &status);
			for(i = 0; i < blockX; ++i){
				for(j = 0; j < blockY; ++j){
					data[_i(i, j, -1)] = infoBuffer[i * blockY + j];
				}
			}
		}
		else {
			for(i = 0; i < blockX; ++i){
				for(j = 0; j < blockY; ++j){
					data[_i(i, j, -1)] = downU;
				}
			}
		}

		if (ib + 1 < gridX) {
			MPI_Recv(infoBuffer, tmpSize1, MPI_DOUBLE, _ib(ib + 1, jb, kb), _ib(ib + 1, jb, kb), MPI_COMM_WORLD, &status);
			for(j = 0; j < blockY; ++j){
				for(k = 0; k < blockZ; ++k){
					data[_i(blockX, j, k)] = infoBuffer[j * blockZ + k];
				}
			}
		}
		else {
			for(j = 0; j < blockY; ++j){
				for(k = 0; k < blockZ; ++k){
					data[_i(blockX, j, k)] = rightU;
				}
			}
		}

		if (jb + 1 < gridY) {
			MPI_Recv(infoBuffer, tmpSize2, MPI_DOUBLE, _ib(ib, jb + 1, kb), _ib(ib, jb + 1, kb), MPI_COMM_WORLD, &status);
			for(i = 0; i < blockX; ++i){
				for(k = 0; k < blockZ; ++k){
					data[_i(i, blockY, k)] = infoBuffer[i * blockZ + k];
				}
			}
		}
		else {
			for(i = 0; i < blockX; ++i){
				for(k = 0; k < blockZ; ++k){
					data[_i(i, blockY, k)] = backU;
				}
			}
		}

		if (kb + 1 < gridZ) {
			MPI_Recv(infoBuffer, tmpSize3, MPI_DOUBLE, _ib(ib, jb, kb + 1), _ib(ib, jb, kb + 1), MPI_COMM_WORLD, &status);
			for(i = 0; i < blockX; ++i){
				for(j = 0; j < blockY; ++j){
					data[_i(i, j, blockZ)] = infoBuffer[i * blockY + j];
				}
			}
		}
		else {
			for(i = 0; i < blockX; ++i){
				for(j = 0; j < blockY; ++j){
					data[_i(i, j, blockZ)] = upU;
				}
			}
		}

		// Перевычисление значений температуры

		for(i = 0; i < blockX; ++i){
			for(j = 0; j < blockY; ++j){
				for(k = 0; k < blockZ; ++k){
					next[_i(i, j, k)] = 0.5 * ( (data[_i(i + 1, j, k)] + data[_i(i - 1, j, k)]) * localConst1 +
												(data[_i(i, j + 1, k)] + data[_i(i, j - 1, k)]) * localConst2 +
												(data[_i(i, j, k + 1)] + data[_i(i, j, k - 1)]) * localConst3
											  ) / divisor;
					if( abs(next[_i(i, j, k)] - data[_i(i, j, k)]) > localMax ){
						localMax = abs(next[_i(i, j, k)] - data[_i(i, j, k)]);
					}
				}
			}
		}
		
		temp = next;
		next = data;
		data = temp;

		globalMaxes[id] = localMax;
		MPI_Allgather(&localMax, 1, MPI_DOUBLE, globalMaxes, 1, MPI_DOUBLE, MPI_COMM_WORLD);

		work = false;
		for(step = 0; step < numproc; ++step ){
			if( globalMaxes[step] >= epsilon ){
				work = true;
			}
		}

	}

	int n_size = 20;
	char * buff = (char *) malloc(sizeof(char) * (blockX) * (blockY) * (blockZ) * n_size);
	memset(buff, ' ', (blockX) * (blockY) * (blockZ) * n_size * sizeof(char));
	
	for(k = 0; k < blockZ; ++k){
		for(j = 0; j < blockY; ++j) {
			for(i = 0; i < blockX; ++i){
				sprintf(buff + ( k * blockX * blockY + j * blockX + i ) * n_size, " %.6e ", data[_i(i, j, k)]);
			}
			if (_ibx(id) + 1 == gridX){
				buff[ (k * blockX * blockY + (j + 1) * blockX) * n_size - 1] = '\n';
			}
		}
	}
	for(i = 0; i < (blockX) * (blockY) * (blockZ) * n_size; ++i){
		if (buff[i] == '\0'){
			buff[i] = ' ';
		}
	}

	MPI_Datatype filetype;
	int array_of_sizes[3] = { gridZ * blockZ, gridY * blockY, gridX * blockX * n_size};
	int array_of_subsizes[3] = { blockZ, blockY, blockX * n_size};
	int array_of_starts[3] = {_ibz(id) * blockZ,_iby(id) * blockY, _ibx(id) * blockX * n_size};
	MPI_Type_create_subarray(3, array_of_sizes, array_of_subsizes, array_of_starts, MPI_ORDER_C , MPI_CHAR, &filetype);
	MPI_Type_commit(&filetype);

	MPI_File fp;
	MPI_File_delete(outputFile.c_str(), MPI_INFO_NULL);
	MPI_File_open(MPI_COMM_WORLD, outputFile.c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);
	MPI_File_set_view(fp, 0, MPI_CHAR, filetype, "native", MPI_INFO_NULL);
	MPI_File_write_all(fp, buff, (blockX) * (blockY) * (blockZ) * n_size, MPI_CHAR, MPI_STATUS_IGNORE);
	MPI_File_close(&fp);

	MPI_Buffer_detach(buffer, &buffer_size);
	MPI_Finalize();	

	free(buff);
	free(infoBuffer);
	free(data);
	free(next);
	free(buffer);
	free(globalMaxes);

	return 0;
}

