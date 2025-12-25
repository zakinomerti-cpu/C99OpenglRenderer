#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

typedef struct Texture {
	void (*bindTexture)(struct Texture*);
	void (*unbindTexture)(struct Texture*);
	void (*deleteTexture)(struct texture*);
	unsigned int tex;
	int loc;

} Texture;

Texture* Texture_new(const char* path, unsigned int shd_prg);
#endif