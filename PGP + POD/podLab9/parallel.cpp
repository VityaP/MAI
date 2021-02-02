#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <mpi.h>
#include <omp.h>
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

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

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


	// #pragma omp parallel
	// {
	// 	int numThreads = omp_get_num_threads();
	// 	int threadNum = omp_get_thread_num();
	// 	for(int index = threadNum; index < blockX * blockY * blockZ ; index += numThreads) {
	// 		data[_i(index % blockX, (index % (blockX * blockY)) / blockX , index / (blockX * blockY) )] = startU;
	// 	}
	// }
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

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockY * blockZ ; index += numThreads) {
			// 		buff[(index % blockY) * blockZ + (index / blockY)] = data[_i(blockX - 1, index % blockY, index / blockY)];
			// 	}
			// }
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					buff[j * blockZ + k] = data[_i(blockX - 1, j, k)];
				}
			}

			int tmpSize = blockY * blockZ;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib + 1, jb, kb), id, MPI_COMM_WORLD);
		}

		if (jb + 1 < gridY) {

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockZ ; index += numThreads) {
			// 		buff[(index % blockX) * blockZ + (index / blockX)] = data[_i(index % blockX, blockY - 1, index / blockX)];
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					buff[i * blockZ + k] = data[_i(i, blockY - 1, k)];
				}
			}

			int tmpSize = blockX * blockZ;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb + 1, kb), id, MPI_COMM_WORLD);
		}

		if (kb + 1 < gridZ) {

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockY ; index += numThreads) {
			// 		buff[(index % blockX) * blockY + (index / blockX)] = data[_i(index % blockX, index / blockX, blockZ - 1)];
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					buff[i * blockY + j] = data[_i(i, j, blockZ - 1)];
				}
			}

			int tmpSize = blockX * blockY;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb + 1), id, MPI_COMM_WORLD);
		}
	
		if (ib > 0) {

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockY * blockZ ; index += numThreads) {
			// 		buff[(index % blockY) * blockZ + (index / blockY)] = data[_i(0, index % blockY, index / blockY)];
			// 	}
			// }
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					buff[j * blockZ + k] = data[_i(0, j, k)];
				}
			}

			int tmpSize = blockY * blockZ;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib - 1, jb, kb), id, MPI_COMM_WORLD);
		}
		
		if (jb > 0) {

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockZ ; index += numThreads) {
			// 		buff[(index % blockX) * blockZ + (index / blockX)] = data[_i(index % blockX, 0, index / blockX)];
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					buff[i * blockZ + k] = data[_i(i, 0, k)];
				}
			}

			int tmpSize = blockX * blockZ;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb - 1, kb), id, MPI_COMM_WORLD);
		}

		if (kb > 0) {

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockY ; index += numThreads) {
			// 		buff[(index % blockX) * blockY + (index / blockX)] = data[_i(index % blockX, index / blockX, 0)];
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					buff[i * blockY + j] = data[_i(i, j, 0)];
				}
			}

			int tmpSize = blockX * blockY;
			MPI_Bsend(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb - 1), id, MPI_COMM_WORLD);
		}

		// Прием данных

		if (ib > 0) {
			int tmpSize = blockY * blockZ;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib - 1, jb, kb), _ib(ib - 1, jb, kb), MPI_COMM_WORLD, &status);

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockY * blockZ ; index += numThreads) {
			// 		data[_i(-1, index % blockY, index / blockY)] = buff[(index % blockY) * blockZ + (index / blockY)];
			// 	}
			// }
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					data[_i(-1, j, k)] = buff[j * blockZ + k];
				}
			}
		}
		else {

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockY * blockZ ; index += numThreads) {
			// 		data[_i(-1, index % blockY, index / blockY)] = leftU;
			// 	}
			// }
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					data[_i(-1, j, k)] = leftU;
				}
			}
		}

		if (jb > 0) {
			int tmpSize = blockX * blockZ;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb - 1, kb), _ib(ib, jb - 1, kb), MPI_COMM_WORLD, &status);

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockZ ; index += numThreads) {
			// 		data[_i(index % blockX, -1, index / blockX)] = buff[(index % blockX) * blockZ + (index / blockX)];
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					data[_i(i, -1, k)] = buff[i * blockZ + k];
				}
			}
		}
		else {

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockZ ; index += numThreads) {
			// 		data[_i(index % blockX, -1, index / blockX)] = frontU;
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					data[_i(i, -1, k)] = frontU;
				}
			}
		}

		if (kb > 0) {
			int tmpSize = blockX * blockY;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb - 1), _ib(ib, jb, kb - 1), MPI_COMM_WORLD, &status);

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockY ; index += numThreads) {
			// 		data[_i(index % blockX, index / blockX, -1)] = buff[(index % blockX) * blockY + (index / blockX)];
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					data[_i(i, j, -1)] = buff[i * blockY + j];
				}
			}
		}
		else {


			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockY ; index += numThreads) {
			// 		data[_i(index % blockX, index / blockX, -1)] = downU;
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					data[_i(i, j, -1)] = downU;
				}
			}
		}

		if (ib + 1 < gridX) {
			int tmpSize = blockY * blockZ;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib + 1, jb, kb), _ib(ib + 1, jb, kb), MPI_COMM_WORLD, &status);

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockY * blockZ ; index += numThreads) {
			// 		data[_i(blockX, index % blockY, index / blockY)] = buff[(index % blockY) * blockZ + (index / blockY)];
			// 	}
			// }
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					data[_i(blockX, j, k)] = buff[j * blockZ + k];
				}
			}
		}
		else {

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockY * blockZ ; index += numThreads) {
			// 		data[_i(blockX, index % blockY, index / blockY)] = rightU;
			// 	}
			// }
			for(j = 0; j < blockY; j++){
				for(k = 0; k < blockZ; k++){
					data[_i(blockX, j, k)] = rightU;
				}
			}
		}

		if (jb + 1 < gridY) {
			int tmpSize = blockX * blockZ;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb + 1, kb), _ib(ib, jb + 1, kb), MPI_COMM_WORLD, &status);
			
			
			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockZ ; index += numThreads) {
			// 		data[_i(index % blockX, blockY, index / blockX)] = buff[(index % blockX) * blockZ + (index / blockX)];
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					data[_i(i, blockY, k)] = buff[i * blockZ + k];
				}
			}
		}
		else {

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockZ ; index += numThreads) {
			// 		data[_i(index % blockX, blockY, index / blockX)] = backU;
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(k = 0; k < blockZ; k++){
					data[_i(i, blockY, k)] = backU;
				}
			}
		}

		if (kb + 1 < gridZ) {
			int tmpSize = blockX * blockY;
			MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb + 1), _ib(ib, jb, kb + 1), MPI_COMM_WORLD, &status);
			
			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockY ; index += numThreads) {
			// 		data[_i(index % blockX, index / blockX, blockZ)] = buff[(index % blockX) * blockY + (index / blockX)];
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					data[_i(i, j, blockZ)] = buff[i * blockY + j];
				}
			}
		}
		else {

			// #pragma omp parallel
			// {
			// 	int numThreads = omp_get_num_threads();
			// 	int threadNum = omp_get_thread_num();
			// 	for(int index = threadNum; index < blockX * blockY ; index += numThreads) {
			// 		data[_i(index % blockX, index / blockX, blockZ)] = upU;
			// 	}
			// }
			for(i = 0; i < blockX; i++){
				for(j = 0; j < blockY; j++){
					data[_i(i, j, blockZ)] = upU;
				}
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);


		#pragma omp parallel reduction(max:localMax)
        {
            int numThreads = omp_get_num_threads();
            int threadNum = omp_get_thread_num();
            for(int index = threadNum; index < blockX * blockY * blockZ; index += numThreads) {
                int i = index % blockX;
                int j = (index % (blockX * blockY)) / blockX;
                int k = index / (blockX * blockY);
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

	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (id != 0) {

		for(k = 0; k < blockZ; k++) {
			for(j = 0; j < blockY; j++){

				// #pragma omp parallel
				// {
				// 	int numThreads = omp_get_num_threads();
				// 	int threadNum = omp_get_thread_num();
				// 	for(int i = threadNum; i < blockX ; i += numThreads) {
				// 		buff[i] = data[_i(i, j, k)];
				// 	}
				// }

				for(int i = 0; i < blockX; i++){
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
								
								//#pragma omp parallel
								//{
								//	int numThreads = omp_get_num_threads();
								//	int threadNum = omp_get_thread_num();
								//	for(int i = threadNum; i < blockX ; i += numThreads) {
								//		buff[i] = data[_i(i, j, k)];
								//	}
								//}

								for(int i = 0; i < blockX; i++){
									buff[i] = data[_i(i, j, k)];
								}
								
							}
							else{
								int tmpSize = blockX;
								MPI_Recv(buff, tmpSize, MPI_DOUBLE, _ib(ib, jb, kb), k * blockY + j, MPI_COMM_WORLD, &status);
							}

							for(i = 0; i < blockX; i++){
								file << buff[i] << " ";
							}

							if( ib + 1 == gridX ){
								file << "\n";
							}
							else{
								file << " ";
							}
						}
					}
				}
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

