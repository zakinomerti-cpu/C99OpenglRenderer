#include "Component.h"
#include <stdlib.h>
#include "dataArray.h"
#include <GL/glew.h>
#include "Shader.h"
#include "Entity.h"
#include "Mesh.h"

typedef struct AttribArrayElement {
	char* name;
	unsigned int loc;
} AttribArrayElement;

void RenderCmpInit(Component* cmp) {
	Component* MshCmp = (Component*)cmp->InData->getByIndex(cmp->InData, 0);
	Component* ShdCmp = (Component*)cmp->InData->getByIndex(cmp->InData, 1);
	Component* TexCmp = (Component*)cmp->InData->getByIndex(cmp->InData, 2);
	if (!(MshCmp->isReady && ShdCmp->isReady && TexCmp->isReady)) exit(-455);

	Shader* shd = (Shader*)ShdCmp->LocData->getByIndex(ShdCmp->LocData, 2);
	GLuint* posAttrib = (GLuint*)malloc(sizeof(GLuint)); if (!posAttrib) return;
	GLuint* normAttrib = (GLuint*)malloc(sizeof(GLuint)); if (!normAttrib) return;
	GLuint* textureAttrib = (GLuint*)malloc(sizeof(GLuint)); if (!textureAttrib) return;
	GLuint* texCrdAttrib = (GLuint*)malloc(sizeof(GLuint)); if (!texCrdAttrib) return;
	(*posAttrib) = glGetAttribLocation(shd->shaderProgram, "position");
	(*normAttrib) = glGetAttribLocation(shd->shaderProgram, "norm");
	(*textureAttrib) = glGetUniformLocation(shd->shaderProgram, "u_tex");
	(*texCrdAttrib) = glGetAttribLocation(shd->shaderProgram, "texcoord");

	MshCmp->Bind(MshCmp);

	glEnableVertexAttribArray(*posAttrib);
	glVertexAttribPointer(*posAttrib, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(*normAttrib);
	glVertexAttribPointer(*normAttrib, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(*texCrdAttrib);
	glVertexAttribPointer(*texCrdAttrib, 2, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)(6 * sizeof(float)));

	MshCmp->UnBind(MshCmp);

	dataArr* AttribArray = dataArr_new();
	AttribArray->addToDataArr(AttribArray, posAttrib);
	AttribArray->addToDataArr(AttribArray, normAttrib);
	AttribArray->addToDataArr(AttribArray, textureAttrib);
	AttribArray->addToDataArr(AttribArray, texCrdAttrib);
	cmp->LocData->addToDataArr(cmp->LocData, AttribArray);
	cmp->isReady = 1;

}
void RenderCmpBind(Component* cmp) {

	Entity* ent = cmp->parentEntity;
	dataArr* AttibArray = (dataArr*)cmp->LocData->getByIndex(cmp->LocData, 0);
	Component* MshCmp = (Component*)cmp->InData->getByIndex(cmp->InData, 0);
	Component* ShdCmp = (Component*)cmp->InData->getByIndex(cmp->InData, 1);
	Component* TexCmp = (Component*)cmp->InData->getByIndex(cmp->InData, 2);


	ShdCmp->Bind(ShdCmp);
	glPushMatrix();

	glTranslatef(ent->posx, ent->posy, ent->posz);
	glRotatef(ent->rotx, 1, 0, 0);
	glRotatef(ent->roty, 0, 1, 0);
	glRotatef(ent->rotz, 0, 0, 1);
	glScalef(ent->sizex, ent->sizey, ent->sizez);

	MshCmp->Bind(MshCmp);

	GLuint* posAttrib = (GLuint*)AttibArray->getByIndex(AttibArray, 0);
	GLuint* normAttrib = (GLuint*)AttibArray->getByIndex(AttibArray, 1);
	GLuint* textureAttrib = (GLuint*)AttibArray->getByIndex(AttibArray, 2);
	GLuint* texCrdAttrib = (GLuint*)AttibArray->getByIndex(AttibArray, 3);
	glEnableVertexAttribArray(*posAttrib);
	glEnableVertexAttribArray(*normAttrib);
	glEnableVertexAttribArray(*texCrdAttrib);

	TexCmp->Bind(TexCmp);
	Mesh* msh = (Mesh*)MshCmp->InData->getByIndex(MshCmp->InData, 0);

	GLint eboBind = 0;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eboBind);

	glDrawElements(GL_TRIANGLES, msh->indexCount, GL_UNSIGNED_BYTE, 0);

	glDisableVertexAttribArray(*posAttrib);
	glDisableVertexAttribArray(*normAttrib);
	glDisableVertexAttribArray(*texCrdAttrib);

	TexCmp->UnBind(TexCmp);
	MshCmp->UnBind(MshCmp);

	glPopMatrix();
	ShdCmp->UnBind(ShdCmp);
}
void RenderCmpUnBind(Component* cmp) {

}
void RenderCmpDeleteComponent(Component* cmp) {

}
void RenderCmpAddChild(Component* prnt, Component* cmp) {

}

// InData[0] - MeshComponent
// InData[1] - ShaderComponent
// Indata[2] - TextureComponet
// все компоненты должны быть инициализированы заранее
// LocData[0] = AttribArray
Component* RenderComponent_new(Component* prnt, Entity* ent, dataArr* InData) {

	Component* cmp = (Component*)malloc(sizeof(Component));
	if (!cmp) return;
	cmp->Init = RenderCmpInit;
	cmp->Bind = RenderCmpBind;
	cmp->UnBind = RenderCmpUnBind;
	cmp->DeleteComponent = RenderCmpDeleteComponent;
	cmp->AddChild = RenderCmpAddChild;
	cmp->parentEntity = ent;

	cmp->InData = InData;
	cmp->LocData = dataArr_new();
	if (!cmp->LocData) {
		free(cmp);
		return NULL;
	}
	cmp->child = NULL;
	cmp->isReady = 0;
	cmp->chdCount = 0;
	return cmp;
}