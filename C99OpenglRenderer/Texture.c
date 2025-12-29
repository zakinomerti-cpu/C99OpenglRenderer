#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "stdio.h"
#include "GL/glew.h"
#include "stb_image.h"

void BindTextureObject(Texture* tx) {
	if (!tx->isReady) return;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tx->tex);
	glUniform1i(tx->loc, 0);
}

void UnBindTextureObject(Texture* tx) {
	if (!tx->isReady) return;
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SetShaderProgramTextureObject(Texture* tx, unsigned int sp) {
	tx->sp = sp;
}

void SetPathToTextureTextureObject(Texture* tx, const char* path) {
	tx->path = path;
}

void TextureInitTextureObject(Texture* tx) {
	if (!tx->sp) return;
	if (!tx->path) return;
	glGenTextures(1, &tx->tex);
	glBindTexture(GL_TEXTURE_2D, tx->tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // без mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int w, h, n;
	unsigned char* data = stbi_load(tx->path, &w, &h, &n, 0);
	if (!data) {
		printf("Texture %s not found", tx->path);
		exit(-1);
	}

	GLenum fmt = (n == 4) ? GL_RGBA : GL_RGB;
	GLint  ifmt = (n == 4) ? GL_RGBA8 : GL_RGB8;

	glTexImage2D(GL_TEXTURE_2D, 0, ifmt, w, h,
		0, fmt, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);


	glBindTexture(GL_TEXTURE_2D, 0);
	tx->loc = glGetUniformLocation(tx->sp, "u_tex");
	tx->isReady = 1;
}

//обязятельно в шейдере должен быть саплер 2д с именем u_tex
//иначе пиздец и не запустится
Texture* Texture_new(char* TextureName) {
	if (!TextureName) return NULL;
	Texture* out = (Texture*)malloc(sizeof(Texture));
	if (!out) return NULL;

	out->textureName = NULL;
	out->textureName = _strdup(TextureName);
	if (!out->textureName) { free(out); return NULL; }

	out->setShaderProgram = SetShaderProgramTextureObject;
	out->setPathToTexture = SetPathToTextureTextureObject;
	out->textureBind = BindTextureObject;
	out->textureUnBind = UnBindTextureObject;
	out->textureInit = TextureInitTextureObject;

	out->sp = 0;
	out->path = 0;
	out->tex = 0;

	out->loc = 0;
	out->isReady = 0;
	return out;

}