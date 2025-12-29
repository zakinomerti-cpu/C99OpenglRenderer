#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Scene.h"
#include "platform.h"
#include "MeshObjects.h"
#include "Entity.h"
#include "dataArray.h"
#include "Shader.h"

Scene* scn;

void display() {
	scn->renderScene(scn);
}

void init() {
	scn = Scene_new(1024);
	Entity* ent = EntityCube_new("cube");
	
	dataArr* InData = dataArr_new();
	InData->addToDataArr(InData, "sky.bmp");
	ent->setInputData(ent, InData);
	ent->entityInit(ent);
	ent->setPosition(ent, 0, 0, -3);
	ent->setRotation(ent, 45, 45, 0);
	scn->addEntity(scn, ent);
}

int main(int argc, char** argv) {
	Platform* plt = GlutPlatform_new(argc, argv);
	plt->setDisplayFunc(plt, display);
	plt->setStartFunc(plt, init);

	plt->createWindow(plt, "window", 800, 600);
	plt->PlatformInit(plt);

	while (!plt->shouldClose(plt)) {
		plt->render(plt);
	}

	return 0;

}