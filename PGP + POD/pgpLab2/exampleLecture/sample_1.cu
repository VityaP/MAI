#include <stdio.h>
#include <stdlib.h>

__global__ void kernel(float *arr, int n) {
	int i, idx = blockDim.x * blockIdx.x + threadIdx.x;			// Абсолютный номер потока
	int offset = blockDim.x * gridDim.x;						// Общее кол-во потоков
	for(i = idx; i < n; i += offset)	
		arr[i] *= arr[i];
}

int main() {
	int i, n = 100000;
	float *arr = (float *)malloc(sizeof(float) * n);
	for(i = 0; i < n; i++)
		arr[i] = i;

	float *dev_arr;
	cudaMalloc(&dev_arr, sizeof(float) * n);
	cudaMemcpy(dev_arr, arr, sizeof(float) * n, cudaMemcpyHostToDevice);

	kernel<<<256, 256>>>(dev_arr, n);

	cudaMemcpy(arr, dev_arr, sizeof(float) * n, cudaMemcpyDeviceToHost);
	cudaFree(dev_arr);
	for(i = 0; i < 100; i++)
		printf("%f ", arr[i]);
	printf("\n");
	free(arr);
	return 0;
}
