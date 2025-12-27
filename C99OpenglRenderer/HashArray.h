#ifndef HASHARRAYCLASS_H
#define HASHARRAYCLASS_H

typedef struct dataArr dataArr;

typedef struct HashArrayElement {
	char* name;
	void* element;
} HashArrayElement;

typedef struct HashArray {
	void (*addObject)(struct HashArray*, void*, const char*);
	void* (*getEntity)(struct HashArray*, const char*);

	dataArr* (*getInnerArray)(struct HashArray*, int);
	int elementCount;
	dataArr* Data;
} HashArray;

HashArray* HashArray_new(int);

#endif