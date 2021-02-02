
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace std;
typedef long long ll;


__global__ void kernel(double* a, double* b, double* answer, ll n)
{
    ll offset = gridDim.x * blockDim.x;
    ll idx = blockDim.x * blockIdx.x + threadIdx.x;
    for(ll i = idx; i < n; i = i + offset){
        answer[i] = a[i] - b[i];
    }
    return;
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll n;
    cin >> n;

    double* a = new double[n];
    double* b = new double[n];
    double* answer = new double[n];

    for (ll i = 0; i < n; ++i) {
        cin >> a[i];
    }
    for (ll i = 0; i < n; ++i) {
        cin >> b[i];
    }

    double* nums1;
    double* nums2;
    double* res;

    if (cudaMalloc((void**)&nums1, sizeof(double) * n) != cudaSuccess) {
        cerr << "ERROR!   46\n";
        abort();
    }
    if (cudaMalloc((void**)&nums2, sizeof(double) * n) != cudaSuccess) {
        cerr << "ERROR!   50\n";
        abort();
    }
    if (cudaMalloc((void**)&res, sizeof(double) * n) != cudaSuccess) {
        cerr << "ERROR!   54\n";
        abort();
    }

    if (cudaMemcpy(nums1, a, sizeof(double) * n, cudaMemcpyHostToDevice) != cudaSuccess) {
        cerr << "ERROR!   59\n";
        abort();
    }
    if (cudaMemcpy(nums2, b, sizeof(double) * n, cudaMemcpyHostToDevice) != cudaSuccess) {
        cerr << "ERROR!   63\n";
        abort();
    }

    kernel <<<256, 256>>> (nums1, nums2, res, n);

    if (cudaGetLastError() != cudaSuccess) {
        cerr << "ERROR!   70\n";
        abort();
    }

    if (cudaMemcpy(answer, res, sizeof(double) * n, cudaMemcpyDeviceToHost) != cudaSuccess) {
        cerr << "ERROR!   75\n";
        abort();
    }

    cout.precision(10);
    cout.setf(ios::scientific);
    for (ll i = 0; i < n; ++i) {
        cout << answer[i] << " ";
    }
    cout << "\n";

    if (cudaFree(nums1) != cudaSuccess) {
        cerr << "ERROR!   87\n";
        abort();
    }
    if (cudaFree(nums2) != cudaSuccess) {
        cerr << "ERROR!   91\n";
        abort();
    }
    if (cudaFree(res) != cudaSuccess) {
        cerr << "ERROR!   95\n";
        abort();
    }

    delete[] a;
    delete[] b;
    delete[] answer;

    return 0;
}
