#include <stdlib.h>
#include "List.h"

List* ListCreate()
{
	List* list = (List*)malloc(sizeof(List));
	list->Car = list->Cdr = 0;

	return list;
}

void ListAppend(List** self, List* list)
{
	if(*self)
	{
		List* p = *self;
		while(p && p->Cdr)
		{
			p = p->Cdr;
		}
		p->Cdr = list;
	}
	else
	{
		*self = list;
	}
}

int ListCount(List* self)
{
	int count = 0;
	List* p = self;
	while(p)
	{
		count++;
		p = p->Cdr;
	}

	return count;
}

void ListDestroy(List** self)
{
	List* p = *self;
	List* q;
	while(p)
	{
		q = p;
		p = p->Cdr;
		free(q);
	}

	*self = 0;
}