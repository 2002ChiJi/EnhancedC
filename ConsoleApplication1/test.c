#include "CPlus.h"
#include <stdio.h>

int main()
{
	struct VectorData VD;
	FunctionVector.Init(sizeof(int), &VD);

	for (int num = 0; num < 8; num++)
		FunctionVector.PushBack(&num, &VD);
	for (int num = 7; num >= 0; num--)
		FunctionVector.Change(7 - num, &num, &VD);
	for (int num = 0; num < VD.used; num++)
		printf("%d\n",*(int*)FunctionVector.At(num, &VD));
}