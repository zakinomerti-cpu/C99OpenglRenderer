#ifndef SCENECLASS_H
#define SCENECLASS_H

typedef struct Entity Entity;
typedef struct hashArray hashArray;
typedef struct EngineCtx EngineCtx;

typedef struct Scene {
	hashArray* EntityArray;
	void (*setEngineContext)(struct Scene*, EngineCtx*);
	void (*renderScene)(struct Scene*);
	void (*addEntity)(struct Scene*, Entity*);
	EngineCtx* ectx;
} Scene;

Scene* Scene_new(int);
void Scene_delete(Scene*);

#endif
