/*
VER  1.2
新增了Pointer和GC，前者提供了对指针的简单封装，后者能够在程序结束前
自动释放添加进去的指针，能有效防止因为记性不好导致的内存泄漏
隐藏了结构的具体字段，防止因为不小心修改而导致的运行错误
修复了若干个BUG
														   21.7.24
VER  1.1
修复了内存泄露的BUG，修改了List的接口，提升了list的访问性能，
增加了List的At函数，完善了参数检查功能，修改了Init接口
														   21.7.16
VER: 1.0
该文件提供了链表，可变数组，字符串，文件IO，printf的实现或者包装
能够在进行C语言开发时提供趁手的工具，减少写基础代码的时间，简化开发流程
所有代码都经过测试，但受限于作者的水平还难免有BUG，请各位小心使用

该文件提供了检查参数功能，供各位在调试时发现因为参数不正确引起的错误
如果要开启，请定义 _CPLUS_DEBUG_MODE_ 宏，若关闭，删除该宏即可

代码作者：曾梓浩 在使用该代码时请遵守GPL3.0协议 感谢您的使用
														   21.7.11
*/

#define _CPLUS_DEBUG_MODE_

#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _CPLUS_DEBUG_MODE_ 
#include <Windows.h>

/*
报错函数
*/
#define _ERROR_TITLE_ "致命错误! 程序即将关闭 >_<"
#define _ERROR_ARGV_(argv) "\n参数 " #argv " 不正确,可能超出了容器的大小,为负数或零"
#define _ERROR_NULL_  "\n参数或其某个成员为NULL"
#define _ERROR_LOCATION_(FunctionName) "位于 " __FILE__ "\n的函数 " FunctionName " 出现问题："
#define _ERROR_FILE_OPEN_FAIL_ "\n文件无法打开"

void SendErrorMessage(const char* error);

#endif

typedef int CPBool;
#define TRUE 1
#define FALSE 0

/*把void指针转换成指定的变量类型，比如Get(pointer,int*)把pointer转换为int*并解引用*/
#define GetVal(VoidPointer,PointerTy) *(PointerTy)(VoidPointer) 
/*回调函数的类型*/
#define CPCallBackType (void (*)(void*))

/*
List
*/

typedef struct ListNode ListNode;

typedef struct CPList CPList;

CPList* ListInit();

void ListDelSubList(void(*DestoryFunction)(void*), ListNode* node);

void ListDestory(void(*DestoryFunction)(void*), CPList* list);

inline  ListNode* ListFindNode(const int position, CPList* list);

void* ListAt(const int position, CPList* list);

void ListNodeInsertA(const int position, void* data, CPList* list);

void ListNodeInsertB(const int position, void* data, CPList* list);

void ListNodeDelete(const int position, void(*DestoryFunction)(void*), CPList* list);

void ListDelete(const int from, const int to, void(*DestoryFunction)(void*), CPList* list);

void ListPushBack(void* data, CPList* list);

inline void ListForEach(void(*Op)(void*), CPList* list);

struct
{
	CPList* (*Init)();
	void (*Destory)(void(*DestoryFunction)(void*), CPList* list);
	ListNode* (*FindNode)(const int position, CPList* list);
	void* (*At)(const int position, CPList* list);
	void (*InsertA)(const int position, void* data, CPList* list);
	void (*InsertB)(const int position, void* data, CPList* list);
	void (*DeleteNode)(const int position, void(*DestoryFunction)(void*), CPList* list);
	void (*DeleteList)(const int from, const int to, void(*DestoryFunction)(void*), CPList* list);
	void (*PushBack)(void* data, CPList* list);
	void (*ForEach)(void(*Op)(void*), CPList* list);
}static const FunctionList =
{
&ListInit,&ListDestory,&ListFindNode,&ListAt,&ListNodeInsertA,
&ListNodeInsertB,&ListNodeDelete,&ListDelete,&ListPushBack,
&ListForEach
};

/*
Vector
在使用的时候务必清楚自己要往里面存的变量类型
在用该容器管理指针的时候务必处理好数据所有权的问题
涉及到传递Element时仅仅进行复制，不会把Element据为己有(Element为指针除外)
*/

typedef struct CPVector CPVector;

CPVector* VectorInit(unsigned char ElenmentSize, void(*DestoryFunction)(void*));

void VectorResize(const unsigned newLength, CPVector* vector);

void VectorPushBack(void* Element, CPVector* vector);

void VectorInsertA(const unsigned position, const void* Element, CPVector* vector);

void VectorInsertB(const unsigned position, const void* Element, CPVector* vector);

void VectorRemove(const unsigned position, CPVector* vector);

inline void VectorForEach(void(*Op)(void*), CPVector* vector);

void VectorDestory(CPVector* vector);

