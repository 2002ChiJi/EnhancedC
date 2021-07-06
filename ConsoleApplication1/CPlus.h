#pragma once
#include <string.h>
#include <stdlib.h>

/*
List
*/

struct ListNode
{
	struct ListNode* next;
	void* data;
};

struct ListData
{
	struct ListNode *head, *end;
	int length;
};

void ListInit(const int length,struct ListData* list)
{		
	if (length < 1) return;

	struct ListNode* Head = (struct ListNode*)malloc(sizeof(struct ListNode));
	Head->next = NULL;
	Head->data = NULL;

	if (length > 1)
	{
		struct ListNode* last = Head;
		for (int num = 0; num < length; num++)
		{
			struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
			node->data = NULL;

			last->next = node;
			last = node;
		}
		last->next = NULL;

		{
			list->end = last;
			list->head = Head;
			list->length = length;
		}
	}
}

inline void ListDestory(void(*DestoryFunction)(void*),struct ListNode* node)
{
	if (DestoryFunction == NULL)
	{
		for (struct ListNode* now = node; now == NULL; node = now)
		{
			now = node->next;
			free(node->data);
			free(node);
		}
	}
	else
	{
		for (struct ListNode* now = node; now == NULL; node = now)
		{
			now = node->next;
			DestoryFunction(node->data);
			free(node);
		}
	}
}

inline struct ListNode* ListFindNode(const int position, struct ListData* list) //查找第position个元素
{
	struct ListNode* pos = list->head;
	for (int num = 0; num < position; num++)
		pos = pos->next;
	return pos;
}

void ListNodeInsertA(const int position, void* data, struct ListData* list)  //插入到position前面
{
	struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
	node->data = data;

	if (position == 0)
	{
		node->next = list->head;
		list->head = node;
	}
	else
	{
		struct ListNode* pos = ListFindNode(pos - 1, list);
		node->next = pos->next;
		pos->next = node;
	}
	list->length += 1;
}

void ListNodeInsertB(const int position, void* data,struct ListData* list)  //插入到position后面
{
	struct ListNode* pos = ListFindNode(position, list);
	struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));

	node->data = data;
	node->next = pos->next;
	pos->next = node;

	if (position == list->length - 1)
		list->end = node;
	list->length += 1;
}

