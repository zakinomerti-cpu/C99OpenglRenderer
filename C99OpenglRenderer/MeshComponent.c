#include "Component.h"
#include "dataArray.h"
#include "Mesh.h"
#include <stdlib.h>
#include "GL/glew.h"


//спецификация компонента таковы, что
//LocData[0] это vbo, LocData[1] это ibo
void ComInit(Component* cmp) {
	dataArr* data = cmp->LocData;
	GLuint* vbo = (GLuint*)malloc(sizeof(GLuint));
	GLuint* ibo = (GLuint*)malloc(sizeof(GLuint));
	if (!vbo || !ibo) return;
	data->addToDataArr(data, vbo);
	data->addToDataArr(data, ibo);

	dataArr* inDt = cmp->InData;

	glGenBuffers(1,	vbo);
	glGenBuffers(1, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, (*vbo));
	glBufferData(GL_ARRAY_BUFFER,
		((Mesh*)inDt->getByIndex(inDt, 0))->vertexCount * sizeof(float),
		((Mesh*)inDt->getByIndex(inDt, 0))->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*ibo));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		((Mesh*)inDt->getByIndex(inDt, 0))->indexCount * sizeof(GLubyte),
		((Mesh*)inDt->getByIndex(inDt, 0))->indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	cmp->isReady = 1;
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
void DeleteMeshComponent(Component* cmp) {
	cmp->Init = NULL;
	cmp->Bind = NULL;
	cmp->UnBind = NULL;
	cmp->DeleteComponent = NULL;

	glDeleteBuffers(1, cmp->LocData->getByIndex(cmp->LocData, 0));
	glDeleteBuffers(1, cmp->LocData->getByIndex(cmp->LocData, 1));
	free(cmp->LocData->getByIndex(cmp->LocData, 0));
	free(cmp->LocData->getByIndex(cmp->LocData, 1));
	dataArr_delete(cmp->LocData);
	free(cmp);
}

//vbo = LocData[0]
//ibo = LocData[1]
//Mesh* = InData[0]
Component* MeshComponent_new(Component* prnt, Entity* ent, dataArr* InData) {
	Component* cmp = (Component*)malloc(sizeof(Component));
	if (!cmp) return NULL;

	cmp->Init = ComInit;
	cmp->Bind = bind;
	cmp->UnBind = unBind;
	cmp->DeleteComponent = DeleteMeshComponent;
	cmp->parentEntity = ent;

	cmp->InData = InData;
	cmp->LocData = dataArr_new();
	if (!cmp->LocData) {
		free(cmp);
		return NULL;
	}

	cmp->child = NULL;
	cmp->isReady = 0;
	cmp->chdCount = -1;

	return cmp;
}