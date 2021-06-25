#include "list.h"

int main()
{
	struct ListData LD;
	FunctionList.ListInit(10, &LD);

	int* pt;
	for (int num = 0; num < LD.length; num++)
	{
		pt = malloc(sizeof(int));
		*pt = num;

		FunctionList.FindNode(num, &LD)->data = pt;
	}

	FunctionList.DeleteNode(9, NULL, &LD);
	FunctionList.DeleteNode(0, NULL, &LD);
	FunctionList.DeleteNode(4, NULL, &LD);

	for (int num = 0; num < LD.length; num++)
		printf("%d", *(int*)(FunctionList.FindNode(num, &LD)->data));
	return 0;
}