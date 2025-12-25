#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Scene.h"
#include "platform.h"
#include "MeshObjects.h"
#include "Entity.h"

Scene* scn;

void display() {
	scn->renderScene(scn);
}

void init() {
	scn = Scene_new(1024);
	Entity* ent = Entity_new("cube");
	ent->setMesh(ent, createCube());
	ent->entityInit(ent);
	//ent->setTexture(ent, "sky.bmp");
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