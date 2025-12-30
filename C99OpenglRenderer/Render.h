#ifndef RENDERCLASS_H
#define RENDERCLASS_H

typedef struct Texture Texture;
typedef struct Shader Shader;
typedef struct Mesh Mesh;
typedef struct dataArr dataArr;

typedef struct Render {
	char* name;
	void (*setMesh)(struct Render*, Mesh*);
	void (*setShader)(struct Render*, Shader*);
	void (*setTexture)(struct Render*, Texture*);
	void (*setPostion)(struct Render*, float, float, float);
	void (*setRotation)(struct Render*, float, float, float);
	void (*setScale)(struct Render*, float, float, float);
	void (*renderInit)(struct Render*);
	void (*rend)(struct Render*);
	Texture* tex;
	Shader* shad;
	Mesh* mesh;

	int unSize;
	float pos[3];
	float rot[3];
	float size[3];
	dataArr* unData;

	char isReady;
} Render;

Render* Render_new(char* name);

#endif