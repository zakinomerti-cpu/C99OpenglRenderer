#ifndef RENDERCLASS_H
#define RENDERCLASS_H

typedef struct Texture Texture;
typedef struct Shader Shader;
typedef struct Mesh Mesh;

typedef struct Render {
	char* name;
	void (*setMesh)(struct Render*, Mesh*);
	void (*setShader)(struct Render*, Shader*);
	void (*setTexture)(struct Render*, Texture*);
	void (*renderInit)(struct Render*);
	void (*rend)(struct Render*);
	Texture* tex;
	Shader* shad;
	Mesh* mesh;

	char isReady;
} Render;

Render* Render_new(char* name);

#endif