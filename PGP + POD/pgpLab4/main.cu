#include <iostream>
#include <math.h>
#include <cmath>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

class Comparator{
public:
    __host__ __device__ bool operator()(const double a, const double b) const{
        return fabs(a) < fabs(b);
    }
};

__global__ void swapLines(double * matrix, double* identity, int n, int i, int j) {

    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int offset = gridDim.x * blockDim.x;

    int k;
    double tmp;
    for (k = idx; k < n; k += offset) {
        tmp = matrix[k * n + i];
        matrix[k * n + i] = matrix[k * n + j];
        matrix[k * n + j] = tmp;

        tmp = identity[k * n + i];
        identity[k * n + i] = identity[k * n + j];
        identity[k * n + j] = tmp;
    }
}

__global__ void devideIdentity(double* matrix, double* identity, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int idy = blockIdx.y * blockDim.y + threadIdx.y;
    int offsetx = gridDim.x * blockDim.x;
    int offsety = gridDim.y * blockDim.y;
    int i, j;
    for (i = idx; i < n; i += offsetx) {
        for (j = idy; j < n; j += offsety) {
            identity[j * n + i] /= matrix[i * n + i];
        }
    }
}

__global__ void makeDownNull(double* matrix, double* identity, int n, int x) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int idy = blockIdx.y * blockDim.y + threadIdx.y;
    int offsetx = gridDim.x * blockDim.x;
    int offsety = gridDim.y * blockDim.y;

    int i, j;
    double particion;
    for (i = x + 1 + idx; i < n; i += offsetx) {
        particion = -matrix[x * n + i] / matrix[x * n + x];
        for (j = x + 1 + idy; j < n; j += offsety) {
            // matrix[j * n + i] = fma(-matrix[x * n + i] / matrix[x * n + x], matrix[j * n + x], matrix[j * n + i]);
            matrix[j * n + i] = particion * matrix[j * n + x] + matrix[j * n + i];
        }
        for (j = idy; j < n; j += offsety) {
            // identity[j * n + i] = fma(-matrix[x * n + i] / matrix[x * n + x], identity[j * n + x], identity[j * n + i]);
            identity[j * n + i] = particion * identity[j * n + x] + identity[j * n + i];
        }
    }

}

__global__ void makeUpNull(double* matrix, double* identity, int n, int x) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    int idy = threadIdx.y + blockIdx.y * blockDim.y;
    int offsetx = gridDim.x * blockDim.x;
    int offsety = gridDim.y * blockDim.y;

    int i, j;
    double particion;
    for (i = x - 1 - idx; i >= 0; i -= offsetx) {
        particion = -matrix[x * n + i] / matrix[x * n + x];
        for (j = idy; j < n; j += offsety) {
            // identity[j * n + i] = fma(-matrix[x * n + i] / matrix[x * n + x], identity[j * n + x], identity[j * n + i]);
            identity[j * n + i] = particion * identity[j * n + x] + identity[j * n + i];
        }
    }
}


int main() {

    std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

    int n;
    cin >> n;
    if( n <= 0 ){
        return 0;
    }

    int i, j;
    double *matrix = (double*) malloc( n * n * sizeof(double) );
    double *identity = (double*) malloc( n * n * sizeof(double) );

    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            cin >> matrix[j * n + i];
        }
    }

    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            if( i != j){
                identity[i * n + j] = 0.0;
            }
            else{
                identity[i * n + j] = 1.0;
            }
        }
    }


    
    double* dev_matrix;
    double* dev_identity;
    cudaMalloc(&dev_matrix, sizeof(double) * n * n);
    cudaMalloc(&dev_identity, sizeof(double) * n * n);
    cudaMemcpy(dev_matrix, matrix, sizeof(double) * n * n, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_identity, identity, sizeof(double) * n * n, cudaMemcpyHostToDevice);
    const thrust::device_ptr<double> ptr = thrust::device_pointer_cast(dev_matrix);

    const Comparator comp;
    dim3 block(32, 16);
    dim3 thread(32, 16);

    for (i = 0; i < n - 1; ++i) {
        const int max_idx = thrust::max_element(ptr + i * n + i, ptr + (i + 1) * n, comp) - ptr - i * n;
        if (max_idx != i){
            swapLines<<<256, 256>>>(dev_matrix, dev_identity, n, i, max_idx);
        }
        makeDownNull<<<block, thread>>>(dev_matrix, dev_identity, n, i);
    }


    for (i = n - 1; i > 0; i--) {
        // const int max_idx = thrust::max_element(ptr + i * n + i, ptr + (i + 1) * n, compare) - ptr - i * n;
        // if (max_idx != i){
        //     swapLines<<<256, 256>>>(dev_matrix, dev_identity, n, i, max_idx);
        // }
        makeUpNull<<<block, thread>>>(dev_matrix, dev_identity, n, i);
    }

    devideIdentity<<<block, thread>>>(dev_matrix, dev_identity, n);


    cudaMemcpy(matrix, dev_matrix, sizeof(double) * n * n, cudaMemcpyDeviceToHost);
    cudaMemcpy(identity, dev_identity, sizeof(double) * n * n, cudaMemcpyDeviceToHost);
    cudaFree(dev_matrix);
    cudaFree(dev_identity);

    cout << scientific;
    cout.precision(10);
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            cout << identity[j * n + i] << " ";
        }
        cout << "\n";
    }
    
    delete[] matrix;
    delete[] identity;

    return 0;
}