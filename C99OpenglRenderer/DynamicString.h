#ifndef DSTRINGCLASS_H
#define DSTRINGCLASS_H

typedef struct dstr {
	int size;
	int capacity;
	char* data;

	void (*addSymbol)(struct dstr*, char c);
	char* (*getString)(struct dstr*);
} dstr;

dstr* dstr_new();

#endif