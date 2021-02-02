#include <iostream>
#include <stdlib.h>
#include <ctime>
using namespace std;

typedef unsigned char uchar;
typedef unsigned int uint;

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

struct uchar4{
    uchar x, y, z, w;
};

struct  double4{
    double x, y, z, w;
};

inline static int idx(const int i, const uint n)
{
    if (i < 0){
        return 0;
    }
    else if (i >= n){
        return n - 1;
    }
    return i;
}


void kernel(uchar4* dst, int w, int h, int wNew, int hNew, uchar4* data){

    for (int y = 0; y < hNew; y += 1) {
        for (int x = 0; x < wNew; x += 1) {
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

            i = idx(i, w);
            j = idx(j, h);

            uchar4 pIJ = data[j * w + i];
            uchar4 pI1J = data[j * w + (i + 1)];
            uchar4 pIJ1 = data[(j + 1) * w + i];
            uchar4 pI1J1 = data[(j + 1) * w + (i + 1)];
            
            float r = pIJ.x * (1.0f - xx) * (1.0f - yy) + pI1J.x * xx * (1.0f - yy) + pIJ1.x * (1.0f - xx) * yy + pI1J1.x * xx * yy;
            float g = pIJ.y * (1.0f - xx) * (1.0f - yy) + pI1J.y * xx * (1.0f - yy) + pIJ1.y * (1.0f - xx) * yy + pI1J1.y * xx * yy;
            float b = pIJ.z * (1.0f - xx) * (1.0f - yy) + pI1J.z * xx * (1.0f - yy) + pIJ1.z * (1.0f - xx) * yy + pI1J1.z * xx * yy;
            float w = pIJ.w;
            uchar4 res;
            res.x = r;
            res.y = g;
            res.z = b;
            res.w = w;
            dst[y * wNew + x] = res;
        }
    }
    return;
}

int main(){
    
    uint w, h, wNew, hNew;
    cin >> w >> h >> wNew >> hNew;
    uchar4* src  = (uchar4 *) malloc(sizeof(uchar4) * w * h);
    uchar4* dest = (uchar4 *) malloc(sizeof(uchar4) * wNew * hNew);

    clock_t start = clock();
    kernel(dest, w, h, wNew, hNew, src);
    clock_t stop = clock();
    double time = (double) (stop - start) / CLOCKS_PER_SEC;
    cout << time << endl;

    free(dest);
    free(src);
    return 0;
}
