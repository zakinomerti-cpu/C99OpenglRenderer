#include "Entity.h"
#include <stdlib.h>
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Render.h"
#include <string.h>
#include "dataArray.h"
#include "MeshObjects.h"

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

//обязательно для каждого entity
//иметь уникальный префикс
#define PREFIX_ cubeEntity
#define FUNC(name) PREFIX##name

void FUNC(SetPosition)(Entity* ent, float x, float y, float z) {
	ent->posx = x; ent->posy = y; ent->posz;
}
void FUNC(SetRotation)(Entity* ent, float x, float y, float z) {
	ent->rotx = x; ent->roty = y; ent->rotz = z;
}
void FUNC(SetScale)(Entity* ent, float x, float y, float z) {
	ent->sizex = x; ent->sizey = y; ent->sizez = z;
}
void FUNC(setInputData)(Entity* ent, dataArr* InData) {
	if (!InData) return;
	if (InData->size != 1) return;
	ent->InData = InData;
}
void FUNC(EntityInit)(Entity* ent) {
	if (!ent->InData) return;
	ent->LocalData = dataArr_new();
	if (!ent->LocalData) return;

	Texture* tex = Texture_new("texture1");
	if (!tex) return NULL;
	Shader* shd = Shader_new("shader1");
	if (!shd) { free(tex); return NULL; }
	Mesh* mesh = Mesh_new("mesh1");
	if (!mesh) { free(tex); free(shd); return NULL; }
	Render* rnd = Render_new("render1");
	if (!mesh) { free(tex); free(shd); free(mesh); return NULL; }

	Mesh* submesh = createCube();
	mesh->vertexCount = submesh->vertexCount;
	mesh->vertices = submesh->vertices;
	mesh->indexCount = submesh->indexCount;
	mesh->indices = submesh->indices;
	mesh->meshInit(mesh);

	shd->setVertexShader(shd, standart_vs);
	shd->setFragmentShader(shd, standart_fs);
	shd->shaderInit(shd);

	//обязательно после инициализации
	tex->setShaderProgram(tex, shd->shaderProgram);
	tex->setPathToTexture(tex, (char*)ent->InData->getByIndex(ent->InData, 0));
	tex->textureInit(tex);

	if (!(tex->isReady && shd->isReady && mesh->isReady)) return;

	ent->LocalData = dataArr_new();
	ent->LocalData->addToDataArr(ent->LocalData, mesh);
	ent->LocalData->addToDataArr(ent->LocalData, shd);
	ent->LocalData->addToDataArr(ent->LocalData, tex);
	ent->LocalData->addToDataArr(ent->LocalData, rnd);
	ent->isReady = 1;

}
void FUNC(Draw)(Entity* ent) {
	if (!ent->isReady) {
		printf("entity %s not ready to draw", ent->entityName);
	}

	Texture* tx = (Texture*)ent->LocalData->getByIndex(ent->LocalData, 2);
	Shader* shd = (Shader*)ent->LocalData->getByIndex(ent->LocalData, 1);
	Mesh* mesh = (Mesh*)ent->LocalData->getByIndex(ent->LocalData, 0);

	Render* rnd = (Render*)ent->LocalData->getByIndex(ent->LocalData, 3);
	rnd->setMesh(rnd, mesh);
	rnd->setTexture(rnd, tx);
	rnd->setShader(rnd, shd);

	rnd->rend(rnd);
}

Entity* EntityCube_new(const char* name) {
	if (!name) return NULL;
	Entity* ent = (Entity*)malloc(sizeof(Entity));
	if (!ent) return;

	ent->posx = ent->posy = ent->posz = 0.0f;
	ent->rotx = ent->roty = ent->rotz = 0.0f;
	ent->sizex = ent->sizey = ent->sizez = 1.0f;

	ent->entityName = NULL;
	ent->entityName = _strdup(name);

	ent->setPosition = FUNC(SetPosition);
	ent->setRotation = FUNC(SetRotation);
	ent->setScale = FUNC(SetScale);
	ent->setInputData = FUNC(setInputData);
	ent->entityInit = FUNC(EntityInit);
	ent->draw = FUNC(Draw);

	ent->LocalData = NULL;
	ent->InData = NULL;

	ent->isReady = 0;
	return ent;
}
