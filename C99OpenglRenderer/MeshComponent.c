#include "Component.h"
#include "dataArray.h"
#include "Mesh.h"
#include <stdlib.h>
#include "GL/glew.h"

void ComInit(Component* cmp) {
	dataArr* data = cmp->LocData;
	GLuint* vbo = (GLuint*)malloc(sizeof(GLuint));
	GLuint* ibo = (GLuint*)malloc(sizeof(GLuint));
	if (!vbo || !ibo) return;
	data->addToDataArr(data, vbo, sizeof(GLuint));
	data->addToDataArr(data, ibo, sizeof(GLuint));

	glGenBuffers(1,	vbo);
	glGenBuffers(1, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, (*vbo));
	glBufferData(GL_ARRAY_BUFFER,
		((Mesh*)cmp->InData)->vertexCount * sizeof(float),
		((Mesh*)cmp->InData)->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*ibo));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		((Mesh*)cmp->InData)->indexCount * sizeof(GLubyte),
		((Mesh*)cmp->InData)->indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void bind(Component* cmp) {
	dataArr* data = cmp->LocData;
	GLuint* vbo = data->getByIndex(data, 0);
	GLuint* ibo = data->getByIndex(data, 1);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbo));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*ibo));
}

void unBind(Component* cmp) {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void addChild(Component* cmp1, Component* cmp2) { /* тут не реализовано*/ }
void removeChild(Component* cmp1, Component* cmp2) { /* тут не реализовано*/ }

// только вне потока opengl, если знаешь
// что компонент точно пойдет на удаление
void DeleteComponent(Component* cmp) {
	cmp->Init = NULL;
	cmp->Bind = NULL;
	cmp->UnBind = NULL;
	cmp->AddChild = NULL;
	cmp->RemoveChild = NULL;
	cmp->DeleteComponent = NULL;

	glDeleteBuffers(1, cmp->LocData->getByIndex(cmp->LocData, 0));
	glDeleteBuffers(1, cmp->LocData->getByIndex(cmp->LocData, 1));
	free(cmp->LocData->getByIndex(cmp->LocData, 0));
	free(cmp->LocData->getByIndex(cmp->LocData, 1));

	dataArr_delete(cmp->child);
	dataArr_delete(cmp->LocData);
	free(cmp);
}

//vbo = LocData[0]
//ibo = LocData[1]
Component* MeshComponent_new(Component* prnt, Entity* ent, Mesh* mesh) {
	Component* cmp = (Component*)malloc(sizeof(Component));
	if (!cmp) return NULL;

	cmp->Init = ComInit;
	cmp->Bind = bind;
	cmp->UnBind = unBind;
	cmp->AddChild = addChild;
	cmp->RemoveChild = removeChild;
	cmp->DeleteComponent = DeleteComponent;

	cmp->parentCmp = prnt;
	cmp->parentEntity = ent;

	cmp->InData = mesh;
	cmp->child = dataArr_new();
	if (!cmp->child) {
		free(cmp);
		return NULL;
	}
	cmp->LocData = dataArr_new();
	if (!cmp->LocData) {
		free(cmp);
		return NULL;
	}

	return cmp;
}