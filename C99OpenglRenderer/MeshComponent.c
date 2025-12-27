#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Component.h"
#include "dataArray.h"
#include "Mesh.h"

#define ubyte unsigned char

void MeshCmpInit(Component* cmp) {
	cmp->m_Object = dataArr_new();
}
void MeshCmpBind(Component* cmp) {

}
void MeshCmpUnBind(Component* cmp) {}
void MeshCmpSystemCreateEvent(Component* cmp, SYSTEM* sys) {}
void MeshCmpSystemUpdateEvent(Component* cmp, SYSTEM* sys) {}
void MeshCmpAddObject(Component* cmp, dataArr* InData) {

	Mesh* mesh = Mesh_new();
	float* vertices = (float*)InData->getByIndex(InData, 0);
	int* vertexCount = (int*)InData->getByIndex(InData, 1);
	ubyte* indices = (ubyte*)InData->getByIndex(InData, 2);
	int* indexCount = (int*)InData->getByIndex(InData, 3);
	mesh->setVertices(mesh, vertices, vertexCount);
	mesh->setIndices(mesh, indices, indexCount);
	mesh->init(mesh);
	cmp->m_Object->addToDataArr(cmp->m_Object, mesh);
	cmp->m_iIsReady = 1;

}
void MeshCmpAddChild(Component* prnt, Component* cmp) {}
void MeshCmpRemoveChild(Component* cmp, int index) {}

//имя копируется внутрь обьекта
//InData при добавлении: arr[0] - vertices(float*), arr[1] - vertexCount(int) 
//arr[2] - indices(unsigned char*), arr[3] - indexCount(int)
Component* MeshComponent_new(Component* prnt,
	Entity* ent,
	char* CmpName)
{
	if (!CmpName) {
		printf("Component must have own name");
		return NULL;
	}
	Component* cmp = (Component*)malloc(sizeof(Component));	
	if (!cmp) return NULL;
	cmp->m_pName = (char*)malloc(sizeof(char)*(strlen(CmpName)+1));
	if (!cmp->m_pName) { free(cmp); return NULL; }
	memcpy(cmp->m_pName, CmpName, sizeof(CmpName)+1);

	cmp->TagValue = CMP_MSHOBJ;
	cmp->Init   = MeshCmpInit;
	cmp->Bind   = MeshCmpBind;
	cmp->UnBind = MeshCmpUnBind;
	cmp->AddObject   = MeshCmpAddObject;
	cmp->AddChild    = MeshCmpAddChild;
	cmp->RemoveChild = MeshCmpRemoveChild;
	cmp->SystemCreateEvent = MeshCmpSystemCreateEvent;
	cmp->SystemUpdateEvent = MeshCmpSystemUpdateEvent;
	cmp->m_Object = NULL;

	//private Block
	cmp->m_pParentEntity = ent;
	cmp->m_InData = NULL;
	cmp->m_LocData = NULL;
	cmp->m_child = NULL;

	//если m_bChdCount = -1 то
	//компонент не может иметь детей
	cmp->m_bChdCount = -1;

	//если m_iSelectedObjet = -1 
	//то компонент не имеет
	//инициализированных обьектов
	cmp->m_iSelectedObjet = -1;

	return cmp;
}