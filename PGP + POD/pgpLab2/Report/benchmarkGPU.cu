
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
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

texture<uchar4, 2, cudaReadModeElementType> tex;

__global__ void kernel(uchar4* dst, int w, int h, int wNew, int hNew)
{
    int idx = blockDim.x * blockIdx.x + threadIdx.x;
    int idy = blockDim.y * blockIdx.y + threadIdx.y;
    int offsetx = blockDim.x * gridDim.x;
    int offsety = blockDim.y * gridDim.y;

    for (int y = idy; y < hNew; y += offsety) {
        for (int x = idx; x < wNew; x += offsetx) {
            int i = (float)(x + 0.5) * w / wNew - 0.5;
            int j = (float)(y + 0.5) * h / hNew - 0.5;
            float xx = (float)(x + 0.5) * w / wNew - 0.5 - i;
            float yy = (float)(y + 0.5) * h / hNew - 0.5 - j;

            if (xx < (float)0.0) {
                i -= 1;
                xx += (float)1.0;
            }

            if (yy < (float)0.0) {
                j -= 1;
                yy += (float)1.0;
            }
            uchar4 pIJ = tex2D(tex, i, j);
            uchar4 pI1J = tex2D(tex, i + 1, j);
            uchar4 pIJ1 = tex2D(tex, i, j + 1);
            uchar4 pI1J1 = tex2D(tex, i + 1, j + 1);
            //uchar4 res;
            float r = pIJ.x * (1.0f - xx) * (1.0f - yy) + pI1J.x * xx * (1.0f - yy) + pIJ1.x * (1.0f - xx) * yy + pI1J1.x * xx * yy;
            float g = pIJ.y * (1.0f - xx) * (1.0f - yy) + pI1J.y * xx * (1.0f - yy) + pIJ1.y * (1.0f - xx) * yy + pI1J1.y * xx * yy;
            float b = pIJ.z * (1.0f - xx) * (1.0f - yy) + pI1J.z * xx * (1.0f - yy) + pIJ1.z * (1.0f - xx) * yy + pI1J1.z * xx * yy;
            float w = pIJ.w;
            dst[y * wNew + x] = make_uchar4(r, g, b, w);
        }
    }
    return;
}


int main()
{

    int w, h;
    int wNew, hNew;
    cin >> w >> h >> wNew >> hNew;

    uchar4* data = nullptr;
    int size = w * h;
    if (size < wNew * hNew) {
        size = wNew * hNew;
    }
    data = new uchar4[size];

    cudaArray* arr;
    cudaChannelFormatDesc ch = cudaCreateChannelDesc<uchar4>();
    CSC(cudaMallocArray(&arr, &ch, w, h));
    CSC(cudaMemcpyToArray(arr, 0, 0, data, sizeof(uchar4) * h * w, cudaMemcpyHostToDevice));


    tex.addressMode[0] = cudaAddressModeClamp;
    tex.addressMode[1] = cudaAddressModeClamp;
    tex.channelDesc = ch;
    tex.filterMode = cudaFilterModePoint;
    tex.normalized = false;


    CSC(cudaBindTextureToArray(tex, arr, ch));

    uchar4* new_image;
    CSC(cudaMalloc(&new_image, sizeof(uchar4) * hNew * wNew));


    cudaEvent_t start, stop;
    cudaErrorCheck(cudaEventCreate(&start));
    cudaErrorCheck(cudaEventCreate(&stop));
    cudaErrorCheck(cudaEventRecord(start, 0));

    kernel <<<dim3(16, 16), dim3(16, 16) >>> (new_image, w, h, wNew, hNew);
    CSC(cudaGetLastError());

    cudaErrorCheck(cudaEventRecord(stop, 0));
    cudaErrorCheck(cudaEventSynchronize(stop));

    double time = -1.0;
    cudaErrorCheck(cudaEventElapsedTime(&time, start, stop));
    cudaErrorCheck(cudaEventDestroy(start));
    cudaErrorCheck(cudaEventDestroy(stop));
    cout << time << endl;



    CSC(cudaMemcpy(data, new_image, sizeof(uchar4) * hNew * wNew, cudaMemcpyDeviceToHost));

    CSC(cudaUnbindTexture(tex));
    CSC(cudaFreeArray(arr));
    CSC(cudaFree(new_image));

    delete[] data;
    return 0;
}
