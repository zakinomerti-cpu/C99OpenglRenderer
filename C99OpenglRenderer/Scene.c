#include "Scene.h"
#include "Entity.h"
#include "HashArray.h"
#include "dataArray.h"
#include "stdlib.h"

void SceneObjectRenderScene(Scene* scn) {
	int elementCount = scn->EntityArray->elementCount;
	dataArr* arr = scn->EntityArray->Data;
	for (int iter = 0; iter < elementCount; iter+=1) {
		dataArr* innerArr = (dataArr*)arr->getByIndex(arr, iter);
		if (!innerArr) continue;
		for (int i = 0; i < innerArr->size; i += 1) {
			Entity* tempEntity = (Entity*)((HashArrayElement*)innerArr->
				getByIndex(innerArr, i))->data;
			if(tempEntity) tempEntity->draw(tempEntity);
		}
		
	}
}

void SceneObjectAddEntity(Scene* scn, Entity* ent) {
	scn->EntityArray->addObject(scn->EntityArray, ent, ent->entityName);
}

Scene* Scene_new(int size) {
	Scene* scn = (Scene*)malloc(sizeof(Scene));
	if (!scn) return;
	scn->EntityArray = HashArray_new(size);
	scn->renderScene = SceneObjectRenderScene;
	scn->addEntity = SceneObjectAddEntity;

	return scn;

}
