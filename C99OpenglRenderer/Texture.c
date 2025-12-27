#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "stdio.h"
#include "GL/glew.h"
#include "stb_image.h"

void BindTextureObject(Texture* tx, GLuint prg) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tx->tex);
	glUniform1i(tx->loc, 0);
}

void UnBindTextureObject(Texture* tx) {
	glBindTexture(GL_TEXTURE_2D, 0);
}

//обязятельно в шейдере должен быть саплер 2д с именем u_tex
//иначе пиздец и не запустится
Texture* Texture_new(const char* path, unsigned int shd_prg) {
	Texture* out = (Texture*)malloc(sizeof(Texture));
	if (!out) return NULL;

	out->bindTexture = BindTextureObject;
	out->unbindTexture = UnBindTextureObject;

	out->tex = 0;
	glGenTextures(1, &out->tex);
	glBindTexture(GL_TEXTURE_2D, out->tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // без mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int w, h, n;
	unsigned char* data = stbi_load(path, &w, &h, &n, 0);
	if (!data) {
		printf("Texture %s not found", path);
		exit(-1);
	}

	GLenum fmt = (n == 4) ? GL_RGBA : GL_RGB;
	GLint  ifmt = (n == 4) ? GL_RGBA8 : GL_RGB8;

	glTexImage2D(GL_TEXTURE_2D, 0, ifmt, w, h, 
		0, fmt, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);


	glBindTexture(GL_TEXTURE_2D, 0);
	out->loc = glGetUniformLocation(shd_prg, "u_tex");
	return out;

}