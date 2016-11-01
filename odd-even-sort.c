//Odd-Even Transposition Sort in CUDA C

#include <cuda.h>
#include <stdio.h>

#define N 5

__global__ void sort(int *a) {
	int id = threadIdx.x;
	for (int i = 0; i < N; i++) { //Run N times for worst case
		if (id < N - 1) {
			if (id % 2) { //Odd case
				if (a[id] > a[id + 1]) {
					int temp = a[id];
					a[id] = a[id + 1];
					a[id + 1] = temp;
				}
			}
			__syncthreads();
			if (!(id % 2)) { //Even Case
				if (a[id] > a[id + 1]) {
					int temp = a[id];
					a[id] = a[id + 1];
					a[id + 1] = temp;
				}
			}
			__syncthreads();
		}
	}
}

int main(int argc, char const *argv[])
{
	int a[] = {4, 2, 3, 1, 5}, *da;
	cudaMalloc((void**)&da, N * sizeof(int));
	cudaMemcpy(da, a, N * sizeof(int ), cudaMemcpyHostToDevice);
	sort <<< 1, N>>>(da); //Launch N Threads
	cudaMemcpy(a, da, N * sizeof(int ), cudaMemcpyDeviceToHost);
	for (int i = 0; i < N; ++i)
		printf("%d ", a[i]);
	return 0;
}
