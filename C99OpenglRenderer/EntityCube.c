#include "Entity.h"
#include <stdlib.h>
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Render.h"
#include <string.h>
#include "dataArray.h"
#include "MeshObjects.h"
#include "PythonHeader.h"

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
	ent->render->pos[0] = x;
	ent->render->pos[1] = y;
	ent->render->pos[2] = z;
}
void FUNC(SetRotation)(Entity* ent, float x, float y, float z) {
	ent->render->rot[0] = x;
	ent->render->rot[1] = y;
	ent->render->rot[2] = z;
}
void FUNC(SetScale)(Entity* ent, float x, float y, float z) {
	ent->render->size[0] = x;
	ent->render->size[1] = y;
	ent->render->size[2] = z;
}
float* FUNC(GetPosition)(Entity* ent) {
	float* pos = (float*)malloc(sizeof(float)*3);
	if (!pos) return;
	pos[0] = ent->render->pos[0];
	pos[1] = ent->render->pos[1];
	pos[2] = ent->render->pos[2];
	return pos;
}
float* FUNC(getRotation)(Entity* ent) {
	float* rot = (float*)malloc(sizeof(float) * 3);
	if (!rot) return;
	rot[0] = ent->render->rot[0];
	rot[1] = ent->render->rot[1];
	rot[2] = ent->render->rot[2];
	return rot;
}
float* FUNC(GetScale)(Entity* ent) {
	float* size = (float*)malloc(sizeof(float) * 3);
	if (!size) return;
	size[0] = ent->render->size[0];
	size[1] = ent->render->size[1];
	size[2] = ent->render->size[2];
	return size;
}

//fffffff
void FUNC(setInputData)(Entity* ent, dataArr* InData) {
	if (!InData) return;
	if (InData->size != 1) return;
	ent->InData = InData;
}
void FUNC(setStartScript)(Entity* ent, const char* path) {
	ent->stScriptPath = path;
}
void FUNC(setUpdateScript)(Entity* ent, const char* path) {
	ent->updtScriptPath = path;
}
void FUNC(onStart)(Entity* ent) {
	PythonScript* ps = PythonScript_new(ent, SCRIPT_START, "script");
	if (!ps) return;

	ps->scriptInit(ps, "script.py");
	ps->scriptBind(ps);
}
void FUNC(onUpdate)(Entity* ent) {

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
	rnd->setMesh(rnd, mesh);
	rnd->setTexture(rnd, tex);
	rnd->setShader(rnd, shd);
	rnd->renderInit(rnd);

	ent->render = rnd;
	ent->isReady = 1;

	ent->onStart(ent);

}
void FUNC(Draw)(Entity* ent) {
	if (!ent->isReady) {
		printf("entity %s not ready to draw", ent->entityName);
	}

	Render* rnd = ent->render;
	rnd->rend(rnd);
}

Entity* EntityCube_new(const char* name) {
	if (!name) return NULL;
	Entity* ent = (Entity*)malloc(sizeof(Entity));
	if (!ent) return;

	ent->entityName = NULL;
	ent->entityName = _strdup(name);

	ent->setPosition = FUNC(SetPosition);
	ent->setRotation = FUNC(SetRotation);
	ent->setScale = FUNC(SetScale);
	ent->setInputData = FUNC(setInputData);

	ent->onStart = FUNC(onStart);
	ent->onUpdate = FUNC(onUpdate);

	ent->entityInit = FUNC(EntityInit);
	ent->draw = FUNC(Draw);

	ent->LocalData = NULL;
	ent->InData = NULL;
	ent->render = NULL;

	ent->stScriptPath = NULL;
	ent->updtScriptPath = NULL;

	ent->isReady = 0;
	return ent;
}
