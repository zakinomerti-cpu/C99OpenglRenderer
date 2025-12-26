#include <stdio.h>
#include <stdlib.h>
#include "dataArray.h"

void addToDataArr(dataArr* arr, void* data) {
	if (!arr) return;
	if (arr->size >= arr->capacity) {
		arr->capacity = (arr->capacity == 0) ? 2 : arr->capacity * 2;
		void** tmp = realloc(arr->data, sizeof(void*) * arr->capacity);
		if (!tmp) return;
		arr->data = tmp;
	}

	arr->data[arr->size] = data;
	arr->size += 1;
}

void* getByIndex(dataArr* arr, size_t index) {
	if (!arr) return NULL;
	if (index >= arr->size) return NULL;
	return arr->data[index];
}

dataArr* dataArr_new() {
	dataArr* arr = (dataArr*)malloc(sizeof(dataArr));
	if (!arr) return NULL;
	arr->capacity = 0;
	arr->size = 0;
	arr->data = NULL;
	arr->addToDataArr = addToDataArr;
	arr->getByIndex = getByIndex;
	return arr;
}

void dataArr_delete(dataArr* arr) {
	if (!arr) return;
	if (arr->size == 0 || arr->capacity == 0) {
		arr->addToDataArr = NULL;
		arr->getByIndex = NULL;
		free(arr);
		arr = NULL;
		return;
	}

	arr->addToDataArr = NULL;
	arr->getByIndex = NULL;
	free(arr->data);
	free(arr);
	arr = NULL;
	return;
}