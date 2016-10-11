#include <cuda.h>

__global__ void part(int *a, int *pos, int size) {
	int *cp_a, i;
	int pivot, left, right, t;
	cp_a = (int*)malloc(sizeof(int) * size);
	for (i = 0; i < size; i++) { //create a copy of array a
		cp_a[i] = a[i];
	}

	//Swap first element with current (threadIdx.x element)
	t = cp_a[threadIdx.x];
	cp_a[threadIdx.x] = cp_a[0];
	cp_a[0] = t;

	//Set pivot
	pivot = cp_a[0];

	left = 1; right = size - 1;

	while (left < right) {
		while (left > 0 && left < size && cp_a[left] <= pivot)
			left++;
		while (right > 0 && right < size && cp_a[right] > pivot)
			right--;
		if (left < right && left > 0 && left < size && right > 0 && right < size) {
			t = cp_a[left];
			cp_a[left] = cp_a[right];
			cp_a[right] = t;
		}
	}
	pos[threadIdx.x] = right;
}

int main() {
	int *a, *dev_a, *pos, *dev_pos, i, *fin, sz;
	printf("\n Enter the size of the array : ");
	scanf("%d", &sz);
	a = (int*) malloc(sizeof(int) * sz);
	pos = (int*) malloc(sizeof(int) * sz);
	fin = (int*) malloc(sizeof(int) * sz);
	for (i = 0; i < sz; i++) {
		printf("\n Enter the %dth element : ", i);
		scanf("%d", a + i);
		fin[i] = INT_MAX;
	}
	cudaMalloc((void**)&dev_a, sizeof(int)*sz);
	cudaMalloc((void**)&dev_pos, sizeof(int)*sz);
	cudaMemcpy(dev_a, a, sz * sizeof(int), cudaMemcpyHostToDevice);
	part <<< 1, sz>>>(dev_a, dev_pos, sz);
	cudaMemcpy(pos, dev_pos, sz * sizeof(int), cudaMemcpyDeviceToHost);
	for (i = 0; i < sz; i++)
		fin[pos[i]] = a[i];
	for (i = sz - 1; i >= 0; i--)
		if (fin[i] == INT_MAX)
			fin[i] = fin[i + 1];
	for (i = 0; i < sz; i++)
		printf("\n %dth element : %d", i, *(fin + i));
	printf("\n\n");
	return 0;
}
