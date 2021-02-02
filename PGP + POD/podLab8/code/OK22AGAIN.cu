#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "mpi.h"
#include <string>
#include <thrust/extrema.h>
#include <thrust/device_vector.h>

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


__global__ void kernel_copy_xy(double *plane, double *data, int blockX, int blockY, int blockZ, int k, bool direction, double defVal){
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int idy = blockIdx.y * blockDim.y + threadIdx.y;
    int offsetx = blockDim.x * gridDim.x;
    int offsety = blockDim.y * gridDim.y;
	int i, j;

    if( direction == true ){
		for(j = idy; j < blockY; j += offsety){
			for(i = idx; i < blockX; i += offsetx){
                plane[j * blockX + i] = data[_i(i, j, k)];
            }
		}
	}
	else{
		if( plane != NULL ){
			for(j = idy; j < blockY; j += offsety){
				for(i = idx; i < blockX; i += offsetx){
					data[_i(i, j, k)] = plane[j * blockX + i];
				}
			}
		}
		else{
			for(j = idy; j < blockY; j += offsety){
				for(i = idx; i < blockX; i += offsetx){
					data[_i(i, j, k)] = defVal;
				}
			}
		}
	}
}


__global__ void kernel_copy_yz(double *plane, double *data, int blockX, int blockY, int blockZ, int i, bool direction, double defVal){
    int idy = blockIdx.x * blockDim.x + threadIdx.x;
    int idz = blockIdx.y * blockDim.y + threadIdx.y;
    int offsety = blockDim.x * gridDim.x;
    int offsetz = blockDim.y * gridDim.y;
	int j, k;

    if( direction == true ){
		for(k = idz; k < blockZ; k += offsetz){
			for(j = idy; j < blockY; j += offsety){
                plane[k * blockY + j] = data[_i(i, j, k)];
            }
		}
	}
	else{
		if( plane != NULL ){
			for(k = idz; k < blockZ; k += offsetz){
				for(j = idy; j < blockY; j += offsety){
					data[_i(i, j, k)] = plane[k * blockY + j];
				}
			}
		}
		else{
			for(k = idz; k < blockZ; k += offsetz){
				for(j = idy; j < blockY; j += offsety){
					data[_i(i, j, k)] = defVal;
				}
			}
		}
	}
}


__global__ void kernel_copy_xz(double *plane, double *data, int blockX, int blockY, int blockZ, int j, bool direction, double defVal){
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int idz = blockIdx.y * blockDim.y + threadIdx.y;
    int offsetx = blockDim.x * gridDim.x;
    int offsetz = blockDim.y * gridDim.y;
	int i, k;

    if( direction == true ){
		for(k = idz; k < blockZ; k += offsetz){
			for(i = idx; i < blockX; i += offsetx){
                plane[k * blockX + i] = data[_i(i, j, k)];
            }
		}
	}
	else{
		if( plane != NULL ){
			for(k = idz; k < blockZ; k += offsetz){
				for(i = idx; i < blockX; i += offsetx){
					data[_i(i, j, k)] = plane[k * blockX + i];
				}
			}
		}
		else{
			for(k = idz; k < blockZ; k += offsetz){
				for(i = idx; i < blockX; i += offsetx){
					data[_i(i, j, k)] = defVal;
				}
			}
		}
	}
}


__global__ void kernel1(double* next, double* data, int blockX, int blockY, int blockZ, double hx, double hy, double hz){

    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int idy = blockIdx.y * blockDim.y + threadIdx.y;
    int idz = blockIdx.z * blockDim.z + threadIdx.z;
    int offsetx = blockDim.x * gridDim.x;
    int offsety = blockDim.y * gridDim.y;
    int offsetz = blockDim.z * gridDim.z;
    int i, j, k;
    for(i = idx; i < blockX; i += offsetx ){
        for(j = idy; j < blockY; j += offsety ){
            for(k = idz; k < blockZ; k += offsetz ){
				next[_i(i, j, k)] = 0.5 * ( (data[_i(i + 1, j, k)] + data[_i(i - 1, j, k)]) / (hx * hx) +
											(data[_i(i, j + 1, k)] + data[_i(i, j - 1, k)]) / (hy * hy) +
											(data[_i(i, j, k + 1)] + data[_i(i, j, k - 1)]) / (hz * hz)
										) / 
										(1.0 / (hx * hx) + 1.0 / (hy * hy) + 1.0 / (hz * hz));
            }
        }
    }

}


