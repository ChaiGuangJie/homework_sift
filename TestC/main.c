#include<stdio.h>
#include<malloc.h>


int main()
{
	//printf("%d",sizeof(char));
	int n = 3, m = 4;

	int** arr;
	arr = (int**)malloc(n * sizeof(int*));
	for (size_t i = 0; i < n; i++)
	{
		arr[i] = (int*)malloc(m * sizeof(int));
	}

	return 0;
}
