#include "CPlus.h"
#include <stdio.h>

/*
已在GitHub上备份
请确保使用的编译器支持C99标准哦^_^
谢谢老师赏光
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