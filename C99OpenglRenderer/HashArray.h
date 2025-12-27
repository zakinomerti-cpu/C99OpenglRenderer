#ifndef HASHARRAYCLASS_H
#define HASHARRAYCALSS_H

long HASHARRAYCLASS_HASHFUNC_BEACH(const char* str) {
	long hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = hash * 33 + c;
	}

	return hash;
}

typedef struct HashArray {


} HashArray;

#endif
