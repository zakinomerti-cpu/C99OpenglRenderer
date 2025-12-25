#ifndef MESHCLASS_H
#define MESHCLASS_H

typedef struct Mesh {
	int vertexCount;
	int indexCount;

	float* indices;
	float* vertices;
} Mesh;

void Mesh_delete(Mesh*);

#endif