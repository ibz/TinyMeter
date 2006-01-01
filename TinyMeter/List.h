#ifndef __LIST_H__
#define __LIST_H__

typedef struct List_
{
	void* Car;
	struct List_* Cdr;
} List;

List* ListCreate();
void ListAppend(List** self, List* list);
int ListCount(List* self);
void ListDestroy(List** self);

#endif // #ifndef __LIST_H__