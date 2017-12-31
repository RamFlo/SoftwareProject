#include "SPArrayList.h"
#include <stdlib.h>
#include <stdio.h>

SPArrayList* spArrayListCreate(int maxSize) {
	int realSize = maxSize * sizeof(int);
	int* elements = (int *)malloc(realSize);
	if (maxSize <= 0)
		return NULL;
	if (!elements) {
		printf("Error: malloc has failed\n");
		return NULL;
	}
	SPArrayList* p;
	p = (SPArrayList *)malloc(sizeof(SPArrayList));
	if (!p) {
		free(elements);
		printf("Error: malloc has failed\n");
		return NULL;
	}
	p->actualSize = 0;
	p->elements = elements;
	p->maxSize = maxSize;
	return p;
}

SPArrayList* spArrayListCopy(SPArrayList* src) {
	int i = 0, *curNewPointer, *curOldPointer;
	SPArrayList* p;
	if (src == NULL)
		return NULL;
	p = spArrayListCreate(src->maxSize);
	if (p == NULL)
		return NULL;
	p->actualSize = src->actualSize;
	curNewPointer = p->elements;
	curOldPointer = src->elements;
	for (i = 0; i < src->maxSize; i++) {
		*curNewPointer = *curOldPointer;
		curNewPointer++;
		curOldPointer++;
	}
	return p;
}

void spArrayListDestroy(SPArrayList* src) {
	if (src != NULL) {
		free(src->elements);
		free(src);
	}
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src) {
	if (src == NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index) {
	int i = 0, *curArrayLoc;
	if (src == NULL || index > src->actualSize || index < 0)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize == src->maxSize)
		return SP_ARRAY_LIST_FULL;
	curArrayLoc = src->elements + src->actualSize;
	for (i = 0; i < src->actualSize - index; i++) {
		*curArrayLoc = *(curArrayLoc - 1);
		curArrayLoc--;
	}
	*curArrayLoc = elem;
	src->actualSize++;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem) {
	return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem) {
	return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index) {
	int numIter, i = 0, *curAddress, *nextAddress;
	if (src == NULL || index >= src->actualSize || index < 0)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize == 0)
		return SP_ARRAY_LIST_EMPTY;
	numIter = (src->actualSize) - index;
	curAddress = src->elements;
	nextAddress = src->elements;
	nextAddress++;
	curAddress += index;
	nextAddress += index;
	for (i = 0; i < numIter-1; i++) {
		*curAddress = *nextAddress;
		curAddress++;
		nextAddress++;
	}
	src->actualSize = (src->actualSize) - 1;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src) {
	return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src) {
	return spArrayListRemoveAt(src, src->actualSize - 1);
}

int spArrayListGetAt(SPArrayList* src, int index) {
	if (src == NULL || index >= src->actualSize)
		return -1;
	return *(src->elements + index);
}

int spArrayListGetFirst(SPArrayList* src) {
	return spArrayListGetAt(src, 0);
}

int spArrayListGetLast(SPArrayList* src) {
	return spArrayListGetAt(src, (src->actualSize)-1);
}

int spArrayListMaxCapacity(SPArrayList* src) {
	if (src == NULL)
		return -1;
	return src->maxSize;
}

int spArrayListSize(SPArrayList* src) {
	if (src != NULL)
		return src->actualSize;
	return -1;
}

bool spArrayListIsFull(SPArrayList* src) {
	if (src != NULL)
		return src->actualSize == src->maxSize;
	return false;
}

bool spArrayListIsEmpty(SPArrayList* src) {
	if (src != NULL && src->actualSize == 0)
		return true;
	return false;
}