void ListNodeDelete(const int position, void(*DestoryFunction)(void*), struct ListData* list) //删除第position个元素
{
	struct ListNode* Deleted;
	if (position == 0)
	{
		Deleted = list->head;
		list->head = list->head->next;
	}
	else
	{
		struct ListNode* pos = ListFindNode(position - 1, list);
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

void ListDelete(const int from, const int to, void(*DestoryFunction)(void*), struct ListData* list) //删除[from,to]区间内的链表
{
	struct ListNode* Deleted, * To = ListFindNode(to, list);
	if (from == 0)
	{
		Deleted = list->head;
		list->head = To->next;
	}
	else
	{
		struct ListNode* From = ListFindNode(from - 1, list);
		Deleted = From->next;
		From->next = To->next;

		if (to == list->length - 1)
			list->end = From;
	}

	ListDestory(DestoryFunction, Deleted);
	list->length -= to - from + 1;
}

static struct
{
	void (*ListInit)(const int length, struct ListData* list);
	void (*Destory)(void(*DestoryFunction)(void*), struct ListNode* node);
	struct ListNode* (*FindNode)(const int position, struct ListData* list);
	void (*InsertA)(const int position, void* data, struct ListData* list);
	void (*InsertB)(const int position, void* data, struct ListData* list);
	void (*DeleteNode)(const int position, void(*DestoryFunction)(void*), struct ListData* list);
	void (*DeleteList)(const int from, const int to, void(*DestoryFunction)(void*), struct ListData* list);
}FunctionList = { &ListInit,&ListDestory,&ListFindNode,&ListNodeInsertA,&ListNodeInsertB,&ListNodeDelete,&ListDelete };

/*
Vector
在使用的时候务必清楚自己要往里面存的变量类型
在用该容器管理指针的时候务必处理好数据所有权的问题
*/

struct VectorData
{
	unsigned char ElementSize;
	unsigned int Length, used;
	void* VectorHead;
};

void VectorInit(unsigned char ElenmentSize,struct VectorData* vector)
{
	vector->ElementSize = ElenmentSize;
	vector->VectorHead = malloc(0);
	vector->Length = 0;
	vector->used = 0;
}

void VectorResize(unsigned int newLength, struct VectorData* vector)
{
	void* temp = realloc(vector->VectorHead, newLength * vector->ElementSize);
	if (temp == NULL) abort();
		
	vector->VectorHead = temp;
	vector->Length = newLength;
	vector->used = 
		(vector->used > newLength ? newLength : vector->used);
}

void VectorPushBack(void* Element, struct VectorData* vector)
{
	if (vector->used == vector->Length)
		VectorResize((vector->Length + 1) * 2, vector);

	void* pt = ((char*)vector->VectorHead) + (vector->used * vector->ElementSize);
	memmove(pt, Element, vector->ElementSize);

	vector->used++;
}

void VectorInsertA(unsigned int position, const void* Element, struct VectorData* vector)
{
	if (vector->used == vector->Length)
		VectorResize((vector->Length) * 2, vector);

	int size = vector->ElementSize;
	char* head = (char*)vector->VectorHead;
	memmove((head + (position + 1) * size), (head + position * size), (vector->used - position) * size);
	memmove((head + position * size), Element, size);

	vector->used++;
}

void VectorInsertB(unsigned int position, const void* Element, struct VectorData* vector)
{
	if (vector->used == vector->Length)
		VectorResize((vector->Length) * 2, vector);

	int size = vector->ElementSize;
	char* head = (char*)vector->VectorHead;
	memmove((head + (position + 2) * size), (head + (position + 1) * size), (vector->used - position) * size);
	memmove((head + (position + 1) * size), Element, size);

	vector->used++;
}

void VectorRemove(unsigned int position, struct VectorData* vector)
{
	int size = vector->ElementSize;
	char* head = (char*)vector->VectorHead;
	memcpy((head + (position)*size), (head + (position + 1) * size), (vector->used - position + 1) * size);

	vector->used--;
}

void VectorDestory(struct VectorData* vector)
{
	free(vector->VectorHead);

	vector->VectorHead = NULL;
	vector->ElementSize = vector->Length = vector->used=0;
}

inline void VectorEmpty(struct VectorData* vector)
{
	vector->used = 0;
}

inline const void* VectorAt(unsigned int position, struct VectorData* vector)
{
	return ((char*)vector->VectorHead) + (position * vector->ElementSize);
}

inline void VectorChange(unsigned int position, void* Element, struct VectorData* vector)
{
	void* pt = ((char*)vector->VectorHead) + (position * vector->ElementSize);
	memmove(pt, Element, vector->ElementSize);
}

struct 
{
	void (*Init)(unsigned char ElenmentSize, struct VectorData* vector);
	void (*Resize)(unsigned int newLength, struct VectorData* vector);
	void (*PushBack)(void* Element, struct VectorData* vector);
	void (*InsertA)(unsigned int position, const void* Element, struct VectorData* vector);
	void (*InsertB)(unsigned int position, const void* Element, struct VectorData* vector);
	void (*Remove)(unsigned int position, struct VectorData* vector);
	void (*Destory)(struct VectorData* vector);
	void (*Empty)(struct VectorData* vector);
	const void* (*At)(unsigned int position, struct VectorData* vector);
	void (*Change)(unsigned int position, void* Element, struct VectorData* vector);
}FunctionVector = { &VectorInit,&VectorResize,&VectorPushBack,&VectorInsertA,&VectorInsertB,&VectorRemove,&VectorDestory,&VectorEmpty,&VectorAt,&VectorChange};

/*
CPString
底层采用Vector
*/

struct CPStrData
{
	struct VectorData data;
};

void CPStrInit(const char* str,struct CPStrData* CPStr)
{
	FunctionVector.Init(sizeof(char), &(CPStr->data));
	if (str != NULL)
	{
		FunctionVector.Resize(strlen(str), &(CPStr->data));
		strcpy(CPStr->data.VectorHead, str);
	}
}

inline void CPStrDestory(struct CPStrData* CPStr)
{
	FunctionVector.Destory(&(CPStr->data));
}

inline void CPStrEmpty(struct CPStrData* CPStr)
{
	FunctionVector.Empty(&(CPStr->data));
}

inline const char* CPStrAt(unsigned int position,struct CPStrData* CPStr)
{
	return FunctionVector.At(position, &(CPStr->data));
}

inline void CPStrChange(unsigned int position,const  char* Element, struct CPStrData* CPStr)
{
	for (int num = 0, len = strlen(Element); num < len; num++)
		FunctionVector.Change(position + num, Element + num, CPStr);
}

inline void CPStrRemove(unsigned int position, struct CPStrData* CPStr)
{
	FunctionVector.Remove(position,&(CPStr->data));
}

inline CPStrPushBack(char* Element, struct CPStrData* CPStr)
{
	for (int num = 0, len = strlen(Element); num < len; num++)
		FunctionVector.PushBack(Element + num, &(CPStr->data));
}

inline CPStrInsertA(unsigned int position, const char* Element, struct CPStrData* CPStr)
{
	for (int len = strlen(Element) - 1; len >= 0; len--)
		FunctionVector.InsertA(position, Element + len, &(CPStr->data));
}

inline CPStrInsertB(unsigned int position, const char* Element, struct CPStrData* CPStr)
{
	for (int len = strlen(Element) - 1; len >= 0; len--)
		FunctionVector.InsertB(position, Element + len, &(CPStr->data));
}

inline char* CPStrc_str(struct CPStrData* CPStr)
{
	return CPStr->data.VectorHead;
}

inline void CPStrShorten(unsigned int newLength, struct CPStrData* CPStr)
{
	if (newLength >= CPStr->data.used)abort();

	FunctionVector.Resize(newLength, CPStr);
	FunctionVector.PushBack('\0', &(CPStr->data));
}

inline void CPStrCpy(const char* str, struct CPStrData* CPStr)		//复制后原CPStr的字符串会被清空
{
	FunctionVector.Destory(CPStr);
	CPStrInit(str,CPStr);
}

void CPStrCat(const char* str, struct CPStrData* CPStr)
{
	if (CPStr->data.Length < CPStr->data.used + strlen(str) - 1)
		FunctionVector.Resize(CPStr->data.used + strlen(str) - 1, &(CPStr->data));

	char* head = CPStr->data.VectorHead;
	memcpy(head + CPStr->data.used - 1, str, strlen(str));
	CPStr->data.used += (strlen(str) - 1);
}

inline const char* CPStrFind(const unsigned start,const char* str, struct CPStrData* CPStr)
{
	return strstr((char*)(CPStr->data.VectorHead) + start, str);
}

inline unsigned CPStrCount(const char* str, struct CPStrData* CPStr)
{
	int start = 0, count = 0;
	for (; strstr((char*)(CPStr->data.VectorHead) + start, str); count++);
	return count;
}

inline void lower(struct CPStrData* CPStr)
{
	char* head = CPStr->data.VectorHead;
	for (; (*head) != '\0'; head++)
		if ((*head) >= 65 && (*head) <= 90) (*head) += 32;
}

inline upper(struct CPStrData* CPStr)
{
	char* head = CPStr->data.VectorHead;
	for (; (*head) != '\0'; head++)
		if ((*head) >= 97 && (*head) <= 122) (*head) -= 32;
}

void CPStrRelpace(const char* from, const char* to, struct CPStrData* CPStr)
{
	struct CPStrData buffer;
	int lenFrom = strlen(from);
	char* head = CPStr->data.VectorHead;

	for (int num = 0; num < CPStr->data.used; num++)
	{
		if (!strncmp(head + num, from, strlen(from)))
		{
			CPStrCat(to, &buffer);
			num += lenFrom - 1;
		}
		else
			CPStrPushBack(head + num, &buffer);
	}

	CPStrCpy(buffer.data.VectorHead, CPStr);
	CPStrDestory(&buffer);
}

inline int CPStrLen(struct CPStrData* CPStr)
{
	return CPStr->data.used;
}

char* CPStrSplit(const unsigned from, const unsigned to, struct CPStrData* CPStr)
{
	char* ret = malloc(from - to + 1),
		* head = CPStr->data.VectorHead,
		* end = (char*)(CPStr->data.VectorHead) + to,
		* begin = (char*)(CPStr->data.VectorHead) + from;

	do
	{
		ret[begin - head] = *begin;
		begin++;
	} while (begin <= end);
	ret[from - to] = '\0';

	return ret;
}