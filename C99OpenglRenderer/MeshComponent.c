#include "Component.h"
#include "Mesh.h"
#include <stdlib.h>

void MeshInit(Component* cmp) {
	glGenBuffers(1, &ent->vbo);
	glGenBuffers(1, &ent->ibo);

	glBindBuffer(GL_ARRAY_BUFFER, ent->vbo);
	glBufferData(GL_ARRAY_BUFFER,
		ent->mesh->vertexCount * sizeof(float),
		ent->mesh->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ent->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		ent->mesh->indexCount * sizeof(GLubyte),
		ent->mesh->indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Component* MeshComponent_new(Component* prnt, Entity* ent, Mesh* mesh) {
	Component* cmp = (Component*)malloc(sizeof(Component));
	cmp->parentCmp = prnt;
	cmp->parentEntity = ent;
	cmp->InData = (void*)mesh;
	cmp->LocData = (void**)malloc(sizeof(void*));

	cmp->size = 0;
	cmp->capacity = 0;
	cmp->child = NULL;
}