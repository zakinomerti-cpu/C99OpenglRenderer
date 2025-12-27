#ifndef SCENECLASS_H
#define SCENECLASS_H

typedef struct Entity Entity;
typedef struct HashArray HashArray;

typedef struct Scene {
	HashArray* EntityArray;
	void (*renderScene)(struct Scene*);
	void (*addEntity)(struct Scene*, Entity*);
} Scene;

Scene* Scene_new(int);
void Scene_delete(Scene*);

#endif