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

	cmp->isReady = 1;

}
void ShaderCmpBind(Component* cmp) {

	Shader* shader = (Shader*)cmp->LocData->getByIndex(cmp->LocData, 2);
	shader->shaderBind(shader);
}
void ShaderCmpUnBind(Component* cmp) {
	Shader* shader = (Shader*)cmp->LocData->getByIndex(cmp->LocData, 2);
	shader->shaderUnBind(shader);
}

void ShaderCmpAddChild(Component* cmp, Component* chd) {}
void ShaderCmpRemoveChild(Component* cmp, Component* chd) {}
void ShaderCmpDeleteComponent(Component* cmp) {}


// LocData[0] = vs
// LocData[1] = fs
// LocData[2] = Shader

// компонент ожидает, что InData[0], InData[1] это vs, fs
Component* ShaderComponent_new(Component* prnt, Entity* ent, dataArr* InData) {
	Component* cmp = (Component*)malloc(sizeof(Component));
	if (!cmp) return NULL;

	cmp->Init = ShaderCmpInit;
	cmp->Bind = ShaderCmpBind;
	cmp->UnBind = ShaderCmpUnBind;
	cmp->DeleteComponent = ShaderCmpDeleteComponent;
	cmp->parentEntity = ent;

	cmp->InData = InData;
	cmp->child = NULL;
	cmp->LocData = dataArr_new();
	if (!cmp->LocData) {
		free(cmp);
		return NULL;
	}
	cmp->LocData->addToDataArr(cmp->LocData, InData->getByIndex(InData, 0));
	cmp->LocData->addToDataArr(cmp->LocData, InData->getByIndex(InData, 1));
	
	cmp->isReady = 0;
	cmp->chdCount = -1;
	
	return cmp;
}