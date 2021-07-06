#include "CPlus.h"
#include <stdio.h>

int main()
{
	int a = 10086;

	struct VectorData VD;
	FunctionVector.Init(sizeof(int), &VD);

	for (int num = 0; num < 10; num++)
		FunctionVector.PushBack(&num, &VD);
	for (int num = 0; num < VD.Length; num++)		
		printf("%d\n",*(int*)FunctionVector.At(num, &VD));
}