#include "HashArray.h"
#include <stdlib.h>
#include "dataArray.h"
#include <string.h>

long HashArrToolSimpleHash(const char* str) {
    long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = hash * 33 + c;
    }

    return hash;
}

dataArr* HashArrToolGetInnerArray(dataArr* arr, int index) {
    dataArr* tmp = arr->getByIndex(arr, index);
    if (!tmp) {
        tmp = dataArr_new();
        arr->data[index] = tmp;
    }
    return tmp;
}


void HashArrToolAddObject(HashArray* arr, void* data, const char* name) {
    if (data == NULL) return;
    size_t hashIndex = HashArrToolSimpleHash(name) % arr->elementCount;
    dataArr* innerArr = HashArrToolGetInnerArray(arr->Data, hashIndex);
    HashArrayElement* element = (HashArrayElement*)malloc(
        sizeof(HashArrayElement)); if (!element) return;

    element->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    if (!element->name) return;

    element->data = data;
    memcpy(element->name, name, strlen(name) + 1);

    innerArr->addToDataArr(innerArr, element);
}

void* HashArrToolGetObject(HashArray* arr, const char* name) {
    size_t hashIndex = HashArrToolSimpleHash(name) % arr->elementCount;
    dataArr* innerArr = HashArrToolGetInnerArray(arr, hashIndex);
    for (size_t iter = 0; iter < innerArr->size; iter++) {
        HashArrayElement* temp = 
            (HashArrayElement*)innerArr->getByIndex(innerArr, iter);
        if (strcmp(temp->name, name) == 0) {
            return temp->data;
        }
    }

    return NULL;
}

HashArray* HashArray_new(int size)
{
    HashArray* arr = (HashArray*)malloc(sizeof(HashArray));
    if (!arr) return NULL;

    arr->addObject = HashArrToolAddObject;
    arr->getObject = HashArrToolGetObject;
    arr->getInnerArray = HashArrToolGetInnerArray;
    arr->elementCount = size;
    arr->Data = dataArr_new();
    if (!arr->Data) {
        free(arr);
        return NULL;
    }

    for (size_t i = 0; i < size; i += 1) {
        arr->Data->addToDataArr(arr->Data, NULL);
    }

    return arr;

}
