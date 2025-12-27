#include "Component.h"
#include "dataArray.h"
#include "Mesh.h"
#include <stdlib.h>

void ComInit(Component* cmp) {

	dataArr* data = cmp->InData;

	Mesh* mesh = Mesh_new();
	mesh->setVertices(mesh, data->getByIndex(data, 0), data->getByIndex(data, 1));
	mesh->setIndices(mesh, data->getByIndex(data, 2), data->getByIndex(data, 3));
	mesh->init(mesh);
	cmp->LocData->addToDataArr(cmp->LocData, mesh);
	cmp->isReady = 1;
}

void bind(Component* cmp) {
	Mesh* mesh = (Mesh*)cmp->LocData->getByIndex(cmp->LocData, 0);
	mesh->bind(mesh);
}

void unBind(Component* cmp) {
	Mesh* mesh = (Mesh*)cmp->LocData->getByIndex(cmp->LocData, 0);
	mesh->unBind(mesh);
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
	free(cmp);
}

// LocData[0] - Low Level Mesh Object
//vertices = InData[0]
//vertexCount = InData[1]
//indices = InData[2]
//indexCount = InData[3]

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