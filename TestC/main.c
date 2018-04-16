//#include<stdio.h>
//#include<malloc.h>
//#include<stdlib.h>
//
//
//int main()
//{
//	//printf("%d",sizeof(char));
//	int n = 3, m = 4;
//
//	int** arr;
//	arr = (int**)malloc(n * sizeof(int*));
//	for (size_t i = 0; i < n; i++)
//	{
//		arr[i] = (int*)malloc(m * sizeof(int));
//	}
//
//	int* iPtr = (int*)malloc((10)*sizeof(int));
//
//	int len = sizeof(*iPtr);
//	printf("%d\n", len);
//
//	int iArr[5] = { 1,2,3,4,5 };
//	len = sizeof(iArr) / sizeof(int*);
//	printf("%d\n", len);
//	
//	system("pause");
//	return 0;
//}

#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char str[50] = "Hello ";
	char str2[50] = "World!";
	strcat(str, str2);
	strcat(str, " ...");
	strcat(str, " Goodbye World!");
	puts(str);

#ifdef __STDC_LIB_EXT1__
	set_constraint_handler_s(ignore_handler_s);
	int r = strcat_s(str, sizeof str, " ... ");
	printf("str = \"%s\", r = %d\n", str, r);
	r = strcat_s(str, sizeof str, " and this is too much");
	printf("str = \"%s\", r = %d\n", str, r);
#endif
}