inline void VectorEmpty(CPVector* vector);

inline const void* VectorAt(const unsigned position, CPVector* vector);

inline void VectorChange(const unsigned position, void* Element, CPVector* vector);

struct
{
	CPVector* (*Init)(unsigned char ElenmentSize, void(*DestoryFunction)(void*));
	void (*Resize)(const unsigned newLength, CPVector* vector);
	void (*PushBack)(void* Element, CPVector* vector);
	void (*InsertA)(const unsigned position, const void* Element, CPVector* vector);
	void (*InsertB)(const unsigned position, const void* Element, CPVector* vector);
	void (*Remove)(const unsigned position, CPVector* vector);
	void (*Destory)(CPVector* vector);
	void (*Empty)(CPVector* vector);
	const void* (*At)(const unsigned position, CPVector* vector);
	void (*Change)(const unsigned position, void* Element, CPVector* vector);
	void (*ForEach)(void(*Op)(void*), CPVector* vector);
}static FunctionVector =
{
	&VectorInit,&VectorResize,&VectorPushBack,&VectorInsertA,&VectorInsertB,
	&VectorRemove,&VectorDestory,&VectorEmpty,&VectorAt,&VectorChange,&VectorForEach
};

/*
CPString
底层采用Vector
*/

typedef struct CPString CPString;

CPString* CPStrInit(const char* str);

inline void CPStrDestory(CPString* CPStr);

inline void CPStrEmpty(CPString* CPStr);

inline const char* CPStrAt(const unsigned position, CPString* CPStr);

inline char* CPStrc_str(CPString* CPStr);

void CPStrCover(const unsigned position, const  char* str, CPString* CPStr);

void CPStrRemove(const unsigned from, const unsigned to, CPString* CPStr);

void CPStrAppendStr(const char* str, CPString* CPStr);

void CPStrAppendChar(const char chr, CPString* CPStr);

void CPStrInsertA(const unsigned position, const char* str, CPString* CPStr);

void CPStrInsertB(const unsigned position, const char* str, CPString* CPStr);

inline void CPStrShorten(const unsigned newLength, CPString* CPStr);

inline void CPStrCpy(const char* str, CPString* CPStr);

int CPStrFind(const unsigned start, const char* str, CPString* CPStr);

inline unsigned CPStrCount(const char* str, CPString* CPStr);

void CPStrTransform(CPString* CPStr, CPBool IsLower);

void CPStrRelpace(const char* from, const char* to, CPString* CPStr);

inline int CPStrLen(CPString* CPStr);

char* CPStrSplit(const unsigned from, const unsigned to, CPString* CPStr);

struct
{
	CPString* (*Init)(const char* str);
	void (*Destory)(CPString* CPStr);
	void (*Empty)(CPString* CPStr);
	const char* (*At)(const unsigned position, CPString* CPStr);
	void (*Cover)(const unsigned position, const  char* str, CPString* CPStr);
	void (*Remove)(const unsigned from, const unsigned to, CPString* CPStr);
	void (*AppendStr)(const char* str, CPString* CPStr);
	void (*AppendChar)(const char Element, CPString* CPStr);
	void (*InsertA)(const unsigned position, const char* str, CPString* CPStr);
	void (*InsertB)(const unsigned position, const char* str, CPString* CPStr);
	void (*Shorten)(const unsigned newLength, CPString* CPStr);
	void (*Cpy)(const char* str, CPString* CPStr);
	int (*StrFind)(const unsigned start, const char* str, CPString* CPStr);
	unsigned (*Count)(const char* str, CPString* CPStr);
	void (*Transform)(CPString* CPStr, CPBool IsLower);
	void (*Relpace)(const char* from, const char* to, CPString* CPStr);
	int (*StrLen)(CPString* CPStr);
	char* (*Split)(const unsigned from, const unsigned to, CPString* CPStr);
	char* (*c_str)(CPString* CPStr);
}static const FunctionCPStr =
{
&CPStrInit,&CPStrDestory,&CPStrEmpty,&CPStrAt,&CPStrCover,&CPStrRemove,
&CPStrAppendStr,&CPStrAppendChar,&CPStrInsertA,&CPStrInsertB,&CPStrShorten,&CPStrCpy,&CPStrFind,
&CPStrCount,&CPStrTransform,&CPStrRelpace,&CPStrLen,&CPStrSplit,&CPStrc_str
};

/*
FILE
由于作者技术不过关导致读取中文时候会乱码，把文本编码调成ANSI即可解决
*/

typedef struct CPFile CPFile;

CPFile* FILEInit(const char* FilePath);

void FILEDestory(CPFile* file);

inline CPString* FILEAt(const unsigned position, CPFile* file);

inline void FILESave(CPFile* file);

inline void FILEDeleteLine(const unsigned from, const unsigned to, CPFile* file);

