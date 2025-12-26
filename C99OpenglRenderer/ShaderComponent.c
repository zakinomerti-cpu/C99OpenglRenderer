#include "Component.h"
#include "gl/glew.h"
#include <stdio.h>
#include <stdlib.h>
#include "dataArray.h"
#include "Shader.h"

// LocData[2] есть сам шейдер
void ShaderCmpInit(Component* cmp) {
	Shader* shd = Shader_new("ShaderFromComponent");
	shd->setVertexShader(shd, (const char*)(cmp->LocData->getByIndex(cmp->LocData, 0)));
	shd->setFragmentShader(shd, (const char*)(cmp->LocData->getByIndex(cmp->LocData, 1)));
	shd->shaderInit(shd);
	cmp->LocData->addToDataArr(cmp->LocData, shd);

}
void ShaderCmpBind(Component* cmp) {

}
void ShaderCmpUnBind(Component* cmp) {}
void ShaderCmpAddChild(Component* cmp, Component* chd) {}
void ShaderCmpRemoveChild(Component* cmp, Component* chd) {}
void ShaderCmpDeleteComponent(Component* cmp) {}


// LocData[0] = vs
// LocData[1] = fs
// LocData[2] = Shader
// LocData[3] = attribArray: в LocData[3] лежит dataArr массив аттрибутов шейдера
// LocData[4] = uniformArray: в LocData[3] лежит dataArr массив uniform'ов шейдера

// компонент ожидает, что InData[0], InData[1] это vs, fs, InData[2]
// это массив аттрибутов( структур содержащих имя, размерность и данные )
// по ним создается glVertexAttribPointer с нужными параметрами
// InData[3] это массив uniform'ов
Component* ShaderComponent_new(Component* prnt, Entity* ent, dataArr* InData) {
	Component* cmp = (Component*)malloc(sizeof(Component));
	if (!cmp) return NULL;

	cmp->Init = ShaderCmpInit;
	cmp->Bind = ShaderCmpBind;
	cmp->UnBind = ShaderCmpUnBind;
	cmp->AddChild = ShaderCmpAddChild;
	cmp->RemoveChild = ShaderCmpRemoveChild;
	cmp->DeleteComponent = ShaderCmpDeleteComponent;

	cmp->parentCmp = prnt;
	cmp->parentEntity = ent;

	cmp->InData = InData;
	cmp->child = dataArr_new();
	cmp->LocData->addToDataArr(cmp->LocData, InData->getByIndex(InData, 0));
	cmp->LocData->addToDataArr(cmp->LocData, InData->getByIndex(InData, 0));
	cmp->LocData = dataArr_new();
	return cmp;
}