#include <upc_relaxed.h>
#include <stdio.h>
#define N 5 * THREADS

shared int A[N], B[N];
shared int MUL[N];

int main()
{
	int i;
	upc_forall(i = 0; i < N; i++; i)
	{
		A[i] = i;
		B[i] = 2;
		MUL[i] = A[i] * B[i];
		printf("%d\n", MUL[i]);
	}

	return 0;
}