inline void FILERepleace(const char* from, const char* to, CPFile* file);

inline void FILEPushBack(const char* str, CPFile* file);

inline void FILEInsertLineA(const unsigned position, const char* str, CPFile* file);

inline void FILEInsertLineB(const unsigned position, const char* str, CPFile* file);

inline void FILEForEach(void(*Op)(CPString*), CPFile* file);

struct
{
	CPFile* (*Init)(const char* FilePath);
	void (*Destory)(CPFile* file);
	CPString* (*At)(const unsigned position, CPFile* file);
	void (*Save)(CPFile* file);
	void (*DeleteLine)(const unsigned from, const unsigned to, CPFile* file);
	void (*Repleace)(const char* from, const char* to, CPFile* file);
	void (*PushBack)(const char* str, CPFile* file);
	void (*InsertLineA)(const unsigned position, const char* str, CPFile* file);
	void (*InsertLineB)(const unsigned position, const char* str, CPFile* file);
	void (*ForEach)(void (*Op)(CPString*), CPFile* file);
}const static FunctionFILE =
{
&FILEInit,&FILEDestory,&FILEAt,&FILESave,&FILEDeleteLine,
&FILERepleace,&FILEPushBack,&FILEInsertLineA,&FILEInsertLineB,&FILEForEach,
};

/*
printf包装
PRINTF最多能够一次性输出10个变量，而且变量类型不能有空格
(如果类型有空格就把空格忽略掉，如longlong)
字符串类型为string
这堆代码写得很烂，主要原因是作者不会宏编程
*/

#define INT "%d"
#define LONG "%ld"
#define LONGINT "%ld"
#define LONGLONG "%lld"
#define LONGLONGINT "%lld"
#define UNSIGNED "%u"
#define UNSIGNEDINT "%u"

#define CHAR "%c"
#define STRING "%s"

#define FLOAT "%f"
#define DOUBLE "%lf"
#define LONGDOUBLE "%Lf"
/*=================以上为类型定义================*/
/*=================以下为输出函数================*/
#define CPlusPrintf()

#define CPlusPrintfint(pt) printf(INT, pt);
#define CPlusPrintflong(pt) printf(LONG, pt);
#define CPlusPrintflongint(pt) printf(LONG pt);
#define CPlusPrintflonglong(pt) printf(LONG,pt);
#define CPlusPrintflonglongint(pt) printf(LONGLONG,pt);
#define CPlusPrintfunsigned(pt) printf(UNSIGNED, pt);
#define CPlusPrintfunsignedint(pt) printf(UNSIGNED, pt);

#define CPlusPrintfchar(pt) printf(CHAR, pt);
#define CPlusPrintfstring(pt) printf(STRING, pt);

#define CPlusPrintffloat(pt) printf(FLOAT, pt);
#define CPlusPrintfdouble(pt) printf(DOUBLE, pt);
#define CPlusPrintflongdouble  printf(LONGDOUBLE, pt);

#define PRINTF(type1,argv1,type2,argv2,type3,argv3,type4,argv4,type5,argv5,type6,argv6,type7,argv7,type8,argv8,type9,argv9,type10,argv10)\
do{\
CPlusPrintf##type1(argv1) CPlusPrintf##type2(argv2) CPlusPrintf##type3(argv3) CPlusPrintf##type4(argv4) CPlusPrintf##type5(argv5)\
CPlusPrintf##type6(argv6) CPlusPrintf##type7(argv7) CPlusPrintf##type8(argv8) CPlusPrintf##type9(argv9) CPlusPrintf##type10(argv10)\
}while(0);

/*
包装过的指针
*/

typedef struct CPPointer CPPointer;

CPPointer* SmartPointerInit(void* Memory, void (*DestoryFunction)(void*));

void* SmartPointerGet(CPPointer* SmartPt);

void SmartPointerDestory(CPPointer* SmartPt);

struct
{
	void* (*Get)(CPPointer* SmartPt);
	void (*Destory)(CPPointer* SmartPt);
	CPPointer* (*Init)(void* Memory, void (*DestoryFunction)(void*));
}const static FunctionSmartPointer = { &SmartPointerGet,&SmartPointerDestory,&SmartPointerInit };

/*
GC 垃圾回收器
负责在程序结束时自动释放被添加的内容
不得手动释放被添加的内容
*/

struct
{
	CPVector* vector;
}static GC = { NULL };

inline void GCInit();

inline void GCDestory();

inline void GCAdd(void(*DestoryFunction)(void*), void* pointer);

struct
{
	void* (*Init)();
	void (*Destory)();
	CPPointer* (*Add)(void(*DestoryFunction)(void*), void* pointer);
}const static FunctionGC = { &GCInit,&GCDestory,&GCAdd };