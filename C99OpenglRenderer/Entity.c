#include "Entity.h"
#include "stdio.h"
#include "stdlib.h"
#include "GL/glew.h"
#include "string.h"
#include "Texture.h"
#include "dataArray.h"
#include "Component.h"

#include "Mesh.h"
#include "Shader.h"

void setPosition(Entity* ent, float x, float y, float z) {
	ent->posx = x; ent->posy = y; ent->posz = z;
}
void setRotation(Entity* ent, float x, float y, float z) {
	ent->rotx = x; ent->roty = y; ent->rotz = z;
}
void setScale(Entity* ent, float x, float y, float z) {
	ent->sizex = x; ent->sizey = y; ent->sizez = z;
}

//добавляй текстуры только после entityInit
void entityInit(Entity* ent) {

	Component* RenderComponent = RenderComponent_new(NULL, ent, ent->component);
	RenderComponent->Init(RenderComponent);
	ent->component->addToDataArr(ent->component, RenderComponent);

}
void draw(Entity* ent) {
	Component* RenderComponent = (Component*)ent->component->getByIndex(ent->component, 3);
	RenderComponent->Bind(RenderComponent);

}

void addComponent(Entity* ent, Component* cmp) {
	ent->component->addToDataArr(ent->component, cmp);
}

Entity* Entity_new(const char* name) {
	Entity* ent = (Entity*)malloc(sizeof(Entity));
	if (!ent) return NULL;
	ent->posx = 0;
	ent->posy = 0;
	ent->posz = 0;

	ent->rotx = 0;
	ent->roty = 0;
	ent->rotz = 0;

	ent->sizex = 1;
	ent->sizey = 1;
	ent->sizez = 1;

	ent->draw = draw;
	ent->addComponent = addComponent;
	ent->setPosition = setPosition;
	ent->setRotation = setRotation;
	ent->setScale = setScale;
	ent->entityInit = entityInit;
	
	ent->component = dataArr_new();

	ent->isEntInit = 0;

	size_t iter = 0;
	ent->entityName = (char*)malloc(sizeof(char) * (strlen(name)+1));
	if (!ent->entityName) return NULL;
	for (; iter < strlen(name); iter += 1) {
		ent->entityName[iter] = name[iter];
	}
	ent->entityName[iter] = '\0';
	return ent;
}

void Entity_delete(Entity* ent) {
	ent->draw = NULL;
	ent->setPosition = NULL;
	ent->setRotation = NULL;
	ent->setScale = NULL;
	ent->entityInit = NULL;
	ent->getEntityName = NULL;

	free(ent->entityName);
	ent->entityName = NULL;

	free(ent);
	ent = NULL;
}