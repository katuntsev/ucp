#include <upc_relaxed.h>
#include <stdio.h>
#include <time.h>
#define N 10000000 * THREADS
#define MAX 10000000

shared unsigned long A[N], B[N];
shared unsigned long MUL[N];

int main()
{
	clock_t start, end;
	start = clock();
	unsigned long i;
	upc_forall(i = 0; i < MAX; i++; i)
	{
		A[i] = 1;
		B[i] = 2;
		MUL[i] = A[i] * B[i];
	}
	end = clock();

	double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time = %f\n", cpu_time_used);

	return 0;
}