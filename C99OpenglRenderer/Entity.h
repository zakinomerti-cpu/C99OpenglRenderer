#ifndef ENTITYCLASS_H
#define ENTITYCLASS_H

typedef struct dataArr dataArr;
typedef struct Texture Texture;
typedef struct Shader Shader;
typedef struct Mesh Mesh;
typedef struct Component Component;
typedef struct TrailPoint {
	float x; float y;
} TrailPoint;

typedef struct Entity {
	float posx, posy, posz;
	float rotx, roty, rotz;
	float sizex, sizey, sizez;
	size_t trailSize;

	Texture* tex;
	Shader* shader;
	Shader* TrailShader;
	TrailPoint* trp;
	int trpSize;

	void (*draw)(struct Entity*);
	void (*addComponent)(struct Entity*, Component*);
	void (*setPosition)(struct Entity*, float, float, float);
	void (*setRotation)(struct Entity*, float, float, float);
	void (*setScale)(struct Entity*, float, float, float);
	void (*entityInit)(struct Entity*);
	char* (*getEntityName)(struct Entity*);

	void (*setVertexShader)(struct Entity*, const char*);
	void (*setFragmentShader)(struct Entity*, const char*);
	void (*setTrail)(struct Entity*, size_t);

	void (*setTexture)(struct Entity*, const char*);

	unsigned int trailsVbo;

	unsigned int posAttrib;
	unsigned int normAttrib;
	unsigned int textureAttrib;
	unsigned int texCrdAttrib;
	unsigned int TrailPosAttrib;


	unsigned char isShaderInit;
	unsigned char isEntInit;
	char* entityName;

	char* vertexShader;
	char* fragmentShader;

	dataArr* component;

} Entity;

Entity* Entity_new(const char* name);
void Entity_delete(Entity* ent);

#endif