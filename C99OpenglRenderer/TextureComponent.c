#define STB_IMAGE_IMPLEMENTATION
#include "Component.h"
#include <stdlib.h>
#include <GL/glew.h>
#include "dataArray.h"
#include "stb_image.h"

void TextureCmpInit(Component* cmp) {
	GLuint* textureID = (GLuint*)malloc(sizeof(GLuint));
	if (!textureID) return;


	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, (*textureID));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // без mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int w, h, n;
	char* path = (char*)cmp->InData->getByIndex(cmp->InData, 0);
	unsigned char* data = stbi_load(path, &w, &h, &n, 0);
	if (!data) {
		printf("Texture %s not found\n", path);
	}

	GLenum fmt = (n == 4) ? GL_RGBA : GL_RGB;
	GLint ifmt = (n == 4) ? GL_RGBA8 : GL_RGB8;

	glTexImage2D(GL_TEXTURE_2D, 0, ifmt, w, h, 0,
		fmt, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	int* Location = (int*)malloc(sizeof(int));
	if (!Location) return;

	GLuint* shaderProgram = (GLuint*)cmp->InData->getByIndex(cmp->InData, 1);
	(*Location) = glGetUniformLocation((*shaderProgram), "u_tex");

	cmp->LocData->addToDataArr(cmp->LocData, textureID);
	cmp->LocData->addToDataArr(cmp->LocData, Location);

	cmp->isReady = 1;

}
void TextureCmpBind(Component* cmp) {

	int loc = *((int*)cmp->LocData->getByIndex(cmp->LocData, 1));
	GLuint tID = *((GLuint*)cmp->LocData->getByIndex(cmp->LocData, 0));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tID);
	glUniform1i(loc, 0);
}
void TextureCmpUnBind(Component* cmp) {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureCmpDeleteComponent(Component* cmp) {

}

//InData[0] = path to texture
//InData[1] = shaderProg ID
//LocData[0] = textureID
//LocData[1] = textureLocation
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