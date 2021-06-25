#pragma once
#include <stdio.h>

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

	struct ListNode* Head = malloc(sizeof(struct ListNode));
	Head->next = Head->data = NULL;

	if (length > 1)
	{
		struct ListNode* last = Head;
		for (int num = 0; num < length; num++)
		{
			struct ListNode* node = malloc(sizeof(struct ListNode));
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

void Destory(void(*DestoryFunction)(), struct ListData* list)
{
	if (DestoryFunction == NULL)
	{
		for (struct ListNode* now = list->head; now == NULL; list->head = now)
		{
			now = list->head->next;
			free(list->head->data);
			free(list->head);
		}
	}
	else
	{
		for (struct ListNode* now = list->head; now == NULL; list->head = now)
		{
			now = list->head->next;
			DestoryFunction(list->head->data);
			free(list->head);
		}
	}
	list->end = list->head = list->length = NULL;
}

inline struct ListNode* FindNode(const int position, struct ListData* list) //查找第position个元素
{
	struct ListNode* pos = list->head;
	for (int num = 0; num < position; num++)
		pos = pos->next;
	return pos;
}

void NodeInsertA(const int position, void* data, struct ListData* list)  //插入到position前面
{
	struct ListNode* node = malloc(sizeof(struct ListNode));
	node->data = data;

	if (position == 0)
	{
		node->next = list->head;
		list->head = node;
	}
	else
	{
		struct ListNode* pos = FindNode(position - 1, list);
		node->next = pos->next;
		pos->next = node;
	}
	list->length += 1;
}

void NodeInsertB(const int position, void* data,struct ListData* list)  //插入到position后面
{
	struct ListNode* pos = FindNode(position, list);
	struct ListNode* node = malloc(sizeof(struct ListNode));

	node->data = data;
	node->next = pos->next;
	pos->next = node;

	if (position == list->length - 1)
		list->end = node;
	list->length += 1;
}

void NodeDelete(const int position, void(*DestoryFunction)(), struct ListData* list) //删除第position个元素
{
	struct ListNode* Deleted;
	if (position == 0)
	{
		Deleted = list->head;
		list->head = list->head->next;
	}
	else
	{
		struct ListNode* pos = FindNode(position - 1, list);
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

void ListDelete(const int from, const int to, void(*DestoryFunction)(), struct ListData* list) //删除[from,to]区间内的链表
{
	struct ListNode* Deleted, * To = FindNode(to, list);
	if (from == 0)
	{
		Deleted = list->head;
		list->head = To->next;
	}
	else
	{
		struct ListNode* From = FindNode(from - 1, list);
		Deleted = From->next;
		From->next = To->next;

		if (to == list->length - 1)
			list->end = From;
	}

	Destory(DestoryFunction,list);
	list->length -= to - from + 1;
}

static struct
{
	struct ListNode* (*ListInit)(const int length, struct ListData* list);
	struct ListNode* (*FindNode)(const int position, struct ListData* list);
	void (*InsertA)(const int position, struct ListData* list);
	void (*InsertB)(const int position, void* data, struct ListData* list);
	void (*DeleteNode)(const int position, void(*DestoryFunction)(), struct ListData* list);
	void (*DeleteList)(const int from, const int to, void(*DestoryFunction)(), struct ListData* list);
}FunctionList = { &ListInit,&FindNode,&NodeInsertA,&NodeInsertB,&NodeDelete,&ListDelete };