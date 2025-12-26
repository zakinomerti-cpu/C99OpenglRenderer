#include "Entity.h"
#include "stdio.h"
#include "stdlib.h"
#include "GL/glew.h"
#include "string.h"
#include "Texture.h"
#include "dataArray.h"
#include "Component.h"

#include "Mesh.h"
#include "Shader.h"

void setTexture(Entity* ent, const char* path) {
	ent->tex = Texture_new(path, ent->shader->shaderProgram);
}

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
"    gl_FragColor = texture2D(u_tex, vTexCoord);\n"
"}";

const char* trail_vs =
"#version 120\n"
"attribute vec2 position;"
"void main() {"
"	gl_Position = gl_ModelViewProjectionMatrix * vec4(position.x, position.y, -3.0, 1.0);"
"}";

const char* trail_fs =
"#version 120\n"
"void main() {"
"	gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);"
"}";

void setPosition(Entity* ent, float x, float y, float z) {
	ent->posx = x; ent->posy = y; ent->posz = z;
}
void setRotation(Entity* ent, float x, float y, float z) {
	ent->rotx = x; ent->roty = y; ent->rotz = z;
}
void setScale(Entity* ent, float x, float y, float z) {
	ent->sizex = x; ent->sizey = y; ent->sizez = z;
}

