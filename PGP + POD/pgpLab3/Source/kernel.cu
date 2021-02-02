#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
#include <fstream>
#include <vector>
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


struct Image{
	int w;
	int h;
	uchar4* pixels;

	Image() {
		w = 0;
		h = 0;
		pixels = nullptr;
	}

	uchar4 GetPixel(int x, int y) {
		return pixels[x + y * w];
	}

	void ReadFromFile(string &input){

		int width;
		int height;
		ifstream inputFile(input, std::ios::in | std::ios::binary);

		if (inputFile.is_open()) {
			if (!inputFile.read((char*)&width, sizeof(width))) {
				cerr << "ERROR: can't read from file " << __LINE__ << endl;
				abort();
			}
			if (!inputFile.read((char*)&height, sizeof(height))) {
				cerr << "ERROR: can't read from file " << __LINE__ << endl;
				abort();
			}
			
			w = width;
			h = height;
			//pixels = (uchar4*)malloc(sizeof(uchar4) * width * height);
			pixels = new uchar4[width * height];

			if (!inputFile.read((char*)pixels, width * height * sizeof(uchar4))) {
				cerr << "ERROR: can't read from file " << __LINE__ << endl;
				abort();
			}
			inputFile.close();
		}
		else {
			cerr << "ERROR: can't open file " << __LINE__ << endl;
			abort();
		}

	}

	void WriteToFile(string &output){

		std::ofstream outputFile(output, std::ios::out | std::ios::binary);

		if (outputFile.is_open()) {
			if (!outputFile.write((char*)&w, sizeof(w))) {
				cerr << "ERROR: can't open write " << __LINE__ << endl;
				abort();
			}
			if (!outputFile.write((char*)&h, sizeof(h))) {
				cerr << "ERROR: can't open write " << __LINE__ << endl;
				abort();
			}
			if (!outputFile.write((char*)pixels, w * h * sizeof(uchar4))) {
				cerr << "ERROR: can't open write " << __LINE__ << endl;
				abort();
			}
			outputFile.close();
		}
		else {
			cerr << "ERROR: can't open file " << __LINE__ << endl;
			abort();
		}
	}

	void Delete() {
		delete[] pixels;
		w = 0;
		h = 0;
		pixels = nullptr;
	}
};



__constant__ double g_avgs[32][3];
__constant__ double g_inv_covs[32][3][3];


__device__ double classifierFunc(uchar4* pixel, int classInd){

	double diff[3];
	double res[3];

	diff[0] = (double) pixel->x - g_avgs[classInd][0];
	diff[1] = (double) pixel->y - g_avgs[classInd][1];
	diff[2] = (double) pixel->z - g_avgs[classInd][2];

	res[0] = 0.0;
	res[1] = 0.0;
	res[2] = 0.0;

	for (int j = 0; j < 3; ++j) {
		for (int k = 0; k < 3; ++k) {
			res[j] += diff[k] * g_inv_covs[classInd][k][j];
		}
	}

	double answer = 0.0;

	for (int k = 0; k < 3; ++k) {
		answer += res[k] * diff[k];
	}
	answer *= -1.0;

	return answer;
}


__global__ void kernel(uchar4* pixels, int size, int PGP_NC) {

	int idX = blockIdx.x * blockDim.x + threadIdx.x;
	int offsetX = gridDim.x * blockDim.x;

	for (int i = idX; i < size; i = i + offsetX) {
		uchar4* pixel = &pixels[i];
		double maxVal = classifierFunc(pixel, 0);
		int maxInd = 0;

		for (int c = 1; c < PGP_NC; ++c){

			double tmpVal = classifierFunc(pixel, c);

			if (tmpVal > maxVal){
				maxVal = tmpVal;
				maxInd = c;
			}
		}

		pixel->w = (unsigned char) maxInd;
	}

}


