#ifndef MESHCLASS_H
#define MESHCLASS_H

typedef struct Mesh {
	char* meshName;
	void (*setVertices)(struct Mesh*, float*, int);
	void (*setIndices)(struct Mesh*, unsigned char*, int);
	void (*meshBind)(struct Mesh*);
	void (*unBindMesh)(struct Mesh*);
	void (*meshInit)(struct Mesh*);

	int vertexCount;
	int indexCount;
	unsigned char* indices;
	float* vertices;

	unsigned int vbo;
	unsigned int ibo;
	char isReady;

} Mesh;

Mesh* Mesh_new(char* meshName);
void Mesh_delete(Mesh*);

#endif