__global__ void kernel2(double* next, double* data, int blockX, int blockY, int blockZ){

    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int idy = blockIdx.y * blockDim.y + threadIdx.y;
    int idz = blockIdx.z * blockDim.z + threadIdx.z;
    int offsetx = blockDim.x * gridDim.x;
    int offsety = blockDim.y * gridDim.y;
    int offsetz = blockDim.z * gridDim.z;
    int i, j, k;
    for(i = idx - 1; i <= blockX; i += offsetx ){
        for(j = idy - 1; j <= blockY; j += offsety ){
            for(k = idz - 1; k <= blockZ; k += offsetz ){
				if( (i != -1) && (j != -1) && (k != -1) && (i != blockX) && (j != blockY) && (k != blockZ) ){
					data[_i(i, j, k)] = abs(next[_i(i, j, k)] - data[_i(i, j, k)]);
				}
				else{
					data[_i(i, j, k)] = 0.0;
				}
            }
        }
    }

}


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
	double *data, *temp, *next;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	// cudaDeviceProp devProp;
	// CSC(cudaGetDeviceProperties(&devProp, 0));
	int deviceCount;
	CSC(cudaGetDeviceCount(&deviceCount));
	CSC(cudaSetDevice(id % deviceCount));
	
	if (id == 0) {

		cin >> gridX >> gridY >> gridZ;
		cin >> blockX >> blockY >> blockZ;
		cin >> outputFile;
		cin >> epsilon;
		cin >> lx >> ly >> lz;
		cin >> downU >> upU >> leftU >> rightU >> frontU >> backU;
		cin >> startU;
		stringLen = outputFile.length();

		// cerr << gridX << " " << gridY << " " << gridZ << "\n";
		// cerr << blockX << " " << blockY << " " << blockZ << "\n";
		// cerr << outputFile << "\n";
		// cerr << epsilon << "\n";
		// cerr << lx << " " << ly << " " << lz << "\n";
		// cerr << downU << " " << upU << " " << leftU << " ";
		// cerr << rightU << " " << frontU << " " << backU << "\n";
		// cerr << startU << "\n";
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
	
	double hx = lx / (double) (gridX * blockX);	
	double hy = ly / (double) (gridY * blockY);
	double hz = lz / (double) (gridZ * blockZ);
	
	int sizeOfBuff = blockX * blockY;
	sizeOfBuff = max(sizeOfBuff, blockX * blockZ);
	sizeOfBuff = max(sizeOfBuff, blockY * blockZ);
	// infoBuffer = (double *)malloc(sizeof(double) * sizeOfBuff);
	int buffer_size = 12 * sizeOfBuff * sizeof(double) + 12 * MPI_BSEND_OVERHEAD;	// Выделение буфера для буферезированной отправки сообщений
	double *buffer = (double *)malloc(buffer_size);
	MPI_Buffer_attach(buffer, buffer_size);

	data = (double *)malloc(sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2));	
	next = (double *)malloc(sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2));

	for(i = -1; i <= blockX; ++i){					// Инициализация блока
		for(j = -1; j <= blockY; ++j){
			for(k = -1; k <= blockZ; ++k){
				data[_i(i, j, k)] = startU;
			}
		}
	}

	double* dev_data;
	CSC(cudaMalloc( (void **) &dev_data, sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2)));
	CSC(cudaMemcpy(dev_data, data, sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2), cudaMemcpyHostToDevice));
	
	double* dev_next;
	CSC(cudaMalloc( (void **) &dev_next, sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2)));
	
	double* plane_xy_CPU = (double *)malloc(sizeof(double) * blockX * blockY);
	double* plane_xy;
	CSC(cudaMalloc( (void **) &plane_xy, sizeof(double) * blockX * blockY));

	double* plane_yz_CPU = (double *)malloc(sizeof(double) * blockY * blockZ);
	double* plane_yz;
	CSC(cudaMalloc( (void **) &plane_yz, sizeof(double) * blockY * blockZ));
	
	double* plane_xz_CPU = (double *)malloc(sizeof(double) * blockX * blockZ);
	double* plane_xz;
	CSC(cudaMalloc( (void **) &plane_xz, sizeof(double) * blockX * blockZ));

	dim3 blocks(32, 32);
	dim3 threads(32, 32);

	bool work = true;
	double localMax = 0.0;
	double* globalMaxes = (double *) malloc(sizeof(double) * numproc);
	int tmpSize1 = blockY * blockZ;
	int tmpSize2 = blockX * blockZ;
	int tmpSize3 = blockX * blockY;


	while(work == true) {

		// MPI_Barrier(MPI_COMM_WORLD);

		localMax = 0.0;

		if (ib + 1 < gridX) {					
			// for(j = 0; j < blockY; ++j){
			// 	for(k = 0; k < blockZ; ++k){
			// 		infoBuffer[j * blockZ + k] = data[_i(blockX - 1, j, k)];
			// 	}
			// }
			kernel_copy_yz<<<blocks, threads>>>(plane_yz, dev_data, blockX, blockY, blockZ, blockX - 1, true, 1.0);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(plane_yz_CPU, plane_yz, sizeof(double) * blockY * blockZ, cudaMemcpyDeviceToHost));
			MPI_Bsend(plane_yz_CPU, tmpSize1, MPI_DOUBLE, _ib(ib + 1, jb, kb), id, MPI_COMM_WORLD);
		}

		if (jb + 1 < gridY) {
			// for(i = 0; i < blockX; ++i){
			// 	for(k = 0; k < blockZ; ++k){
			// 		infoBuffer[i * blockZ + k] = data[_i(i, blockY - 1, k)];
			// 	}
			// }
			kernel_copy_xz<<<blocks, threads>>>(plane_xz, dev_data, blockX, blockY, blockZ, blockY - 1, true, 1.0);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(plane_xz_CPU, plane_xz, sizeof(double) * blockX * blockZ, cudaMemcpyDeviceToHost));
			MPI_Bsend(plane_xz_CPU, tmpSize2, MPI_DOUBLE, _ib(ib, jb + 1, kb), id, MPI_COMM_WORLD);
		}

		if (kb + 1 < gridZ) {
			// for(i = 0; i < blockX; ++i){
			// 	for(j = 0; j < blockY; ++j){
			// 		infoBuffer[i * blockY + j] = data[_i(i, j, blockZ - 1)];
			// 	}
			// }
			kernel_copy_xy<<<blocks, threads>>>(plane_xy, dev_data, blockX, blockY, blockZ, blockZ - 1, true, 1.0);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(plane_xy_CPU, plane_xy, sizeof(double) * blockX * blockY, cudaMemcpyDeviceToHost));
			MPI_Bsend(plane_xy_CPU, tmpSize3, MPI_DOUBLE, _ib(ib, jb, kb + 1), id, MPI_COMM_WORLD);
		}
	
		if (ib > 0) {
			// for(j = 0; j < blockY; ++j){
			// 	for(k = 0; k < blockZ; ++k){
			// 		infoBuffer[j * blockZ + k] = data[_i(0, j, k)];
			// 	}
			// }
			kernel_copy_yz<<<blocks, threads>>>(plane_yz, dev_data, blockX, blockY, blockZ, 0, true, 1.0);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(plane_yz_CPU, plane_yz, sizeof(double) * blockY * blockZ, cudaMemcpyDeviceToHost));
			MPI_Bsend(plane_yz_CPU, tmpSize1, MPI_DOUBLE, _ib(ib - 1, jb, kb), id, MPI_COMM_WORLD);
		}
		
		if (jb > 0) {
			// for(i = 0; i < blockX; ++i){
			// 	for(k = 0; k < blockZ; ++k){
			// 		infoBuffer[i * blockZ + k] = data[_i(i, 0, k)];
			// 	}
			// }
			kernel_copy_xz<<<blocks, threads>>>(plane_xz, dev_data, blockX, blockY, blockZ, 0, true, 1.0);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(plane_xz_CPU, plane_xz, sizeof(double) * blockX * blockZ, cudaMemcpyDeviceToHost));
			MPI_Bsend(plane_xz_CPU, tmpSize2, MPI_DOUBLE, _ib(ib, jb - 1, kb), id, MPI_COMM_WORLD);
		}

		if (kb > 0) {
			// for(i = 0; i < blockX; ++i){
			// 	for(j = 0; j < blockY; ++j){
			// 		infoBuffer[i * blockY + j] = data[_i(i, j, 0)];
			// 	}
			// }
			kernel_copy_xy<<<blocks, threads>>>(plane_xy, dev_data, blockX, blockY, blockZ, 0, true, 1.0);
			CSC(cudaGetLastError());
			CSC(cudaMemcpy(plane_xy_CPU, plane_xy, sizeof(double) * blockX * blockY, cudaMemcpyDeviceToHost));
			MPI_Bsend(plane_xy_CPU, tmpSize3, MPI_DOUBLE, _ib(ib, jb, kb - 1), id, MPI_COMM_WORLD);
		}

		// Прием данных

		if (ib > 0) {
			MPI_Recv(plane_yz_CPU, tmpSize1, MPI_DOUBLE, _ib(ib - 1, jb, kb), _ib(ib - 1, jb, kb), MPI_COMM_WORLD, &status);
			CSC(cudaMemcpy(plane_yz, plane_yz_CPU, sizeof(double) * tmpSize1, cudaMemcpyHostToDevice));
			kernel_copy_yz<<<blocks, threads>>>(plane_yz, dev_data, blockX, blockY, blockZ, -1, false, 1.0);
			CSC(cudaGetLastError());
			// for(j = 0; j < blockY; ++j){
			// 	for(k = 0; k < blockZ; ++k){
			// 		data[_i(-1, j, k)] = infoBuffer[j * blockZ + k];
			// 	}
			// }
		}
		else {
			// for(j = 0; j < blockY; ++j){
			// 	for(k = 0; k < blockZ; ++k){
			// 		data[_i(-1, j, k)] = leftU;
			// 	}
			// }
			kernel_copy_yz<<<blocks, threads>>>(NULL, dev_data, blockX, blockY, blockZ, -1, false, leftU);
			CSC(cudaGetLastError());
		}

		if (jb > 0) {
			MPI_Recv(plane_xz_CPU, tmpSize2, MPI_DOUBLE, _ib(ib, jb - 1, kb), _ib(ib, jb - 1, kb), MPI_COMM_WORLD, &status);
			cudaMemcpy(plane_xz, plane_xz_CPU, sizeof(double) * tmpSize2, cudaMemcpyHostToDevice);
			kernel_copy_xz<<<blocks, threads>>>(plane_xz, dev_data, blockX, blockY, blockZ, -1, false, 1.0);
			CSC(cudaGetLastError());
			// for(i = 0; i < blockX; ++i){
			// 	for(k = 0; k < blockZ; ++k){
			// 		data[_i(i, -1, k)] = infoBuffer[i * blockZ + k];
			// 	}
			// }
		}
		else {
			// for(i = 0; i < blockX; ++i){
			// 	for(k = 0; k < blockZ; ++k){
			// 		data[_i(i, -1, k)] = frontU;
			// 	}
			// }
			kernel_copy_xz<<<blocks, threads>>>(NULL, dev_data, blockX, blockY, blockZ, -1, false, frontU);
			CSC(cudaGetLastError());
		}

		if (kb > 0) {
			MPI_Recv(plane_xy_CPU, tmpSize3, MPI_DOUBLE, _ib(ib, jb, kb - 1), _ib(ib, jb, kb - 1), MPI_COMM_WORLD, &status);
			cudaMemcpy(plane_xy, plane_xy_CPU, sizeof(double) * tmpSize3, cudaMemcpyHostToDevice);
			kernel_copy_xy<<<blocks, threads>>>(plane_xy, dev_data, blockX, blockY, blockZ, -1, false, 1.0);
			CSC(cudaGetLastError());
			// for(i = 0; i < blockX; ++i){
			// 	for(j = 0; j < blockY; ++j){
			// 		data[_i(i, j, -1)] = infoBuffer[i * blockY + j];
			// 	}
			// }
		}
		else {
			// for(i = 0; i < blockX; ++i){
			// 	for(j = 0; j < blockY; ++j){
			// 		data[_i(i, j, -1)] = downU;
			// 	}
			// }
			kernel_copy_xy<<<blocks, threads>>>(NULL, dev_data, blockX, blockY, blockZ, -1, false, downU);
			CSC(cudaGetLastError());
		}

		if (ib + 1 < gridX) {
			MPI_Recv(plane_yz_CPU, tmpSize1, MPI_DOUBLE, _ib(ib + 1, jb, kb), _ib(ib + 1, jb, kb), MPI_COMM_WORLD, &status);
			cudaMemcpy(plane_yz, plane_yz_CPU, sizeof(double) * tmpSize1, cudaMemcpyHostToDevice);
			kernel_copy_yz<<<blocks, threads>>>(plane_yz, dev_data, blockX, blockY, blockZ, blockX, false, 1.0);
			CSC(cudaGetLastError());
			// for(j = 0; j < blockY; ++j){
			// 	for(k = 0; k < blockZ; ++k){
			// 		data[_i(blockX, j, k)] = infoBuffer[j * blockZ + k];
			// 	}
			// }
		}
		else {
			// for(j = 0; j < blockY; ++j){
			// 	for(k = 0; k < blockZ; ++k){
			// 		data[_i(blockX, j, k)] = rightU;
			// 	}
			// }
			kernel_copy_yz<<<blocks, threads>>>(NULL, dev_data, blockX, blockY, blockZ, blockX, false, rightU);
			CSC(cudaGetLastError());
		}

		if (jb + 1 < gridY) {
			MPI_Recv(plane_xz_CPU, tmpSize2, MPI_DOUBLE, _ib(ib, jb + 1, kb), _ib(ib, jb + 1, kb), MPI_COMM_WORLD, &status);
			cudaMemcpy(plane_xz, plane_xz_CPU, sizeof(double) * tmpSize2, cudaMemcpyHostToDevice);
			kernel_copy_xz<<<blocks, threads>>>(plane_xz, dev_data, blockX, blockY, blockZ, blockY, false, 1.0);
			CSC(cudaGetLastError());
			// for(i = 0; i < blockX; ++i){
			// 	for(k = 0; k < blockZ; ++k){
			// 		data[_i(i, blockY, k)] = infoBuffer[i * blockZ + k];
			// 	}
			// }
		}
		else {
			// for(i = 0; i < blockX; ++i){
			// 	for(k = 0; k < blockZ; ++k){
			// 		data[_i(i, blockY, k)] = backU;
			// 	}
			// }
			kernel_copy_xz<<<blocks, threads>>>(NULL, dev_data, blockX, blockY, blockZ, blockY, false, backU);
			CSC(cudaGetLastError());
		}

		if (kb + 1 < gridZ) {
			MPI_Recv(plane_xy_CPU, tmpSize3, MPI_DOUBLE, _ib(ib, jb, kb + 1), _ib(ib, jb, kb + 1), MPI_COMM_WORLD, &status);
			cudaMemcpy(plane_xy, plane_xy_CPU, sizeof(double) * tmpSize3, cudaMemcpyHostToDevice);
			kernel_copy_xy<<<blocks, threads>>>(plane_xy, dev_data, blockX, blockY, blockZ, blockZ, false, 1.0);
			CSC(cudaGetLastError());
			// for(i = 0; i < blockX; ++i){
			// 	for(j = 0; j < blockY; ++j){
			// 		data[_i(i, j, blockZ)] = infoBuffer[i * blockY + j];
			// 	}
			// }
		}
		else {
			// for(i = 0; i < blockX; ++i){
			// 	for(j = 0; j < blockY; ++j){
			// 		data[_i(i, j, blockZ)] = upU;
			// 	}
			// }
			kernel_copy_xy<<<blocks, threads>>>(NULL, dev_data, blockX, blockY, blockZ, blockZ, false, upU);
			CSC(cudaGetLastError());
		}

		// MPI_Barrier(MPI_COMM_WORLD);

		// Перевычисление значений температуры

		// for(i = 0; i < blockX; ++i){
		// 	for(j = 0; j < blockY; ++j){
		// 		for(k = 0; k < blockZ; ++k){
		// 			next[_i(i, j, k)] = 0.5 * ( (data[_i(i + 1, j, k)] + data[_i(i - 1, j, k)]) * localConst1 +
		// 										(data[_i(i, j + 1, k)] + data[_i(i, j - 1, k)]) * localConst2 +
		// 										(data[_i(i, j, k + 1)] + data[_i(i, j, k - 1)]) * localConst3
		// 									  ) / divisor;
		// 			if( abs(next[_i(i, j, k)] - data[_i(i, j, k)]) > localMax ){
		// 				localMax = abs(next[_i(i, j, k)] - data[_i(i, j, k)]);
		// 			}
		// 		}
		// 	}
		// }

		kernel1<<<dim3(4, 4, 4), dim3(16, 8, 4)>>>(dev_next, dev_data, blockX, blockY, blockZ, hx, hy, hz);
		CSC(cudaGetLastError());
		kernel2<<<dim3(4, 4, 4), dim3(16, 8, 4)>>>(dev_next, dev_data, blockX, blockY, blockZ);
		CSC(cudaGetLastError());

		thrust::device_ptr<double> p_arr = thrust::device_pointer_cast(dev_data);
		thrust::device_ptr<double> resOfThrust = thrust::max_element(p_arr, p_arr + (blockX + 2) * (blockY + 2) * (blockZ + 2) );
		localMax = *resOfThrust;

		temp = dev_data;
		dev_data = dev_next;
		dev_next = temp;

		globalMaxes[id] = localMax;
		// MPI_Barrier(MPI_COMM_WORLD);
		MPI_Allgather(&localMax, 1, MPI_DOUBLE, globalMaxes, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		
		work = false;
		for(step = 0; step < numproc; ++step ){
			if( globalMaxes[step] >= epsilon ){
				work = true;
			}
		}
		// if( work == false ){
		// 	cout << " " << work << " \n";
		// }
	}

	CSC(cudaMemcpy(data, dev_data, sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2), cudaMemcpyDeviceToHost));
	CSC(cudaFree(dev_data));
	CSC(cudaFree(dev_next));
	CSC(cudaFree(plane_xy));
	CSC(cudaFree(plane_yz));
	CSC(cudaFree(plane_xz));

	// MPI_Barrier(MPI_COMM_WORLD);
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


	// MPI_Barrier(MPI_COMM_WORLD);
	MPI_Buffer_detach(buffer, &buffer_size);
	MPI_Finalize();	

	free(buff);
	// free(infoBuffer);
	free(data);
	free(next);
	free(buffer);
	free(globalMaxes);

	return 0;
}