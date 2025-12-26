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

void setTexture(Entity* ent, const char* path) {
	Component* cmp2 = ((Component*)ent->component->getByIndex(ent->component, 1));
	Shader* shd = cmp2->LocData->getByIndex(cmp2->LocData, 2);
	ent->tex = Texture_new(path, shd->shaderProgram);
}
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

	dataArr* a = ent->component;

	Component* cmp = ((Component*)ent->component->getByIndex(ent->component, 0));
	cmp->Init(cmp);

	Component* cmp2 = ((Component*)ent->component->getByIndex(ent->component, 1));
	cmp2->Init(cmp2);

	Shader* shd = cmp2->LocData->getByIndex(cmp2->LocData, 2);

	ent->posAttrib = glGetAttribLocation(shd, "position");
	ent->normAttrib = glGetAttribLocation(shd, "normal");
	ent->textureAttrib = glGetUniformLocation(shd, "texSampler");
	ent->texCrdAttrib = glGetAttribLocation(shd, "texcoord");
	ent->isEntInit = 1;

}
void draw(Entity* ent) {
	if (!ent->isEntInit) {
		printf("you cant draw entity %s because does not init\n", ent->entityName);
		return;
	}

	Component* cmpShader = ((Component*)ent->component->getByIndex(ent->component, 1));
	cmpShader->Bind(cmpShader);

	glPushMatrix();

	glTranslatef(ent->posx, ent->posy, ent->posz);
	glRotatef(ent->rotx, 1, 0, 0);
	glRotatef(ent->roty, 0, 1, 0);
	glRotatef(ent->rotz, 0, 0, 1);
	glScalef(ent->sizex, ent->sizey, ent->sizez);

	Component* cmpMesh = ((Component*)ent->component->getByIndex(ent->component, 0));
	cmpMesh->Bind(cmpMesh);

	glEnableVertexAttribArray(ent->posAttrib);
	glVertexAttribPointer(ent->posAttrib, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(ent->normAttrib);
	glVertexAttribPointer(ent->normAttrib, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(ent->texCrdAttrib);
	glVertexAttribPointer(ent->texCrdAttrib, 2, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)(6 * sizeof(float)));


	if (ent->tex != NULL)
		ent->tex->bindTexture(ent->tex);

	Mesh* msh = (Mesh*)cmpMesh->InData->getByIndex(cmpMesh->InData, 0);
	glDrawElements(GL_TRIANGLES, msh->indexCount, GL_UNSIGNED_BYTE, 0);

	glDisableVertexAttribArray(ent->texCrdAttrib);
	glDisableVertexAttribArray(ent->normAttrib);
	glDisableVertexAttribArray(ent->posAttrib);

	if (ent->tex != NULL)
		ent->tex->unbindTexture(ent->tex);

	cmpMesh->UnBind(cmpMesh);
	glPopMatrix();

}

void setVertexShader(Entity* ent, const char* src) {
	char* tmp = (char*)malloc(sizeof(char) * (strlen(src) + 1));
	if (!tmp) return;

	memcpy(tmp, src, (strlen(src) + 1));
	ent->vertexShader = tmp;
}

void setFragmentShader(Entity* ent, const char* src) {
	char* tmp = (char*)malloc(sizeof(char) * (strlen(src) + 1));
	if (!tmp) return;

	memcpy(tmp, src, (strlen(src) + 1));
	ent->fragmentShader = tmp;
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

	ent->tex = NULL;
	ent->shader = NULL;

	ent->draw = draw;
	ent->addComponent = addComponent;
	ent->setPosition = setPosition;
	ent->setRotation = setRotation;
	ent->setScale = setScale;
	ent->entityInit = entityInit;
	ent->setVertexShader = setVertexShader;
	ent->setFragmentShader = setFragmentShader;
	ent->fragmentShader = NULL;
	ent->vertexShader = NULL;

	ent->setTexture = setTexture;

	ent->posAttrib = 0;
	ent->normAttrib = 0;
	ent->texCrdAttrib = 0;
	ent->textureAttrib = 0;

	
	ent->component = dataArr_new();

	ent->isEntInit = 0;
	ent->isShaderInit = 0;

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

	if(ent->shader)
		Shader_delete(ent->shader);
	free(ent->entityName);
	ent->entityName = NULL;

	free(ent);
	ent = NULL;
}