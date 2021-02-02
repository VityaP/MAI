#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define ASCENDING 1
#define DESCENDING 0

void compASC(int* a, int* b){
    if ( *a > *b ){
        int tmp = *a;
        *a = *b;
        *b = tmp;
    }
}

void compDESC(int* a, int* b){
    if ( *a < *b ){
        int tmp = *a;
        *a = *b;
        *b = tmp;
    }
}

void print(int* nums, int n){

    for(int i = 0; i < n; ++i){
        cout << nums[i] << " ";
    }
    cout << "\n-------------------------\n";
}


int main(){

    for(int localStep = 1; localStep <= 16; localStep *= 2 ){
        for(int idx = 0; idx < 32; ++idx){
            int i = 2 * idx - (idx & (localStep - 1));
            cout << localStep << " " << idx << " " << i << "\n";
        }
    }

    return 0;



    int n;
    cin >> n;
    srand(4545);

    int powerBound = 1;
    while(powerBound < n){
        powerBound *= 2;
    }

    int* nums = (int*) malloc(sizeof(int) * powerBound);
    for(int i = 0; i < n; ++i){
        nums[i] = rand() % 100;
    }
    for (int i = n; i < powerBound; ++i){
        nums[i] = INT_MAX;
    }

    // print(nums, powerBound);

    for(int iteration = 1; iteration < powerBound; iteration *= 2){
        for(int step = iteration; step >= 1; step /= 2){
            for(int i = 0; i < powerBound; i += 2 * step){
                int idx = i % (4 * iteration);
                for(int tmp = 0; tmp < step; ++tmp){
                    // cout << i + tmp << " vs " << i + step +tmp;
                    if (idx < 2 * iteration){
                        compASC(nums + i + tmp, nums + i + step +tmp);
                        // cout << " ASC\n";
                    }
                    else{
                        compDESC(nums + i + tmp, nums + i + step +tmp);
                        // cout << " DESC\n";
                    }
                }
            }
            // cout << "----------------\n";
            // print(nums, powerBound);
        }
    }

    print(nums, powerBound);
    

    free(nums);

    return 0;
}