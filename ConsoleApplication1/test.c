#include "CPlus.h"
#include <stdio.h>

/*
����GitHub�ϱ���
��ȷ��ʹ�õı�����֧��C99��׼Ŷ^_^
лл��ʦ�͹�
*/

void Bar(CPString* str)
{
	FunctionCPStr.Transform(str,FALSE);
}

int main()
{
	CPFile* file = FunctionFILE.Init("E:\\1.txt");
	FunctionGC.Add(FunctionFILE.Destory, file);
	FunctionFILE.Save(file);
}