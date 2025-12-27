#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Scene.h"
#include "platform.h"
#include "MeshObjects.h"
#include "Entity.h"
#include "Component.h"
#include "dataArray.h"
#include "Shader.h"

const char* standart_vs =
"#version 120\n"
"attribute vec3 position;\n"
"attribute vec3 norm;\n"
"attribute vec2 texcoord;\n"
"varying vec2 vTexCoord;\n"
"varying vec3 vNormal;\n"
"void main() {"
"	gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);"
"	vNormal = norm;"
"	vTexCoord = texcoord;"
"}";

const char* standart_fs =
"#version 120\n"
"varying vec3 vNormal;"
"varying vec2 vTexCoord;"
"uniform sampler2D u_tex;\n"
"void main() {\n"
"    gl_FragColor = texture2D(u_tex, vTexCoord);\n"
"}";

Scene* scn;

void display() {
	scn->renderScene(scn);
}

void init() {
	scn = Scene_new(1024);
	Entity* ent = Entity_new("cube");

	Mesh* ms = createCube();

	dataArr* MeshCompData = dataArr_new();
	MeshCompData->addToDataArr(MeshCompData, ms->vertices);
	MeshCompData->addToDataArr(MeshCompData, ms->vertexCount);
	MeshCompData->addToDataArr(MeshCompData, ms->indices);
	MeshCompData->addToDataArr(MeshCompData, ms->indexCount);
	Component* Meshcmp = MeshComponent_new(NULL, NULL, MeshCompData);
	Meshcmp->Init(Meshcmp);
	ent->addComponent(ent, Meshcmp);

	dataArr* ShaderCompData = dataArr_new();
	ShaderCompData->addToDataArr(ShaderCompData, standart_vs);
	ShaderCompData->addToDataArr(ShaderCompData, standart_fs);
	Component* Shadercmp = ShaderComponent_new(NULL, NULL, ShaderCompData);
	Shadercmp->Init(Shadercmp);
	ent->addComponent(ent, Shadercmp);

	dataArr* TextureCompData = dataArr_new();
	TextureCompData->addToDataArr(TextureCompData, "sky.bmp");
	Component* shader = ((Component*)ent->component->getByIndex(ent->component, 1));
	Shader* shd = (Shader*)shader->LocData->getByIndex(shader->LocData, 2);
	GLuint shaderProgram = shd->shaderProgram;
	TextureCompData->addToDataArr(TextureCompData, &shaderProgram);
	Component* Texturecmp = TextureComponent_new(NULL, NULL, TextureCompData);
	Texturecmp->Init(Texturecmp);
	ent->addComponent(ent, Texturecmp);

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