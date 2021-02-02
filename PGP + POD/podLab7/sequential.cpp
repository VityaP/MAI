#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

// Индексация внутри блока
#define _i(i, j, k) ( ((k) + 1) * (blockY + 2) * (blockX + 2) + ((j) + 1) * (blockX + 2) + (i) + 1 )
#define _ix(id) ( ( (id) % (blockX + 2) ) - 1)
#define _iy(id) ( ( ( (id) % ((blockY + 2) * (blockX + 2)) ) / (blockX + 2) ) - 1)
#define _iz(id) ( ( (id) / ((blockY + 2) * (blockX + 2)) ) - 1)

int main(int argc, char *argv[]) {

	clock_t startTime, endTime;
	startTime = clock();

	double downU, upU, leftU, rightU, frontU, backU;
	int gridX, gridY, gridZ;
    int blockX, blockY, blockZ;
    
	string outputFile;
    double epsilon;

	double lx, ly, lz;
    double startU;

	int i, j, k;
	double *data, *temp, *next;

	cin >> gridX >> gridY >> gridZ;
	cin >> blockX >> blockY >> blockZ;
	cin >> outputFile;
	cin >> epsilon;
	cin >> lx >> ly >> lz;
	cin >> downU >> upU >> leftU >> rightU >> frontU >> backU;
	cin >> startU;
	
	double hx = lx / (double) (gridX * blockX);	
	double hy = ly / (double) (gridY * blockY);
	double hz = lz / (double) (gridZ * blockZ);

	blockX = gridX * blockX;
	blockY = gridY * blockY;
	blockZ = gridZ * blockZ;

	data = (double *)malloc(sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2));	
	next = (double *)malloc(sizeof(double) * (blockX + 2) * (blockY + 2) * (blockZ + 2));
	
	for(i = 0; i < blockX; i++){
		for(j = 0; j < blockY; j++){
			for(k = 0; k < blockZ; k++){
				data[_i(i, j, k)] = startU;
			}
		}
	}

	for(j = 0; j < blockY; j++){
		for(k = 0; k < blockZ; k++){
			data[_i(-1, j, k)] = leftU;
		}
	}

	for(i = 0; i < blockX; i++){
		for(k = 0; k < blockZ; k++){
			data[_i(i, -1, k)] = frontU;
		}
	}

	for(i = 0; i < blockX; i++){
		for(j = 0; j < blockY; j++){
			data[_i(i, j, -1)] = downU;
		}
	}

	for(j = 0; j < blockY; j++){
		for(k = 0; k < blockZ; k++){
			data[_i(blockX, j, k)] = rightU;
		}
	}

	for(i = 0; i < blockX; i++){
		for(k = 0; k < blockZ; k++){
			data[_i(i, blockY, k)] = backU;
		}
	}

	for(i = 0; i < blockX; i++){
		for(j = 0; j < blockY; j++){
			data[_i(i, j, blockZ)] = upU;
		}
	}

	bool work = true;
	double localMax = 0.0;

	while(work == true) {

		localMax = 0.0;

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
		
		if( localMax < epsilon ){
			work = false;
		}

	}

	endTime = clock();

	printf("%f seconds \n", ((float ) endTime - startTime)/CLOCKS_PER_SEC);

	free(data);
	free(next);

	return 0;
}

