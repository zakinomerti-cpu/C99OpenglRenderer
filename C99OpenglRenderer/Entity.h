#ifndef ENTITYCLASS_H
#define ENTITYCLASS_H

typedef struct dataArr dataArr;
typedef struct Texture Texture;
typedef struct Shader Shader;
typedef struct Mesh Mesh;
typedef struct Component Component;

typedef struct Entity {
	float posx, posy, posz;
	float rotx, roty, rotz;
	float sizex, sizey, sizez;

	char* entityName;
	void (*setPosition)(struct Entity*, float, float, float);
	void (*setRotation)(struct Entity*, float, float, float);
	void (*setScale)(struct Entity*, float, float, float);
	void (*setInputData)(struct Entity*, dataArr*);
	void (*entityInit)(struct Entity*);
	void (*draw)(struct Entity*);

	dataArr* LocalData;
	dataArr* InData;

	char isReady;

} Entity;

Entity* EntityCube_new(const char* name);
void Entity_delete(Entity* ent);

#endif