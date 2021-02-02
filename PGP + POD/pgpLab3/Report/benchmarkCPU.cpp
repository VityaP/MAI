#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include <set>
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

double avgs[32][3];
double covs[32][3][3];
double covs_inv[32][3][3];

double classifierFunc(uchar4* pixel, int classInd){

	double diff[3];
	double res[3];

	diff[0] = (double) pixel->x - avgs[classInd][0];
	diff[1] = (double) pixel->y - avgs[classInd][1];
	diff[2] = (double) pixel->z - avgs[classInd][2];

	res[0] = 0.0;
	res[1] = 0.0;
	res[2] = 0.0;

	for (int j = 0; j < 3; ++j) {
		for (int k = 0; k < 3; ++k) {
			res[j] += diff[k] * covs_inv[classInd][k][j];
		}
	}

	double answer = 0.0;

	for (int k = 0; k < 3; ++k) {
		answer += res[k] * diff[k];
	}
	answer *= -1.0;

	return answer;
}


void kernel(uchar4* pixels, int size, int PGP_NC){

    for (int i = 0; i < size; ++i) {

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

    return;
}

int main(){

    int w, h;
    cin >> w >> h;
    uchar4* src  = (uchar4 *) malloc(sizeof(uchar4) * w * h);
    for(int i = 0; i < w * h; ++i){
        uchar4 res;
        res.x = rand() % 256;
        res.y = rand() % 256;
        res.z = rand() % 256;
        res.w = 0;
        src[i] = res;
        //cout << src[i].x << " " <<  src[i].y << " " << src[i].z << " " << src[i].w << "\n";
    }

    int nc;
	cin >> nc;
	if( nc > 32 ){
        cout << "Why nc is greater than 32?\n";
        abort();
	}

	vector< vector< pair<int, int> > > groups(nc);


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

    set< pair<int, int> > used;
	for (int i = 0; i < nc; ++i){

		int np;
        np = 5;
		//cin >> np;



		if( np <= 1){
            cout << "Division by 0. np should be > 1\n";
            abort();
		}
		if( np >= w * h ){
            cout << "Too much points in class!!!\n";
            abort();
		}
		if( np + used.size() >= w * h ){
            cout << "Too much points at all!!!\n";
            abort();
		}
		groups[i].resize(np);

		for (int j = 0; j < np; ++j){
                int x = rand() % w;
                int y = rand() % h;
                while( used.find(make_pair(x, y)) != used.end() ){
                    x = rand() % w;
                    y = rand() % h;
                }
                used.insert(make_pair(x, y));
                groups[i][j] = make_pair(x, y);
		}
	}

	for (int i = 0; i < nc; ++i) {

		for (int j = 0; j < groups[i].size(); ++j){
			uchar4 pixel = src[groups[i][j].first + w * groups[i][j].second];
			avgs[i][0] += pixel.x;
			avgs[i][1] += pixel.y;
			avgs[i][2] += pixel.z;
		}

		avgs[i][0] /= (double) groups[i].size();
		avgs[i][1] /= (double) groups[i].size();
		avgs[i][2] /= (double) groups[i].size();

		for (int j = 0; j < groups[i].size(); ++j){

			uchar4 pixel = src[groups[i][j].first + w * groups[i][j].second];
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


		double tmp = (double) groups[i].size() - 1.0;
		for (int s1 = 0; s1 < 3; ++s1) {
			for (int s2 = 0; s2 < 3; ++s2) {
				covs[i][s1][s2] /= tmp;
			}
		}

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


    clock_t start = clock();
    kernel(src, w * h, nc);
    clock_t stop = clock();
    double time = (double) (stop - start) / CLOCKS_PER_SEC;
    cout << time << " seconds" << endl;

    free(src);
    return 0;
}
