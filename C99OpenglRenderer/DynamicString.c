#include "DynamicString.h"
#include <stdlib.h>

void DstrObjAddSymbol(dstr* str, char c) {
    if (str->size >= str->capacity) {
        str->capacity = (str->capacity == 0) ? 4 : str->capacity * 2;
        char* tmp = (char*)realloc(str->data, str->capacity);
        if (!tmp) return;
        str->data = tmp;
    }

    str->data[str->size] = c;
    str->size += 1;
}
char* DstrObjGetString(dstr* str) {
    return str->data;
}
dstr* dstr_new() {
    dstr* str = (dstr*)malloc(sizeof(dstr));
    if (!str) return NULL;

    str->size = 0;
    str->capacity = 0;
    str->data = NULL;

    str->addSymbol = DstrObjAddSymbol;
    str->getString = DstrObjGetString;
    return str;

}
