#ifndef DATAARRCLASS_H
#define DATAARRCLASS_H

typedef struct dataArr {
	void** data;
	size_t size;
	size_t capacity;
	void (*addToDataArr)(struct dataArr*, void* data, size_t dataSize);
	void* (*getByIndex)(struct dataArr*, size_t);
} dataArr;

dataArr* dataArr_new();
void addToDataArr(dataArr*, void*, size_t);
void dataArr_delete(dataArr* arr);

#endif
