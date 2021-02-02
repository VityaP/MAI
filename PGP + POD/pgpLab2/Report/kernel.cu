
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
    string input;
    string output;
    int wNew, hNew;
    cin >> input >> output >> wNew >> hNew;

    uchar4* data = nullptr;
    int w, h;

    ifstream inputFile(input, std::ios::in | std::ios::binary);
    if (inputFile.is_open()) {
        if (!inputFile.read((char*)&w, sizeof(w))) {
            cerr << "ERROR: can't read from file " << __LINE__ << endl;
            abort();
        }
        if (!inputFile.read((char*)&h, sizeof(h))) {
            cerr << "ERROR: can't read from file " << __LINE__ << endl;
            abort();
        }
        int size = w * h;
        if (size < wNew * hNew) {
            size = wNew * hNew;
        }
        data = new uchar4[size];
        if (!inputFile.read((char*)data, w * h * sizeof(uchar4))) {
            cerr << "ERROR: can't read from file " << __LINE__ << endl;
            abort();
        }
        inputFile.close();
    }
    else {
        cerr << "ERROR: can't open file " << __LINE__ << endl;
        abort();
    }

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


    kernel <<<dim3(32, 32), dim3(32, 32) >>> (new_image, w, h, wNew, hNew);
    CSC(cudaGetLastError());

    CSC(cudaMemcpy(data, new_image, sizeof(uchar4) * hNew * wNew, cudaMemcpyDeviceToHost));

    std::ofstream outputFile(output, std::ios::out | std::ios::binary);
    if (outputFile.is_open()) {
        if (!outputFile.write((char*)&wNew, sizeof(wNew))) {
            cerr << "ERROR: can't open write " << __LINE__ << endl;
            abort();
        }
        if (!outputFile.write((char*)&hNew, sizeof(hNew))) {
            cerr << "ERROR: can't open write " << __LINE__ << endl;
            abort();
        }
        if (!outputFile.write((char*)data, wNew * hNew * sizeof(uchar4))) {
            cerr << "ERROR: can't open write " << __LINE__ << endl;
            abort();
        }
        outputFile.close();
    }
    else {
        cerr << "ERROR: can't open file " << __LINE__ << endl;
        abort();
    }

    CSC(cudaUnbindTexture(tex));
    CSC(cudaFreeArray(arr));
    CSC(cudaFree(new_image));

    delete[] data;
    return 0;
}
