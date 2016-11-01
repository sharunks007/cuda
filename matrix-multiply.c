//Square Matrix Multiplication in CUDA C

#include <cuda.h>
#include <stdio.h>
#define N 3
#define size sizeof(int)
#define ID(x,y) ((x)*N)+(y)

__global__ void matmul(int *a, int *b, int *c) {
	int sum = 0;
	int x = blockIdx.x;
	int y = threadIdx.x;
	for (int k = 0; k < N; ++k)
	{
		sum += a[ID(x, k)] * b[ID(k, y)];
	}
	c[ID(x, y)] = sum;
}

int main(int argc, char const *argv[])
{
	int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int b[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
	int c[N][N];
	int *da, *db, *dc;
	int sz = N * N * size;
	cudaMalloc((void**)&da, sz);
	cudaMalloc((void**)&db, sz);
	cudaMalloc((void**)&dc, sz);

	cudaMemcpy(da, a, sz, cudaMemcpyHostToDevice);
	cudaMemcpy(db, b, sz, cudaMemcpyHostToDevice);

	matmul <<< N, N>>>(da, db, dc);
	cudaMemcpy(c, dc, sz, cudaMemcpyDeviceToHost);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			printf("%d ", c[i][j]);
		}
		printf("\n");
	}


	return 0;
}