#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Scene.h"
#include "platform.h"
#include "MeshObjects.h"
#include "Entity.h"
#include "Component.h"
#include "dataArray.h"

const char* standart_vs =
"#version 120\n"
"attribute vec3 position;"
"attribute vec3 normal;"
"attribute vec2 texcoord;"
"varying vec2 vTexCoord;"
"varying vec3 vNormal;"
"void main() {"
"	gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);"
"	vNormal = normal;"
"	vTexCoord = texcoord;"
"}";

const char* standart_fs =
"#version 120\n"
"varying vec3 vNormal;"
"varying vec2 vTexCoord;"
"uniform sampler2D u_tex;\n"
"void main() {\n"
"    gl_FragColor = vec4(0.1,0.2,0.3, 1.0);\n"
"}";

Scene* scn;

void display() {
	scn->renderScene(scn);
}

void init() {
	scn = Scene_new(1024);
	Entity* ent = Entity_new("cube");


	dataArr* MeshCompData = dataArr_new();
	MeshCompData->addToDataArr(MeshCompData, createCube());
	Component* Meshcmp = MeshComponent_new(NULL, NULL, MeshCompData);
	ent->addComponent(ent, Meshcmp);

	dataArr* ShaderCompData = dataArr_new();
	ShaderCompData->addToDataArr(ShaderCompData, standart_vs);
	ShaderCompData->addToDataArr(ShaderCompData, standart_fs);
	Component* Shadercmp = ShaderComponent_new(NULL, NULL, ShaderCompData);
	ent->addComponent(ent, Shadercmp);


	ent->entityInit(ent);
	ent->setTexture(ent, "sky.bmp");
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