//добавляй текстуры только после entityInit
void entityInit(Entity* ent) {

	ent->shader = Shader_new("baseShader");
	if (ent->vertexShader == NULL) ent->vertexShader = standart_vs;
	if (ent->fragmentShader == NULL) ent->fragmentShader = standart_fs;
	ent->shader->setVertexShader(ent->shader, ent->vertexShader);
	ent->shader->setFragmentShader(ent->shader, ent->fragmentShader);
	ent->shader->shaderInit(ent->shader);
	if (ent->shader->isReady) ent->isShaderInit = 1;

	Component* cmp = ((Component*)ent->component->getByIndex(ent->component, 0));
	cmp->Init(cmp);

	ent->posAttrib = glGetAttribLocation(ent->shader->shaderProgram, "position");
	ent->normAttrib = glGetAttribLocation(ent->shader->shaderProgram, "normal");
	ent->textureAttrib = glGetUniformLocation(ent->shader->shaderProgram, "texSampler");
	ent->texCrdAttrib = glGetAttribLocation(ent->shader->shaderProgram, "texcoord");
	ent->isEntInit = 1;

	if (ent->trailSize > 0) {

		ent->TrailShader = Shader_new("trailShader");
		ent->TrailShader->setVertexShader(ent->TrailShader, trail_vs);
		ent->TrailShader->setFragmentShader(ent->TrailShader, trail_fs);
		ent->TrailShader->shaderInit(ent->TrailShader);
		ent->TrailPosAttrib = glGetAttribLocation(ent->TrailShader->shaderProgram, "position");

		glGenBuffers(1, &ent->trailsVbo);
		glBindBuffer(GL_ARRAY_BUFFER, ent->trailsVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TrailPoint) * 5000, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		TrailPoint* tmp = (TrailPoint*)realloc(ent->trp, sizeof(TrailPoint) * ent->trailSize);
		if (!tmp) return;
		ent->trp = tmp;
	}

}
void draw(Entity* ent) {
	if (!ent->isEntInit || !ent->isShaderInit) {
		printf("you cant draw entity %s because does not init\n", ent->entityName);
		return;
	}

	glUseProgram(ent->shader->shaderProgram);
	glPushMatrix();

	glTranslatef(ent->posx, ent->posy, ent->posz);
	glRotatef(ent->rotx, 1, 0, 0);
	glRotatef(ent->roty, 0, 1, 0);
	glRotatef(ent->rotz, 0, 0, 1);
	glScalef(ent->sizex, ent->sizey, ent->sizez);

	Component* cmp = ((Component*)ent->component->getByIndex(ent->component, 0));
	GLuint* vbo = cmp->LocData->getByIndex(cmp->LocData, 0);
	Mesh* mesh = (Mesh*)cmp->InData;
	GLuint a = (*vbo);
	cmp->Bind(cmp);

	glEnableVertexAttribArray(ent->posAttrib);
	glVertexAttribPointer(ent->posAttrib, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(ent->normAttrib);
	glVertexAttribPointer(ent->normAttrib, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(ent->texCrdAttrib);
	glVertexAttribPointer(ent->texCrdAttrib, 2, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)(6 * sizeof(float)));


	if (ent->tex != NULL)
		ent->tex->bindTexture(ent->tex);
	glDrawElements(GL_TRIANGLES, ((Mesh*)cmp->InData)->indexCount, GL_UNSIGNED_BYTE, 0);

	glDisableVertexAttribArray(ent->texCrdAttrib);
	glDisableVertexAttribArray(ent->normAttrib);
	glDisableVertexAttribArray(ent->posAttrib);
	cmp->UnBind(cmp);

	if (ent->tex != NULL)
		ent->tex->unbindTexture(ent->tex);

	glPopMatrix();
	glUseProgram(0);


	//trailblock
	if (ent->trailSize > 0) {

		if (ent->trpSize < ent->trailSize) {
			ent->trp[ent->trpSize].x = ent->posx*1.03;
			ent->trp[ent->trpSize].y = ent->posy*1.03;
			ent->trpSize++;

		}
		else {
			for (int i = 0; i < ent->trpSize - 1; i++) {
				ent->trp[i] = ent->trp[i + 1];
			}
			ent->trp[ent->trpSize - 1].x = ent->posx * 1.03;
			ent->trp[ent->trpSize - 1].y = ent->posy * 1.03;
		}

		glBindBuffer(GL_ARRAY_BUFFER, ent->trailsVbo); 
		glBufferSubData(GL_ARRAY_BUFFER, 0, ent->trpSize * sizeof(TrailPoint), ent->trp); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUseProgram(ent->TrailShader->shaderProgram);

		glBindBuffer(GL_ARRAY_BUFFER, ent->trailsVbo);
		glEnableVertexAttribArray(ent->TrailPosAttrib);
		glVertexAttribPointer(ent->TrailPosAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(TrailPoint), 0);

		glDrawArrays(GL_LINE_STRIP, 0, ent->trpSize);

		glDisableVertexAttribArray(ent->TrailPosAttrib);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}

}

void setVertexShader(Entity* ent, const char* src) {
	char* tmp = (char*)malloc(sizeof(char) * (strlen(src) + 1));
	if (!tmp) return;

	memcpy(tmp, src, (strlen(src) + 1));
	ent->vertexShader = tmp;
}

void setFragmentShader(Entity* ent, const char* src) {
	char* tmp = (char*)malloc(sizeof(char) * (strlen(src) + 1));
	if (!tmp) return;

	memcpy(tmp, src, (strlen(src) + 1));
	ent->fragmentShader = tmp;
}

void setTrail(Entity* ent, size_t pointCount) {
	ent->trailSize = pointCount;
}

void addComponent(Entity* ent, Component* cmp) {
	ent->component->addToDataArr(ent->component, cmp);
}

Entity* Entity_new(const char* name) {
	Entity* ent = (Entity*)malloc(sizeof(Entity));
	if (!ent) return NULL;
	ent->posx = 0;
	ent->posy = 0;
	ent->posz = 0;

	ent->rotx = 0;
	ent->roty = 0;
	ent->rotz = 0;

	ent->sizex = 1;
	ent->sizey = 1;
	ent->sizez = 1;

	ent->tex = NULL;
	ent->shader = NULL;

	ent->draw = draw;
	ent->addComponent = addComponent;
	ent->setPosition = setPosition;
	ent->setRotation = setRotation;
	ent->setScale = setScale;
	ent->entityInit = entityInit;
	ent->setVertexShader = setVertexShader;
	ent->setFragmentShader = setFragmentShader;
	ent->setTrail = setTrail;
	ent->fragmentShader = NULL;
	ent->vertexShader = NULL;
	ent->trp = NULL;
	ent->trpSize = 0;

	ent->setTexture = setTexture;

	ent->posAttrib = 0;
	ent->normAttrib = 0;
	ent->texCrdAttrib = 0;
	ent->textureAttrib = 0;
	ent->TrailPosAttrib = 0;
	ent->trailSize = 0;

	
	ent->component = dataArr_new();

	ent->isEntInit = 0;
	ent->isShaderInit = 0;

	size_t iter = 0;
	ent->entityName = (char*)malloc(sizeof(char) * (strlen(name)+1));
	if (!ent->entityName) return NULL;
	for (; iter < strlen(name); iter += 1) {
		ent->entityName[iter] = name[iter];
	}
	ent->entityName[iter] = '\0';
	return ent;
}

void Entity_delete(Entity* ent) {
	ent->draw = NULL;
	ent->setPosition = NULL;
	ent->setRotation = NULL;
	ent->setScale = NULL;
	ent->entityInit = NULL;
	ent->getEntityName = NULL;
	ent->setTrail = NULL;
	ent->TrailShader = NULL;

	if(ent->shader)
		Shader_delete(ent->shader);
	free(ent->entityName);
	ent->entityName = NULL;

	free(ent);
	ent = NULL;
}