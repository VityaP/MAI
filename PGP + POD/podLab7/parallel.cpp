#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include "mpi.h"
using namespace std;

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
    
	string outputFile;
    double epsilon;

	double lx, ly, lz;
    double startU;

	int id, ib, jb, kb;
	int i, j, k;
	int numproc;
	double *data, *temp, *next, *buff;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);


	MPI_Barrier(MPI_COMM_WORLD);
	
	if (id == 0) {					// Инициализация параметров расчета
		//n = 4;						// Размер блока по одному измерению
		//nb = 4;						// Размер сетки блоков (процессов) по одному измерению	
		//lx = ly = 1.0;
		//bc_down = 1.0;
		//bc_up = 2.0;
		//bc_left = 3.0;
		//bc_right = 4.0;

		cin >> gridX >> gridY >> gridZ;
		cin >> blockX >> blockY >> blockZ;
		cin >> outputFile;
		cin >> epsilon;
		cin >> lx >> ly >> lz;
		cin >> downU >> upU >> leftU >> rightU >> frontU >> backU;
		cin >> startU;

	}

	// MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);			// Передача параметров расчета всем процессам
	MPI_Bcast(&blockX, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&blockY, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&blockZ, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// MPI_Bcast(&nb, 1, MPI_INT, 0, MPI_COMM_WORLD);
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
	
	ib = _ibx(id);		// Переход к 3-мерной индексации процессов 
	jb = _iby(id);
	kb = _ibz(id);
	
	double hx = lx / (double) (gridX * blockX);	
	double hy = ly / (double) (gridY * blockY);
	double hz = lz / (double) (gridZ * blockZ);

	data = (double *)malloc(sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2));	
	next = (double *)malloc(sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2));
	
	int sizeOfBuff = blockX * blockY;
	sizeOfBuff = max(sizeOfBuff, blockX * blockZ);
	sizeOfBuff = max(sizeOfBuff, blockY * blockZ);
	buff = (double *)malloc(sizeof(double) * sizeOfBuff);

	int buffer_size = 12 * sizeOfBuff * sizeof(double) + 12 * MPI_BSEND_OVERHEAD;	// Выделение буфера для буферезированной отправки сообщений
	double *buffer = (double *)malloc(buffer_size);
	MPI_Buffer_attach(buffer, buffer_size);

	for(i = -1; i <= blockX; i++){					// Инициализация блока
		for(j = -1; j <= blockY; j++){
			for(k = -1; k <= blockZ; k++){
				data[_i(i, j, k)] = startU;
			}
		}
	}

	bool work = true;
	double localMax = 0.0;
	double* globalMaxes = (double *) malloc(sizeof(double) * numproc);

	while(work == true) {

		localMax = 0.0;

		MPI_Barrier(MPI_COMM_WORLD);

		if (ib + 1 < gridX) {					
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					buff[j * blockZ + k] = data[_i(blockX - 1, j, k)];
				}
			}
			int tmpSize = blockY * blockZ;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib + 1, jb, kb), id, MPI_COMM_WORLD);
			// std::cout << id << "send to " << _ib(ib + 1, jb, kb) << "\n";
		}

		if (jb + 1 < gridY) {
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					buff[i * blockZ + k] = data[_i(i, blockY - 1, k)];
				}
			}
			int tmpSize = blockX * blockZ;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb + 1, kb), id, MPI_COMM_WORLD);
			// std::cout << id << "send to " << _ib(ib, jb + 1, kb) << "\n";
		}

		if (kb + 1 < gridZ) {
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					buff[i * blockY + j] = data[_i(i, j, blockZ - 1)];
				}
			}
			int tmpSize = blockX * blockY;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb + 1), id, MPI_COMM_WORLD);
			// std::cout << id << "send to " << _ib(ib, jb, kb + 1) << "\n";
		}
	
		if (ib > 0) {
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					buff[j * blockZ + k] = data[_i(0, j, k)];
				}
			}
			int tmpSize = blockY * blockZ;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib - 1, jb, kb), id, MPI_COMM_WORLD);
			// std::cout << id << "send to " << _ib(ib - 1, jb, kb) << "\n";
		}
		
		if (jb > 0) {
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					buff[i * blockZ + k] = data[_i(i, 0, k)];
				}
			}
			int tmpSize = blockX * blockZ;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb - 1, kb), id, MPI_COMM_WORLD);
			// std::cout << id << "send to " << _ib(ib, jb - 1, kb) << "\n";
		}

		if (kb > 0) {
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					buff[i * blockY + j] = data[_i(i, j, 0)];
				}
			}
			int tmpSize = blockX * blockY;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb - 1), id, MPI_COMM_WORLD);
			// std::cout << id << "send to " << _ib(ib, jb, kb - 1) << "\n";
		}

		// Прием данных

		if (ib > 0) {
			int tmpSize = blockY * blockZ;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib - 1, jb, kb), _ib(ib - 1, jb, kb), MPI_COMM_WORLD, &status);
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					data[_i(-1, j, k)] = buff[j * blockZ + k];
				}
			}
		}
		else {
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					data[_i(-1, j, k)] = leftU;
				}
			}
		}

		if (jb > 0) {
			int tmpSize = blockX * blockZ;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb - 1, kb), _ib(ib, jb - 1, kb), MPI_COMM_WORLD, &status);
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					data[_i(i, -1, k)] = buff[i * blockZ + k];
				}
			}
		}
		else {
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					data[_i(i, -1, k)] = frontU;
				}
			}
		}

		if (kb > 0) {
			int tmpSize = blockX * blockY;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb - 1), _ib(ib, jb, kb - 1), MPI_COMM_WORLD, &status);
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					data[_i(i, j, -1)] = buff[i * blockY + j];
				}
			}
		}
		else {
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					data[_i(i, j, -1)] = downU;
				}
			}
		}

		if (ib + 1 < gridX) {
			int tmpSize = blockY * blockZ;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib + 1, jb, kb), _ib(ib + 1, jb, kb), MPI_COMM_WORLD, &status);
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					data[_i(blockX, j, k)] = buff[j * blockZ + k];
				}
			}
		}
		else {
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					data[_i(blockX, j, k)] = rightU;
				}
			}
		}

		if (jb + 1 < gridY) {
			int tmpSize = blockX * blockZ;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb + 1, kb), _ib(ib, jb + 1, kb), MPI_COMM_WORLD, &status);
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					data[_i(i, blockY, k)] = buff[i * blockZ + k];
				}
			}
		}
		else {
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					data[_i(i, blockY, k)] = backU;
				}
			}
		}

		if (kb + 1 < gridZ) {
			int tmpSize = blockX * blockY;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb + 1), _ib(ib, jb, kb + 1), MPI_COMM_WORLD, &status);
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					data[_i(i, j, blockZ)] = buff[i * blockY + j];
				}
			}
		}
		else {
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					data[_i(i, j, blockZ)] = upU;
				}
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);

		// Перевычисление значений температуры

		for(i = 0; i < blockX; i++){
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					next[_i(i, j, k)] = 0.5 * ( (data[_i(i + 1, j, k)] + data[_i(i - 1, j, k)]) / (hx * hx) +
												(data[_i(i, j + 1, k)] + data[_i(i, j - 1, k)]) / (hy * hy) +
												(data[_i(i, j, k + 1)] + data[_i(i, j, k - 1)]) / (hz * hz)
											  ) / 
											  (1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));
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
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Allgather(&localMax, 1, MPI_DOUBLE, globalMaxes, 1, MPI_DOUBLE, MPI_COMM_WORLD);

		work = false;
		for(int step = 0; step < numproc; ++step ){
			if( globalMaxes[step] >= epsilon ){
				work = true;
			}
		}

		// cout << id << "\n";
		// for(int step = 0; step < numproc; ++step ){
		// 	cout << globalMaxes[step] << " ";
		// }
		// cout << endl;

	}

	MPI_Barrier(MPI_COMM_WORLD);
	// cerr << id << endl;

	if (id != 0) {

		for(k = 0; k < blockZ; k++) {

			for(j = 0; j < blockY; j++){
				for(i = 0; i < blockX; i++){
					buff[i] = data[_i(i, j, k)];
				}
				int tmpSize = blockX;
				MPI_Send(buff, tmpSize, MPI_DOUBLE, 0, k * blockY + j, MPI_COMM_WORLD);
			}

			
		}

	}
	else {

		std::fstream file(outputFile, std::ios::out);
        file << std::scientific << std::setprecision(6);

		for(kb = 0; kb < gridZ; kb++){
			for(k = 0; k < blockZ; k++){
				for(jb = 0; jb < gridY; jb++) {
					for(j = 0; j < blockY; j++){
						for(ib = 0; ib < gridX; ib++){
							if (_ib(ib, jb, kb) == 0){
								
								for(i = 0; i < blockX; i++){
									buff[i] = data[_i(i, j, k)];
								}
								
							}
							else{
								int tmpSize = blockX;
								MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb), k * blockY + j, MPI_COMM_WORLD, &status);
								// MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb), k * blockX * blockY + j * blockX, MPI_COMM_WORLD, &status);
							}

							for(i = 0; i < blockX; i++){
								// printf("%.6e ", buff[i]);
								file << buff[i] << " ";
							}

							if( ib + 1 == gridX ){
								file << "\n";
								// printf("\n");
							}
							else{
								file << " ";
								// printf(" ");
							}
						}
						
					}
				}
				// printf("\n");

			}
		}

	}


	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Buffer_detach(buffer, &buffer_size);
	MPI_Finalize();	

	free(buff);
	free(data);
	free(next);
	free(buffer);
	free(globalMaxes);

	return 0;
}

