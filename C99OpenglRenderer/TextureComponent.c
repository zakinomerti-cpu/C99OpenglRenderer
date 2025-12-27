#define STB_IMAGE_IMPLEMENTATION
#include "Component.h"
#include <stdlib.h>
#include <GL/glew.h>
#include "dataArray.h"
#include "Texture.h"

void TextureCmpInit(Component* cmp) {
	const char* path = cmp->InData->getByIndex(cmp->InData, 0);
	GLuint* shaderProg = cmp->InData->getByIndex(cmp->InData, 1);
	Texture* texture = Texture_new(path, (*shaderProg));
	cmp->LocData->addToDataArr(cmp->LocData, texture);
	cmp->isReady = 1;

}
void TextureCmpBind(Component* cmp) {
	Texture* texture = cmp->LocData->getByIndex(cmp->LocData, 0);
	texture->bindTexture(texture);
}
void TextureCmpUnBind(Component* cmp) {
	Texture* texture = cmp->LocData->getByIndex(cmp->LocData, 0);
	texture->unbindTexture(texture);
}

void TextureCmpDeleteComponent(Component* cmp) {

}

//InData[0] = path to texture
//InData[1] = shaderProg ID
//LocData[0] = texture low level object
Component* TextureComponent_new(Component* prnt, Entity* ent, dataArr* InData) {
	Component* cmp = (Component*)malloc(sizeof(Component));
	if (!cmp) return NULL;

	cmp->Init = TextureCmpInit;
	cmp->Bind = TextureCmpBind;
	cmp->UnBind = TextureCmpUnBind;
	cmp->DeleteComponent = TextureCmpDeleteComponent;

	cmp->InData = InData;
	cmp->LocData = dataArr_new();
	if (!cmp->LocData) {
		free(cmp);
		return NULL;
	}

	cmp->isReady = 0;
	cmp->chdCount = -1;
	cmp->child = NULL;
	return cmp;
}