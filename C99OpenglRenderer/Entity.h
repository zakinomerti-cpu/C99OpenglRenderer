#ifndef ENTITYCLASS_H
#define ENTITYCLASS_H

typedef struct dataArr dataArr;
typedef struct Render Render;

typedef struct Entity {
	char* entityName;
	void (*setPosition)(struct Entity*, float, float, float);
	void (*setRotation)(struct Entity*, float, float, float);
	void (*setScale)(struct Entity*, float, float, float);

	float* (*getPosition)(struct Entity*);
	float* (*getRotation)(struct Entity*);
	float* (*getScale)(struct Entity*);

	void (*setInputData)(struct Entity*, dataArr*);
	void (*setStartScript)(struct Entity*, const char*);
	void (*setUpdateScript)(struct Entity*, const char*);

	void (*onStart)(struct Entity*);
	void (*onUpdate)(struct Entity*);

	void (*entityInit)(struct Entity*);
	void (*draw)(struct Entity*);

	dataArr* LocalData;
	dataArr* InData;
	Render* render;

	const char* stScriptPath;
	const char* updtScriptPath;
	char isReady;

} Entity;

Entity* EntityCube_new(const char* name);
void Entity_delete(Entity* ent);

#endif