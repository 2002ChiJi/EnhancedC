/*
VER  1.2
������Pointer��GC��ǰ���ṩ�˶�ָ��ļ򵥷�װ�������ܹ��ڳ������ǰ
�Զ��ͷ���ӽ�ȥ��ָ�룬����Ч��ֹ��Ϊ���Բ��õ��µ��ڴ�й©
�����˽ṹ�ľ����ֶΣ���ֹ��Ϊ��С���޸Ķ����µ����д���
�޸������ɸ�BUG
														   21.7.24
VER  1.1
�޸����ڴ�й¶��BUG���޸���List�Ľӿڣ�������list�ķ������ܣ�
������List��At�����������˲�����鹦�ܣ��޸���Init�ӿ�
														   21.7.16
VER: 1.0
���ļ��ṩ�������ɱ����飬�ַ������ļ�IO��printf��ʵ�ֻ��߰�װ
�ܹ��ڽ���C���Կ���ʱ�ṩ���ֵĹ��ߣ�����д���������ʱ�䣬�򻯿�������
���д��붼�������ԣ������������ߵ�ˮƽ��������BUG�����λС��ʹ��

���ļ��ṩ�˼��������ܣ�����λ�ڵ���ʱ������Ϊ��������ȷ����Ĵ���
���Ҫ�������붨�� _CPLUS_DEBUG_MODE_ �꣬���رգ�ɾ���ú꼴��

�������ߣ������� ��ʹ�øô���ʱ������GPL3.0Э�� ��л����ʹ��
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
������
*/
#define _ERROR_TITLE_ "��������! ���򼴽��ر� >_<"
#define _ERROR_ARGV_(argv) "\n���� " #argv " ����ȷ,���ܳ����������Ĵ�С,Ϊ��������"
#define _ERROR_NULL_  "\n��������ĳ����ԱΪNULL"
#define _ERROR_LOCATION_(FunctionName) "λ�� " __FILE__ "\n�ĺ��� " FunctionName " �������⣺"
#define _ERROR_FILE_OPEN_FAIL_ "\n�ļ��޷���"

void SendErrorMessage(const char* error);

#endif

typedef int CPBool;
#define TRUE 1
#define FALSE 0

/*��voidָ��ת����ָ���ı������ͣ�����Get(pointer,int*)��pointerת��Ϊint*��������*/
#define GetVal(VoidPointer,PointerTy) *(PointerTy)(VoidPointer) 
/*�ص�����������*/
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
��ʹ�õ�ʱ���������Լ�Ҫ�������ı�������
���ø���������ָ���ʱ����ش������������Ȩ������
�漰������Elementʱ�������и��ƣ������Element��Ϊ����(ElementΪָ�����)
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
�ײ����Vector
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
�������߼��������ص��¶�ȡ����ʱ������룬���ı��������ANSI���ɽ��
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
printf��װ
PRINTF����ܹ�һ�������10�����������ұ������Ͳ����пո�
(��������пո�Ͱѿո���Ե�����longlong)
�ַ�������Ϊstring
��Ѵ���д�ú��ã���Ҫԭ�������߲������
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
/*=================����Ϊ���Ͷ���================*/
/*=================����Ϊ�������================*/
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
��װ����ָ��
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
GC ����������
�����ڳ������ʱ�Զ��ͷű���ӵ�����
�����ֶ��ͷű���ӵ�����
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