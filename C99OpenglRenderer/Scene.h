#ifndef SCENECLASS_H
#define SCENECLASS_H

typedef struct Entity Entity;
typedef struct dataArr dataArr;

typedef struct Scene {
	void (*addEntity)(struct Scene*, Entity*);
	Entity* (*getEntity)(struct Scene*, const char*);
	void(*renderScene)(struct Scene*);

	//private block
	dataArr* (*getInnerArray)(struct Scene*, size_t);
	size_t elementCount;
	dataArr* sceneData;

	
} Scene;

Scene* Scene_new(size_t);
void Scene_delete(Scene*);

#endif