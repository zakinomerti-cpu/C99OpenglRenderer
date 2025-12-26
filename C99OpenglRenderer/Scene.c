#include "Scene.h"
#include "Entity.h"
#include "dataArray.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

// алгоритм djb2
// скопировал с инета, чесно не шарю как работает
long simpleHash(const char* str) {
	long hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = hash * 33 + c;
	}

	return hash;
}

dataArr* getInnerArray(Scene* scn, size_t index) {
	dataArr* tmp = (dataArr*)scn->sceneData->getByIndex(scn->sceneData, index);
	if (!tmp) {
		tmp = dataArr_new();
		scn->sceneData->data[index] = tmp;
	}
	return tmp;
}

void addEntity(Scene* scn, Entity* ent) {
	if (ent->entityName == NULL) return;
	size_t hashIndex = simpleHash(ent->entityName) % scn->elementCount;
	dataArr* innerArr = getInnerArray(scn, hashIndex);
	innerArr->addToDataArr(innerArr, ent);
}

Entity* getEntity(Scene* scn, const char* name) {
	size_t hashIndex = simpleHash(name) % scn->elementCount;
	dataArr* innerArr = getInnerArray(scn, hashIndex);
	for (size_t iter = 0; iter < innerArr->size; iter++) {
		Entity* temp_ent = innerArr->getByIndex(innerArr, iter);
		if (strcmp(temp_ent->entityName, name) == 0) {
			return temp_ent;
		}
		int a;
	}

	return NULL;
}

void renderScene(Scene* scn) {
	for (size_t iter = 0; iter < scn->elementCount; iter+=1) {
		dataArr* innerArr = scn->sceneData->getByIndex(scn->sceneData, iter);
		if (!innerArr) continue;
		for (size_t i = 0; i < innerArr->size; i += 1) {
			Entity* tempEntity = innerArr->getByIndex(innerArr, i);
			if(tempEntity) tempEntity->draw(tempEntity);
		}
		
	}
}


Scene* Scene_new(size_t size) {
	Scene* scn = (Scene*)malloc(sizeof(Scene));
	if (!scn) return;
	scn->addEntity = addEntity;
	scn->getEntity = getEntity;
	scn->renderScene = renderScene;
	scn->getInnerArray = getInnerArray;
	scn->elementCount = size;

	scn->sceneData = dataArr_new();
	for (size_t i = 0; i < size; i += 1) {
		scn->sceneData->addToDataArr(scn->sceneData,
			NULL);
	}

	return scn;

}