int main(){

	string input;
	string output;
	Image image;

	cin >> input >> output;
	
	int nc;
	cin >> nc;
	vector< vector< pair<int, int> > > groups(nc);
	double avgs[32][3];
	double covs[32][3][3];
	double covs_inv[32][3][3];

	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 3; ++j) {
			avgs[i][j] = 0.0;
		}
	}

	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				covs[i][j][k] = 0.0;
				covs_inv[i][j][k] = 0.0;
			}
		}
	}

	for (int i = 0; i < nc; ++i){
		int np;
		cin >> np;
		groups[i].resize(np);
		for (int j = 0; j < np; ++j){
			cin >> groups[i][j].first >> groups[i][j].second;
		}
	}

	image.ReadFromFile(input);

	for (int i = 0; i < nc; ++i) {
		
		//makeZeros(avgs[i]);
		//makeZeros(covs[i]);

		for (int j = 0; j < groups[i].size(); ++j){
			uchar4 pixel = image.GetPixel(groups[i][j].first, groups[i][j].second);
			avgs[i][0] += pixel.x;
			avgs[i][1] += pixel.y;
			avgs[i][2] += pixel.z;
		}

		avgs[i][0] /= (double) groups[i].size();
		avgs[i][1] /= (double) groups[i].size();
		avgs[i][2] /= (double) groups[i].size();

		for (int j = 0; j < groups[i].size(); ++j){

			uchar4 pixel = image.GetPixel(groups[i][j].first, groups[i][j].second);
			double diff[3];
			double productT[3][3];

			diff[0] = (double) pixel.x - avgs[i][0];
			diff[1] = (double) pixel.y - avgs[i][1];
			diff[2] = (double) pixel.z - avgs[i][2];

			for (int s1 = 0; s1 < 3; ++s1) {
				for (int s2 = 0; s2 < 3; ++s2) {
					productT[s1][s2] = diff[s1] * diff[s2];
				}
			}

			for (int s1 = 0; s1 < 3; ++s1) {
				for (int s2 = 0; s2 < 3; ++s2) {
					covs[i][s1][s2] += productT[s1][s2];
				}
			}

		}

		//Can't divide by 0
		//What if groups[i].size == 1
		//if (groups[i].size > 1) {
		double tmp = (double) groups[i].size() - 1.0;
		for (int s1 = 0; s1 < 3; ++s1) {
			for (int s2 = 0; s2 < 3; ++s2) {
				covs[i][s1][s2] /= tmp;
			}
		}
		//}

		double det = 0.0;
		det += covs[i][0][0] * covs[i][1][1] * covs[i][2][2];
		det += covs[i][0][1] * covs[i][1][2] * covs[i][2][0];
		det += covs[i][1][0] * covs[i][2][1] * covs[i][0][2];
		det -= covs[i][0][2] * covs[i][1][1] * covs[i][2][0];
		det -= covs[i][0][0] * covs[i][1][2] * covs[i][2][1];
		det -= covs[i][0][1] * covs[i][1][0] * covs[i][2][2];
		
		
		covs_inv[i][0][0] = (covs[i][1][1] * covs[i][2][2] - covs[i][2][1] * covs[i][1][2]) / det;
		covs_inv[i][0][1] = (covs[i][0][2] * covs[i][2][1] - covs[i][0][1] * covs[i][2][2]) / det;
		covs_inv[i][0][2] = (covs[i][0][1] * covs[i][1][2] - covs[i][0][2] * covs[i][1][1]) / det;
		covs_inv[i][1][0] = (covs[i][1][2] * covs[i][2][0] - covs[i][1][0] * covs[i][2][2]) / det;
		covs_inv[i][1][1] = (covs[i][0][0] * covs[i][2][2] - covs[i][0][2] * covs[i][2][0]) / det;
		covs_inv[i][1][2] = (covs[i][1][0] * covs[i][0][2] - covs[i][0][0] * covs[i][1][2]) / det;
		covs_inv[i][2][0] = (covs[i][1][0] * covs[i][2][1] - covs[i][2][0] * covs[i][1][1]) / det;
		covs_inv[i][2][1] = (covs[i][2][0] * covs[i][0][1] - covs[i][0][0] * covs[i][2][1]) / det;
		covs_inv[i][2][2] = (covs[i][0][0] * covs[i][1][1] - covs[i][1][0] * covs[i][0][1]) / det;
		
	}

	CSC(cudaMemcpyToSymbol(g_avgs, avgs, sizeof(double) * 32 * 3));
	CSC(cudaMemcpyToSymbol(g_inv_covs, covs_inv, sizeof(double) * 32 * 9));


	uchar4* result;
	CSC(cudaMalloc( (void **) &result, image.w * image.h * sizeof(uchar4)));
	CSC(cudaMemcpy(result, image.pixels, image.w * image.h * sizeof(uchar4), cudaMemcpyHostToDevice));

	int sz = image.w * image.h;
	kernel <<< 256, 256 >>> (result, sz, nc);
	CSC(cudaGetLastError());

	CSC(cudaMemcpy(image.pixels, result, image.w * image.h * sizeof(uchar4), cudaMemcpyDeviceToHost));
	image.WriteToFile(output);

	image.Delete();
	CSC(cudaFree(result));

	return 0;
}
