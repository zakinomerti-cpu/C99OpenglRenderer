#ifndef MESHCLASS_H
#define MESHCLASS_H

//mesh datatype
typedef struct Mesh {
	void (*init)(struct Mesh*);
	void (*setVertices)(struct Mesh*, float*, int);
	void (*setIndices)(struct Mesh*, unsigned char*, int);
	void (*bind)(struct Mesh*);
	void (*unBind)(struct Mesh*);

	//private block
	int vertexCount;
	int indexCount;
	int isInit;
	unsigned char* indices;
	float* vertices;

	unsigned int vbo;
	unsigned int ibo;

} Mesh;

Mesh* Mesh_new();
void Mesh_delete(Mesh*);

#endif