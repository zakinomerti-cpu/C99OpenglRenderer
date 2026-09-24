#include "Scene.h"
#include "Entity.h"
#include "stdlib.h"
#include "EngineContext.h"
#include "hashArray.h"
#include "pVoidArray.h"
#include "string.h"

void SceneObjectRenderScene(Scene* scn) {
	int elementCount = scn->EntityArray->size;
	pVoidArray* arr = (pVoidArray*)scn->EntityArray->buckets;
	for (int iter = 0; iter < elementCount; iter+=1) {
		pVoidArray* innerArr = NULL;
		arr->ops->get(arr, (void*)&innerArr, iter);
		if (!innerArr) continue;
		for (int i = 0; i < innerArr->size; i += 1) {

			Entity* tempEntity	= NULL;
			innerArr->ops->get(innerArr, (void*)&tempEntity, i);

			if (tempEntity) {
				tempEntity->draw(tempEntity);
				tempEntity->onUpdate(tempEntity, scn->ectx);
			}
		}
		
	}
}

void SceneObjectAddEntity(Scene* scn, Entity* ent) {
	const char* key = ent->entityName;
	size_t keylen = strlen(key);
	void* value = (void*)ent;
	scn->EntityArray->ops->put(scn->EntityArray, key, keylen, value);
}

void SceneObjectSetEngineContext(Scene* scn, EngineCtx* ctx) {
	scn->ectx = ctx;
}

Scene* Scene_new(int size) {
	Scene* scn = (Scene*)malloc(sizeof(Scene));
	if (!scn) return NULL;
	scn->ectx = NULL;
	scn->setEngineContext = SceneObjectSetEngineContext;
	
	scn->EntityArray = NULL;
	hashArray_new(&scn->EntityArray, size);
	scn->renderScene = SceneObjectRenderScene;
	scn->addEntity = SceneObjectAddEntity;

	return scn;

}
