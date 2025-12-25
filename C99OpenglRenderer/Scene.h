#ifndef SCENECLASS_H
#define SCENECLASS_H

typedef struct Entity Entity;
typedef struct dataArr dataArr;

typedef struct EntityArray {
	size_t size;
	size_t capacity;
	Entity** data;
} EntityArray;

typedef struct Scene {
	void (*addEntity)(struct Scene*, Entity*);
	Entity* (*getEntity)(struct Scene*, const char*);
	dataArr* (*getInnerArray)(struct Scene*, size_t);
	void(*renderScene)(struct Scene*);

	//private block
	size_t elementCount;

	dataArr* sceneData;
	EntityArray** firstRenderOrder;
	EntityArray** secondRenderOrder;
	void(*addToFRO)(struct Scene*, Entity*);
	void(*addToSRO)(struct Scene*, Entity*);

	
} Scene;

Scene* Scene_new(size_t);
void Scene_delete(Scene*);

#endif