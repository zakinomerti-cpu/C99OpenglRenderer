#include "MeshObjects.h"
#include <stdlib.h>
#include "GL/glew.h"

//КРИТИЧНО!!! формат 3, 3, 2
float cubeVertices[] = {
	// Front (+Z)     x,y,z, nx,ny,nz, u,v
	-0.5f,-0.5f, 0.5f,  0,0,1,  0.0f,0.0f,
	 0.5f,-0.5f, 0.5f,  0,0,1,  1.0f,0.0f,
	 0.5f, 0.5f, 0.5f,  0,0,1,  1.0f,1.0f,
	-0.5f, 0.5f, 0.5f,  0,0,1,  0.0f,1.0f,

	// Back  (-Z)     зеркальные UV!
	-0.5f,-0.5f,-0.5f,  0,0,-1, 1.0f,0.0f,
	-0.5f, 0.5f,-0.5f,  0,0,-1, 1.0f,1.0f,
	 0.5f, 0.5f,-0.5f,  0,0,-1,  0.0f,1.0f,
	 0.5f,-0.5f,-0.5f,  0,0,-1,  0.0f,0.0f,

	 // Right (+X)
	 0.5f,-0.5f,-0.5f,  1,0,0,  0.0f,0.0f,
	 0.5f, 0.5f,-0.5f,  1,0,0,  1.0f,0.0f,
	 0.5f, 0.5f, 0.5f,  1,0,0,  1.0f,1.0f,
	 0.5f,-0.5f, 0.5f,  1,0,0,  0.0f,1.0f,

	 // Left  (-X)     зеркальные!
	-0.5f,-0.5f,-0.5f, -1,0,0,  1.0f,0.0f,
	-0.5f,-0.5f, 0.5f, -1,0,0,  0.0f,0.0f,
	-0.5f, 0.5f, 0.5f, -1,0,0,  0.0f,1.0f,
	-0.5f, 0.5f,-0.5f, -1,0,0,  1.0f,1.0f,

	// Top   (+Y)
	-0.5f, 0.5f,-0.5f,  0,1,0,  0.0f,0.0f,
	 0.5f, 0.5f,-0.5f,  0,1,0,  1.0f,0.0f,
	 0.5f, 0.5f, 0.5f,  0,1,0,  1.0f,1.0f,
	-0.5f, 0.5f, 0.5f,  0,1,0,  0.0f,1.0f,

	// Bottom (-Y)    зеркальные!
	-0.5f,-0.5f,-0.5f,  0,-1,0,  0.0f,1.0f,
	-0.5f,-0.5f, 0.5f,  0,-1,0,  0.0f,0.0f,
	 0.5f,-0.5f, 0.5f,  0,-1,0,  1.0f,0.0f,
	 0.5f,-0.5f,-0.5f,  0,-1,0,  1.0f,1.0f
};

GLubyte cubeIndices[] = {
	// Front face (вершины 0-3)
	0, 1, 2,  0, 2, 3,

	// Back face (вершины 4-7)  
	4, 6, 5,  4, 7, 6,

	// Left face (вершины 8-11)
	8, 10, 9,  8, 11, 10,

	// Right face (вершины 12-15)
	12, 13, 14,  12, 14, 15,

	// Top face (вершины 16-19)
	16, 17, 18,  16, 18, 19,

	// Bottom face (вершины 20-23)
	20, 22, 21,  20, 23, 22
};

//КРИТИЧНО!!! формат 3, 3, 2
float planeVertices[] = {
	// Front (+Z)     x,y,z, nx,ny,nz, u,v
	-0.5f,-0.5f, 0.5f,  0,0,1,  0.0f,0.0f,
	 0.5f,-0.5f, 0.5f,  0,0,1,  1.0f,0.0f,
	 0.5f, 0.5f, 0.5f,  0,0,1,  1.0f,1.0f,
	-0.5f, 0.5f, 0.5f,  0,0,1,  0.0f,1.0f,
};

GLubyte planeIndices[] = {
	0, 1, 2,  0, 2, 3,
};

Mesh* createPlane() {
	Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
	if (!mesh) return NULL;

	mesh->vertexCount = sizeof(planeVertices) / sizeof(float);
	mesh->vertices = (float*)malloc(mesh->vertexCount * sizeof(float));
	if (!mesh->vertices) { free(mesh); return NULL; }

	memcpy(mesh->vertices, planeVertices, mesh->vertexCount * sizeof(float));

	mesh->indexCount = sizeof(planeIndices) / sizeof(GLubyte);
	mesh->indices = (GLubyte*)malloc(mesh->indexCount * sizeof(GLubyte));
	if (!mesh->indices) { free(mesh->vertices); free(mesh); return NULL; }

	memcpy(mesh->indices, planeIndices, mesh->indexCount * sizeof(GLubyte));
	return mesh;
}

Mesh* createCube() {
	Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
	if (!mesh) return NULL;

	mesh->vertexCount = sizeof(cubeVertices) / sizeof(float);  // 24
	mesh->vertices = (float*)malloc(mesh->vertexCount * sizeof(float));
	if (!mesh->vertices) { free(mesh); return NULL; }

	memcpy(mesh->vertices, cubeVertices, mesh->vertexCount * sizeof(float));

	mesh->indexCount = sizeof(cubeIndices) / sizeof(GLubyte);  // 36
	mesh->indices = (GLubyte*)malloc(mesh->indexCount * sizeof(GLubyte));
	if (!mesh->indices) { free(mesh->vertices); free(mesh); return NULL; }

	memcpy(mesh->indices, cubeIndices, mesh->indexCount * sizeof(GLubyte));
	return mesh;
}

void Mesh_delete(Mesh* mesh) {
	free(mesh->vertices);
	free(mesh->indices);
	mesh->vertices = NULL;
	mesh->indices = NULL;
	free(mesh);

	mesh = NULL;

}

