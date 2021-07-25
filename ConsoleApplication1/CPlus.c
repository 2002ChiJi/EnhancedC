#pragma once
#include "CPlus.h"

void SendErrorMessage(const char* error)
{
	MessageBoxA(NULL, error, _ERROR_TITLE_, MB_ICONERROR | MB_OK);
	exit(-1);
}

/*
List
*/

typedef struct ListNode
{
	struct ListNode* next;
	void* data;
}ListNode;

typedef struct CPList
{
	ListNode* head, * end, * last;
	int length, LastPosition;
}CPList;

CPList* ListInit()
{
	CPList* list = (CPList*)malloc(sizeof(CPList));

	list->last = list->end = list->head = NULL;
	list->LastPosition = list->length = 0;

	return list;
}

void ListDelSubList(void(*DestoryFunction)(void*), ListNode* node)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (node == NULL)
		SendErrorMessage(_ERROR_LOCATION_("ListDelSubList") _ERROR_NULL_);
#endif

	if (DestoryFunction == NULL)
	{
		for (ListNode* now; node != NULL;)
		{
			now = node;
			node = node->next;

			free(now->data);
			free(now);
		}
	}
	else
	{
		for (ListNode* now; node != NULL;)
		{
			now = node;
			node = node->next;

			DestoryFunction(now->data);
			free(now->data);
			free(now);
		}
	}
}

void ListDestory(void(*DestoryFunction)(void*), CPList* list)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (list->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.Destory") _ERROR_NULL_);
#endif

	ListDelSubList(DestoryFunction, list->head);
	list->length = 0;
	list->end = list->head = NULL;
}

