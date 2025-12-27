#include "HashArray.h"
#include <stdlib.h>
#include "dataArray.h"

long HashArrToolSimpleHash(const char* str) {
    long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = hash * 33 + c;
    }

    return hash;
}

void HashArrToolAddObject(HashArray* arr, void* data, const char* name) {
    if (data == NULL) return;
    size_t hashIndex = HashArrToolSimpleHash(name) % arr->elementCount;
    dataArr* innerArr = HashArrToolGetInnerArray(arr->Data, hashIndex);
    innerArr->addToDataArr(innerArr, data);
}

void* HashArrToolGetEntity(HashArray* arr, const char* name) {
    size_t hashIndex = HashArrToolSimpleHash(name) % arr->elementCount;
    dataArr* innerArr = HashArrToolGetInnerArray(arr, hashIndex);
    for (size_t iter = 0; iter < innerArr->size; iter++) {
        void* temp = innerArr->getByIndex(innerArr, iter);
        //if (strcmp(temp_ent->entityName, name) == 0) {
        //    return temp_ent;
        //
        int a;
    }

    return NULL;
}

dataArr* HashArrToolGetInnerArray(HashArray* arr, int index) {
    dataArr* tmp = (dataArr*)arr->Data->getByIndex(arr->Data, index);
    if (!tmp) {
        tmp = dataArr_new();
        arr->Data->data[index] = tmp;
    }
    return tmp;
}

HashArray* HashArray_new(int size)
{
    HashArray* arr = (HashArray*)malloc(sizeof(HashArray));
    if (!arr) return NULL;

    arr->addObject = HashArrToolAddObject;
    arr->getEntity = HashArrToolGetEntity;
    arr->getInnerArray = HashArrToolGetInnerArray;
    arr->elementCount = size;
    arr->Data = dataArr_new();
    if (!arr->Data) {
        free(arr);
        return NULL;
    }

    for (size_t i = 0; i < size; i += 1) {
        arr->Data->addToDataArr(arr->Data,
            NULL);
    }

    return arr;

}
