#include "Mesh.h"
#include <stdlib.h>
#include "GL/glew.h"

void InitMeshObject(Mesh* mesh) {
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ibo);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER,
		mesh->vertexCount * sizeof(float),
		mesh->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mesh->indexCount * sizeof(GLubyte),
		mesh->indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void setVerticesMeshObject(Mesh* mesh, float* v, int vc) {
	mesh->vertices = v;
	mesh->vertexCount = vc;
}
void setIndicesMeshObject(Mesh* mesh, unsigned char* i, int ic) {
	mesh->indices = i;
	mesh->indexCount = ic;
}

void BindMeshObject(Mesh* mesh) {
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
}

void UnBindMeshObject(Mesh* mesh) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh* Mesh_new() {
	Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
	if (!mesh) return NULL;

	mesh->vertices = NULL;
	mesh->indices = NULL;

	mesh->init = InitMeshObject;
	mesh->setVertices = setVerticesMeshObject;
	mesh->setIndices = setIndicesMeshObject;
	mesh->bind = BindMeshObject;
	mesh->unBind = UnBindMeshObject;

	mesh->vertexCount = 0;
	mesh->indexCount = 0;
	mesh->isInit = 0;

	mesh->vbo = 0;
	mesh->ibo = 0;
	return mesh;
}

