#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

typedef struct Texture {
	char* textureName;
	void (*setShaderProgram)(struct Texture*, unsigned int);
	void (*setPathToTexture)(struct Texture*, const char* path);
	void (*textureBind)(struct Texture*);
	void (*textureUnBind)(struct Texture*);
	void (*textureInit)(struct Texture*);

	unsigned int sp;
	unsigned int tex;
	char* path;

	int loc;
	char isReady;

} Texture;

Texture* Texture_new(char* TextureName);
void Texture_delete(Texture*);

#endif