inline  ListNode* ListFindNode(const int position, CPList* list) //查找第position个元素
{
#ifdef _CPLUS_DEBUG_MODE_
	if (list->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.FindNode") _ERROR_NULL_);
	if (list->length <= position || position < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.FindNode") _ERROR_ARGV_(position));
#endif

	if (list->last == list->end || list->last == NULL)
	{
		list->LastPosition = 0;
		list->last = list->head;
	}

	if (position >= list->LastPosition)
	{
		int newPosition = position - list->LastPosition;
		for (int num = 0; num < newPosition; num++)
			list->last = list->last->next;

		list->LastPosition = position;
		return list->last;
	}
	else
	{
		ListNode* pos = list->head;
		for (int num = 0; num < position; num++)
			pos = pos->next;

		list->last = pos;
		list->LastPosition = position;

		return pos;
	}
}

void* ListAt(const int position, CPList* list)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (list->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.At") _ERROR_NULL_);
	if (list->length <= position || position < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.At") _ERROR_ARGV_(position));
#endif
	return ListFindNode(position, list)->data;
}

void ListNodeInsertA(const int position, void* data, CPList* list)  //插入到position前面
{
#ifdef _CPLUS_DEBUG_MODE_
	if (list->head == NULL || data == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.InsertA") _ERROR_NULL_);
	if (list->length <= position || position < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.InsertA") _ERROR_ARGV_(position));
#endif

	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	node->data = data;

	if (position == 0)
	{
		node->next = list->head;
		list->head = node;
	}
	else
	{
		ListNode* pos = ListFindNode(position - 1, list);
		node->next = pos->next;
		pos->next = node;
	}
	list->length += 1;
}

void ListNodeInsertB(const int position, void* data, CPList* list)  //插入到position后面
{
#ifdef _CPLUS_DEBUG_MODE_
	if (list->head == NULL || data == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.InsertB") _ERROR_NULL_);
	if (list->length <= position || position < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.InsertB") _ERROR_ARGV_(position));
#endif

	ListNode* pos = ListFindNode(position, list);
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));

	node->data = data;
	node->next = pos->next;
	pos->next = node;

	if (position == list->length - 1)
		list->end = node;
	list->length += 1;
}

void ListNodeDelete(const int position, void(*DestoryFunction)(void*), CPList* list) //删除第position个元素
{
#ifdef _CPLUS_DEBUG_MODE_
	if (list->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.DeleteNode") _ERROR_NULL_);
	if (list->length <= position || position < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.DeleteNode") _ERROR_ARGV_(position));
#endif

	ListNode* Deleted;
	if (position == 0)
	{
		Deleted = list->head;
		list->head = list->head->next;
	}
	else
	{
		ListNode* pos = ListFindNode(position - 1, list);
		Deleted = pos->next;
		pos->next = Deleted->next;

		if (position == list->length - 1)
			list->end = pos;
	}

	if (DestoryFunction == NULL)
		free(Deleted->data);
	else
		DestoryFunction(Deleted->data);
	free(Deleted);

	list->length -= 1;
}

void ListDelete(const int from, const int to, void(*DestoryFunction)(void*), CPList* list) //删除[from,to]区间内的链表
{
#ifdef _CPLUS_DEBUG_MODE_
	if (list->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.DeleteList") _ERROR_NULL_);
	if (to >= list->length || from > to || from < 0 || to < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.DeleteList") _ERROR_ARGV_("from 或者 to"));
#endif

	ListNode* Deleted, * To = ListFindNode(to, list);
	if (from == 0)
	{
		Deleted = list->head;
		list->head = To->next;
	}
	else
	{
		ListNode* From = ListFindNode(from - 1, list);
		Deleted = From->next;
		From->next = To->next;

		if (to == list->length - 1)
			list->end = From;
	}

	To->next = NULL;
	ListDelSubList(DestoryFunction, Deleted);
	list->length -= (to - from + 1);
}

void ListPushBack(void* data, CPList* list)
{
	ListNode* NewNode = (ListNode*)malloc(sizeof(ListNode));
	NewNode->data = data;
	NewNode->next = NULL;

	if (list->head == NULL)
		list->head = list->end = NewNode;
	else
	{
		list->end->next = NewNode;
		list->end = NewNode;
	}
	list->length++;
}

inline void ListForEach(void(*Op)(void*), CPList* list)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (Op == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionList.ForEach") _ERROR_NULL_);
#endif

	for (ListNode* now = list->head; now != NULL; now = now->next)
		Op(now->data);
}

/*
Vector
在使用的时候务必清楚自己要往里面存的变量类型
在用该容器管理指针的时候务必处理好数据所有权的问题
涉及到传递Element时仅仅进行复制，不会把Element据为己有(Element为指针除外)
*/

typedef struct CPVector
{
	void(*DestoryFunction)(void*);
	unsigned char ElementSize;
	unsigned Length, used;
	void* VectorHead;
}CPVector;

CPVector* VectorInit(unsigned char ElenmentSize, void(*DestoryFunction)(void*))
{
	CPVector* vector = (CPVector*)malloc(sizeof(CPVector));

	vector->DestoryFunction = DestoryFunction;
	vector->ElementSize = ElenmentSize;
	vector->VectorHead = malloc(0);
	vector->Length = 0;
	vector->used = 0;

	return vector;
}

void VectorResize(const unsigned newLength, CPVector* vector)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (vector->VectorHead == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.Resize") _ERROR_NULL_);
	if (newLength <= 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.Resize") _ERROR_ARGV_(newLength));
#endif

	void* temp = realloc(vector->VectorHead, newLength * vector->ElementSize);

	vector->VectorHead = temp;
	vector->Length = newLength;
	vector->used =
		(vector->used > newLength ? newLength : vector->used);
}

void VectorPushBack(void* Element, CPVector* vector)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (Element == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.PushBuck") _ERROR_NULL_);
#endif

	if (vector->used == vector->Length)
		VectorResize((vector->Length + 1) * 2, vector);

	void* pt = ((char*)vector->VectorHead) + (vector->used * vector->ElementSize);
	memmove(pt, Element, vector->ElementSize);

	vector->used++;
}

void VectorInsertA(const unsigned position, const void* Element, CPVector* vector)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (vector->VectorHead == NULL || Element == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.InsertA") _ERROR_NULL_);
	if (position < 0 || position >= vector->used)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.InsertA") _ERROR_ARGV_(position));
#endif

	if (vector->used == vector->Length)
		VectorResize((vector->Length) * 2, vector);

	int size = vector->ElementSize;
	char* head = (char*)vector->VectorHead;
	memmove((head + (position + 1) * size), (head + position * size), (vector->used - position) * size);
	memmove((head + position * size), Element, size);

	vector->used++;
}

void VectorInsertB(const unsigned position, const void* Element, CPVector* vector)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (vector->VectorHead == NULL || Element == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.InsertB") _ERROR_NULL_);
	if (position < 0 || position >= vector->used)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.InsertB") _ERROR_ARGV_(position));
#endif

	if (vector->used == vector->Length)
		VectorResize((vector->Length) * 2, vector);

	int size = vector->ElementSize;
	char* head = (char*)vector->VectorHead;
	memmove((head + (position + 2) * size), (head + (position + 1) * size), (vector->used - position) * size);
	memmove((head + (position + 1) * size), Element, size);

	vector->used++;
}

void VectorRemove(const unsigned position, CPVector* vector)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (vector->VectorHead == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.Remove") _ERROR_NULL_);
	if (position < 0 || position >= vector->used)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.Remove") _ERROR_ARGV_(position));
#endif

	int size = vector->ElementSize;
	char* head = (char*)vector->VectorHead;
	memmove((head + (position)*size), (head + (position + 1) * size), (vector->used - position + 1) * size);

	vector->used--;
}

inline void VectorForEach(void(*Op)(void*), CPVector* vector)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (Op == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.ForEach") _ERROR_NULL_);
#endif

	char* now = (char*)vector->VectorHead;
	for (int counter = 0; counter < vector->used; counter++, now += vector->ElementSize)
		Op(now);
}

void VectorDestory(CPVector* vector)
{
	if (vector->DestoryFunction != NULL)
		VectorForEach(vector->DestoryFunction, vector);
	free(vector->VectorHead);

	vector->VectorHead = NULL;
	vector->ElementSize = vector->Length = vector->used = 0;
}

inline void VectorEmpty(CPVector* vector)
{
	vector->used = 0;
}

inline const void* VectorAt(const unsigned position, CPVector* vector)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (vector->VectorHead == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.At") _ERROR_NULL_);
	if (position < 0 || position >= vector->used)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.At") _ERROR_ARGV_(position));
#endif

	return ((char*)vector->VectorHead) + (position * vector->ElementSize);
}

inline void VectorChange(const unsigned position, void* Element, CPVector* vector)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (vector->VectorHead == NULL || Element == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.Change") _ERROR_NULL_);
	if (position < 0 || position >= vector->used)
		SendErrorMessage(_ERROR_LOCATION_("FunctionVector.Change") _ERROR_ARGV_(position));
#endif

	void* pt = ((char*)vector->VectorHead) + (position * vector->ElementSize);
	memmove(pt, Element, vector->ElementSize);
}

/*
CPString
底层采用Vector
*/

typedef struct CPString
{
	CPVector* data;
}CPString;

CPString* CPStrInit(const char* str)
{
	CPString* CPStr = (CPString*)malloc(sizeof(CPString));

	CPStr->data = FunctionVector.Init(sizeof(char), NULL);
	if (str == NULL) str = "";

	FunctionVector.Resize(strlen(str) + 1, CPStr->data);
	memmove(CPStr->data->VectorHead, str, strlen(str) + 1);
	CPStr->data->used = strlen(str) + 1;

	return CPStr;
}

inline void CPStrDestory(CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.Destory") _ERROR_NULL_);
#endif

	FunctionVector.Destory(CPStr->data);
}

inline void CPStrEmpty(CPString* CPStr)
{
	memset(CPStr->data->VectorHead, '\0', CPStr->data->used);
	FunctionVector.Empty(CPStr->data);
}

inline const char* CPStrAt(const unsigned position, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.At") _ERROR_NULL_);
	if (position < 0 || position >= CPStr->data->used)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.At") _ERROR_ARGV_(position));
#endif

	return (char*)(FunctionVector.At(position, CPStr->data));
}

inline char* CPStrc_str(CPString* CPStr)
{
	return  (char*)(CPStr->data->VectorHead);
}

void CPStrCover(const unsigned position, const  char* str, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL || str == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.Cover") _ERROR_NULL_);
	if (position < 0 || position >= CPStr->data->used)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.Cover") _ERROR_ARGV_(position));
#endif

	const unsigned  len = strlen(str);
	if (len > CPStr->data->used - position - 1)
	{
		FunctionVector.Resize((len + position + 1) * 2, CPStr->data);
		CPStr->data->used = len + position + 1;
		CPStrc_str(CPStr)[CPStr->data->used - 1] = '\0';
	}

	memmove((char*)(CPStr->data->VectorHead) + position, str, len);
}

void CPStrRemove(const unsigned from, const unsigned to, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.Remove") _ERROR_NULL_);
	if (to >= CPStr->data->used || from > to || from < 0 || to < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.Remove") _ERROR_ARGV_("from 或者 to"));
#endif

	if (from == to)
		FunctionVector.Remove(from, CPStr->data);
	else
	{
		char* head = CPStrc_str(CPStr);
		memmove((head + from), (head + to + 1), CPStr->data->used - to - 1);
		CPStr->data->used -= (to - from + 1);
	}
}

void CPStrAppendStr(const char* str, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (str == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.AppendStr") _ERROR_NULL_);
#endif

	if (str == "")return;

	int len = strlen(str);
	if (len + CPStr->data->used > CPStr->data->Length)
		FunctionVector.Resize((len + CPStr->data->used) * 2, CPStr->data);

	memmove(CPStrc_str(CPStr) + CPStr->data->used - 1, str, len + 1);
	CPStr->data->used += len;
}

void CPStrAppendChar(const char chr, CPString* CPStr)
{
	char str[2] = { chr,'\0' };
	CPStrAppendStr(str, CPStr);
}

void CPStrInsertA(const unsigned position, const char* str, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL || str == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.InsertA") _ERROR_NULL_);
	if (CPStr->data->used <= position || position < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPstr.InsertA") _ERROR_ARGV_(position));
#endif

	int len = strlen(str);
	if (len + CPStr->data->used > CPStr->data->Length)
		FunctionVector.Resize((len + CPStr->data->used) * 2, CPStr->data);

	char* head = CPStrc_str(CPStr);
	memmove(head + position + len, head + position, CPStr->data->used - position);
	memmove(head + position, str, len);

	CPStr->data->used += len;
	head[CPStr->data->used - 1] = '\0';
}

void CPStrInsertB(const unsigned position, const char* str, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL || str == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.InsertB") _ERROR_NULL_);
	if (CPStr->data->used <= position || position < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPstr.InsertB") _ERROR_ARGV_(position));
#endif

	int len = strlen(str);
	if (len + CPStr->data->used > CPStr->data->Length)
		FunctionVector.Resize((len + CPStr->data->used) * 2, CPStr->data);

	char* head = CPStrc_str(CPStr);
	memmove(head + position + len + 1, head + position + 1, CPStr->data->used - position - 1);
	memmove(head + position + 1, str, len);

	CPStr->data->used += len;
	head[CPStr->data->used - 1] = '\0';
}

inline void CPStrShorten(const unsigned newLength, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->used <= newLength)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPstr.Shorten") _ERROR_ARGV_(newLength));
#endif

	CPStr->data->used = newLength + 1;
	*((char*)(CPStr->data->VectorHead) + (newLength)) = '\0';
}

inline void CPStrCpy(const char* str, CPString* CPStr)		//复制后原CPStr的字符串会被清空
{
	CPStrDestory(CPStr);
	CPString* temp = CPStrInit(str);

	CPStr->data->DestoryFunction = NULL;
	CPStr->data->ElementSize = sizeof(char);
	CPStr->data->Length = temp->data->Length;
	CPStr->data->used = temp->data->used;
	CPStr->data->VectorHead = temp->data->VectorHead;
}

int CPStrFind(const unsigned start, const char* str, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL || str == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.StrFind") _ERROR_NULL_);
	if (CPStr->data->used <= start || start < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPstr.StrFind") _ERROR_ARGV_(start));
#endif

	char* head = (char*)(CPStr->data->VectorHead);
	int pos = (int)((strlen(str) == 1) ?
		strchr((head + start), str[0]) : strstr((head + start), str));

	if (pos == 0)return -1;
	else return (int)pos - (int)(CPStr->data->VectorHead);
}

inline unsigned CPStrCount(const char* str, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL || str == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.Count") _ERROR_NULL_);
#endif

	int start = 0, len = strlen(str), pos, count = 0;
	for (;; start = (pos + len), count++)
	{
		pos = CPStrFind(start, str, CPStr);
		if (pos == -1)break;
	}
	return count;
}

void CPStrTransform(CPString* CPStr, CPBool IsLower)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.Transform") _ERROR_NULL_);
#endif

	char* head = (char*)CPStr->data->VectorHead;

	if (IsLower)
	{
		for (; (*head) != '\0'; head++)
			if ((*head) >= 'A' && (*head) <= 'Z') (*head) += 32;
	}
	else
	{
		for (; (*head) != '\0'; head++)
			if ((*head) >= 'a' && (*head) <= 'z') (*head) -= 32;
	}
}

void CPStrRelpace(const char* from, const char* to, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL || from == NULL || to == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.Replace") _ERROR_NULL_);
#endif

	if (CPStrFind(0, from, CPStr) != -1)
	{
		CPString* buffer = CPStrInit(NULL);
		char* head = CPStrc_str(CPStr);

		for (unsigned num = 0; num < CPStr->data->used; num++)
		{
			if (!strncmp(head + num, from, strlen(from)))
			{
				CPStrAppendStr(to, buffer);
				num += strlen(from) - 1;
			}
			else
				CPStrAppendChar(head[num], buffer);
		}
		FunctionCPStr.Cpy(FunctionCPStr.c_str(buffer), CPStr);
		FunctionCPStr.Destory(buffer);
	}
}

inline int CPStrLen(CPString* CPStr)
{
	return CPStr->data->used - 1;
}

char* CPStrSplit(const unsigned from, const unsigned to, CPString* CPStr)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (CPStr->data->VectorHead == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.Split") _ERROR_NULL_);
	if (to >= CPStr->data->used || from > to || from < 0 || to < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionCPStr.Split") _ERROR_ARGV_("from 或者 to"));
#endif

	char* ret = (char*)malloc(to - from + 2),
		* begin = (char*)(CPStr->data->VectorHead) + from;

	memmove(ret, begin, to - from + 1);
	ret[to - from + 1] = '\0';

	return ret;
}

/*
FILE
由于作者技术不过关导致读取中文时候会乱码，把文本编码调成ANSI即可解决
*/

typedef struct CPFile
{
	char* FilePath;
	unsigned FileLine;
	CPList* CPFile;
}CPFile;

CPFile* FILEInit(const char* FilePath)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (FilePath == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.Init") _ERROR_NULL_);
#endif

	CPString* temp;
	char* buffer = (char*)malloc(1024);
	FILE* pFile = fopen(FilePath, "r");

#ifdef _CPLUS_DEBUG_MODE_
	if (pFile == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.Init") _ERROR_FILE_OPEN_FAIL_);
#endif

	CPFile* file = (CPFile*)malloc(sizeof(CPFile));

	file->FileLine = 0;
	file->FilePath = (char*)malloc(strlen(FilePath) + 1);
	memmove(file->FilePath, FilePath, (strlen(FilePath) + 1));

	file->CPFile = FunctionList.Init();

	while (fgets(buffer, 1024, pFile) != NULL)
	{
		temp = FunctionCPStr.Init(buffer);
		FunctionCPStr.c_str(temp)[temp->data->used - 2] = '\0';
		temp->data->used--;

		FunctionList.PushBack(temp, file->CPFile);

		file->FileLine++;
	}

	fclose(pFile);
	free(buffer);

	return file;
}

void FILEDestory(CPFile* file)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (file->CPFile->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.Destory") _ERROR_NULL_);
#endif

	file->FileLine = 0;
	free(file->FilePath);
	file->FilePath = NULL;

	FunctionList.Destory(CPCallBackType(FunctionCPStr.Destory), file->CPFile);
	file->CPFile->end = file->CPFile->head = NULL;
	file->CPFile->length = 0;
}

inline CPString* FILEAt(const unsigned position, CPFile* file)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (file->CPFile->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.At") _ERROR_NULL_);
	if (position < 0 || position >= file->CPFile->length)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.At") _ERROR_ARGV_(position));
#endif

	return (CPString*)(FunctionList.FindNode(position, file->CPFile)->data);
}

inline void FILESave(CPFile* file)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (file->CPFile->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.Save") _ERROR_NULL_);
#endif

	FILE* FilePt = fopen(file->FilePath, "w");

	for (ListNode* now = file->CPFile->head; now != NULL; now = now->next)
		fprintf(FilePt, "%s\n", FunctionCPStr.c_str((CPString*)(now->data)));

	fclose(FilePt);
}

inline void FILEDeleteLine(const unsigned from, const unsigned to, CPFile* file)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (file->CPFile->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.DeleteLine") _ERROR_NULL_);
	if (to >= file->CPFile->length || from > to || from < 0 || to < 0)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.DeleteLine") _ERROR_ARGV_("from 或者 to"));
#endif

	if (from == to)
		FunctionList.DeleteNode(from, CPCallBackType(FunctionCPStr.Destory), file->CPFile);
	else
		FunctionList.DeleteList(from, to, CPCallBackType(FunctionCPStr.Destory), file->CPFile);
}

inline void FILERepleace(const char* from, const char* to, CPFile* file)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (file->CPFile->head == NULL || from == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.Replace") _ERROR_NULL_);
#endif

	for (ListNode* now = file->CPFile->head; now != NULL; now = now->next)
		FunctionCPStr.Relpace(from, to, (CPString*)(now->data));
}

inline void FILEPushBack(const char* str, CPFile* file)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (file->CPFile->head == NULL || str == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.PushBack") _ERROR_NULL_);
#endif

	CPString* NewElem = FunctionCPStr.Init(str);
	FunctionList.PushBack(NewElem, file->CPFile);
}

inline void FILEInsertLineA(const unsigned position, const char* str, CPFile* file)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (file->CPFile->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.InsertLineA") _ERROR_NULL_);
	if (position < 0 || position >= file->CPFile->length)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.InsertLineA") _ERROR_ARGV_(position));
#endif

	CPString* NewElem = FunctionCPStr.Init(str);
	FunctionList.InsertA(position, NewElem, file->CPFile);
}

inline void FILEInsertLineB(const unsigned position, const char* str, CPFile* file)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (file->CPFile->head == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.InsertLineB") _ERROR_NULL_);
	if (position < 0 || position >= file->CPFile->length)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.InsertLineB") _ERROR_ARGV_(position));
#endif

	CPString* NewElem = FunctionCPStr.Init(str);
	FunctionList.InsertB(position, NewElem, file->CPFile);
}

inline void FILEForEach(void(*Op)(CPString*), CPFile* file)
{
#ifdef _CPLUS_DEBUG_MODE_
	if (Op == NULL)
		SendErrorMessage(_ERROR_LOCATION_("FunctionFILE.ForEach") _ERROR_NULL_);
#endif

	for (ListNode* now = file->CPFile->head; now != NULL; now = now->next)
		Op((CPString*)(now->data));
}

/*
包装过的指针
*/

typedef struct CPPointer
{
	void* block;
	void (*DestoryFunction)(void*);
}CPPointer;

CPPointer* SmartPointerInit(void* Memory, void (*DestoryFunction)(void*))
{
	CPPointer* Pt = (CPPointer*)malloc(sizeof(CPPointer));

	Pt->block = Memory;
	Pt->DestoryFunction =
		(DestoryFunction == NULL) ? free : DestoryFunction;

	return Pt;
}

void* SmartPointerGet(CPPointer* SmartPt)
{
	return SmartPt->block;
}

void SmartPointerDestory(CPPointer* SmartPt)
{
	SmartPt->DestoryFunction(SmartPt->block);
}

/*
GC 垃圾回收器
负责在程序结束时自动释放被添加的内容
*/

inline void GCInit()
{
	GC.vector = FunctionVector.Init(sizeof(CPPointer), CPCallBackType(FunctionSmartPointer.Destory));
	atexit(GCDestory);
}

inline void GCAdd(void(*DestoryFunction)(void*), void* pointer)
{
	if (GC.vector == NULL)
		FunctionGC.Init();

	CPPointer* NewPt = FunctionSmartPointer.Init(pointer, DestoryFunction);
	FunctionVector.PushBack(NewPt, GC.vector);
}

inline void GCDestory()
{
	FunctionVector.Destory(GC.vector);
}