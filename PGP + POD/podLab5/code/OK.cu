// #include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
// using namespace std;

#define CSC(call)  													\
do {																\
	cudaError_t res = call;											\
	if (res != cudaSuccess) {										\
		fprintf(stderr, "ERROR in %s:%d. Message: %s\n",			\
				__FILE__, __LINE__, cudaGetErrorString(res));		\
		exit(0);													\
	}																\
} while(0)

#define ASCENDING 1
#define DESCENDING 0

__device__ void compASC(int* left, int* right){
    if( *left > *right ){
        int tmp = *left;
        *left = *right;
        *right = tmp;
    }
}

__device__ void compDESC(int* left, int* right){
    if( *left < *right ){
        int tmp = *left;
        *left = *right;
        *right = tmp;
    }
}

#define ThreadPerBlock 512
// !!!Atention Victor!!!
// SharedSize should be equal to 2 * ThreadPerBlock
#define SharedSize 1024

#define _idxShared(id) ((id) + ((id) / 31))
#define _sizeShared(id) ((id) + ((id) / 31) + 100)
// 0   1   2   3   4   ... 29  30  X
// 31  32  33  34  35  ... 60  61  X
// 62  63  64  65  66  ... 91  92  X
// 93  94  95  96  97  ... 122 123 X
// 124 125 126 127 128 ...


__global__ void BitonicSortShared(int* nums, int n, int iterShift, int iterToStart){

    __shared__ int shared[_sizeShared(SharedSize)];

    int idx = blockIdx.x * SharedSize;
    int offset = SharedSize * gridDim.x;
    int prev = idx - (idx & (1 << iterShift));
    int j;
    int iter, localStep;
    int comp = ASCENDING;

    for(int i = idx; i < n; i += offset){
        if ((i >> iterShift) != (prev >> iterShift)){
            comp ^= 1;
        }
        prev = i;

        shared[_idxShared(threadIdx.x)] = nums[i + threadIdx.x];
        shared[_idxShared(ThreadPerBlock + threadIdx.x)] = nums[i + ThreadPerBlock + threadIdx.x];
        __syncthreads();

        for (iter = 2 * iterToStart; iter > 1; iter /= 2){
            for (localStep = iter / 2; localStep > 0; localStep /= 2){

                j = (2 * threadIdx.x) - (threadIdx.x & (localStep - 1));
                if( comp == ASCENDING ){
                    compASC(shared + _idxShared(j), shared + _idxShared(localStep + j));
                }
                else{
                    compDESC(shared + _idxShared(j), shared + _idxShared(localStep + j));
                }
                __syncthreads();

            }
        }

        nums[i + threadIdx.x] = shared[_idxShared(threadIdx.x)];
        nums[i + ThreadPerBlock + threadIdx.x] = shared[_idxShared(ThreadPerBlock + threadIdx.x)];
    }
}

__global__ void BitonicSort(int* nums, int n, int iterShift, int localStep, int localShift){

    int idx = threadIdx.x + blockDim.x * blockIdx.x;
    int offset = blockDim.x * gridDim.x;

    int prev;
    int comp = ASCENDING;
    int i = 2 * idx - (idx & (localStep - 1));
    int kernelShift = (localStep > offset) ? localStep : offset;

    if( (i >> iterShift) % 2 == 1){
        comp = DESCENDING;
    }

    while(i < n){
        if( comp == ASCENDING ){
            compASC(nums + i, nums + i + localStep);
        }
        else{
            compDESC(nums + i, nums + i + localStep);
        }

        prev = i;
        i += offset;

        if( (i >> localShift) != (prev >> localShift) ){
            i += kernelShift;
            if( (i >> iterShift) != (prev >> iterShift) ){
                comp ^= 1;
            }
        }
    }
}

int main(){

    int i, n;
    int powerBound = 1;
    fread(&n, sizeof(int), 1, stdin);

    while(powerBound < n){
        powerBound *= 2;
    }

    int* nums = (int*) malloc(powerBound * sizeof(int));
    fread(nums, sizeof(int), n, stdin);
    for(i = n; i < powerBound; ++i){
        nums[i] = INT_MAX;
    }

    // cout << "\n====================\n";
    // for(int i = 0; i < powerBound; ++i){
    //     cout << nums[i] << " ";
    // }
    // cout << "\n====================\n";

    int* device_nums;
    CSC(cudaMalloc( (void **) &device_nums, powerBound * sizeof(int)));
    CSC(cudaMemcpy(device_nums, nums, powerBound * sizeof(int), cudaMemcpyHostToDevice));

    int iterShift = 1;
    int iteration, localShift, localStep;

    for(iteration = 2; iteration <= powerBound; iteration *= 2){
        localShift = iterShift - 1;
        for(localStep = iteration / 2; localStep > 0; localStep /= 2){

            if (localStep == SharedSize / 2){
                BitonicSortShared<<<16, ThreadPerBlock>>>(device_nums, powerBound, iterShift, localStep);
                CSC(cudaGetLastError());
                break;
            }
            else{
                BitonicSort<<<64, 256>>>(device_nums, powerBound, iterShift, localStep, localShift);
                CSC(cudaGetLastError());
            }
            
            localShift -= 1;
        }

        iterShift += 1;
    }

    CSC(cudaMemcpy(nums, device_nums, powerBound * sizeof(int), cudaMemcpyDeviceToHost));
    CSC(cudaFree(device_nums));

    // cout << "\n====================\n";
    // for(int i = 0; i < powerBound; ++i){
    //     cout << nums[i] << " ";
    // }
    // cout << "\n====================\n";

    fwrite(nums, sizeof(int), n, stdout);
    free(nums);

    return 0;
}