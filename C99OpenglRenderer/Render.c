#include "Render.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include "GL/glew.h"
#include "dataArray.h"

void RenderObjectSetMesh(Render* render, Mesh* msh) {
    if (!msh) {
        printf("Render Object %s does not get your mesh because input mesh refer to NULL\n",
            render->name);
        exit(-1);
    }

    if (!msh->isReady) {
        printf("Render Object %s does not get your mesh because mesh %s does not init\n",
            render->name, msh->meshName);
        exit(-1);
    }
    render->mesh = msh;
}

void RenderObjectSetShader(Render* render, Shader* shd) {
    if (!shd) {
        printf("Render Object %s does not get your mesh because input mesh refer to NULL\n",
            render->name);
        exit(-1);
    }

    if (!shd->isReady) {
        printf("Render Object %s does not get your mesh because %s does not init\n",
            render->name, shd->shaderName);
        exit(-1);
    }
    render->shad = shd;
}

void RenderObjectSetTexture(Render* render, Texture* tex) {
    if (!tex) {
        printf("Render Object %s does not get your tex because input tex refer to NULL\n",
            render->name);
        exit(-1);
    }

    if (!tex->isReady) {
        printf("Render Object %s does not get your texture because %s does not init\n",
            render->name, tex->textureName);
        exit(-1);
    }
    render->tex = tex;
}

void RenderObjectInit(Render* render) {
    GLuint* posAttrib = (GLuint*)malloc(sizeof(GLuint)); if (!posAttrib) return;
    GLuint* normAttrib = (GLuint*)malloc(sizeof(GLuint)); if (!normAttrib) return;
    GLuint* textureAttrib = (GLuint*)malloc(sizeof(GLuint)); if (!textureAttrib) return;
    GLuint* texCrdAttrib = (GLuint*)malloc(sizeof(GLuint)); if (!texCrdAttrib) return;
    (*posAttrib) = glGetAttribLocation(render->shad->shaderProgram, "position");
    (*normAttrib) = glGetAttribLocation(render->shad->shaderProgram, "norm");
    (*textureAttrib) = glGetUniformLocation(render->shad->shaderProgram, "u_tex");
    (*texCrdAttrib) = glGetAttribLocation(render->shad->shaderProgram, "texcoord");

    render->mesh->meshBind(render->mesh);

    glEnableVertexAttribArray(*posAttrib);
    glVertexAttribPointer(*posAttrib, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(*normAttrib);
    glVertexAttribPointer(*normAttrib, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(*texCrdAttrib);
    glVertexAttribPointer(*texCrdAttrib, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)(6 * sizeof(float)));

    render->mesh->unBindMesh(render->mesh);

    render->unData = dataArr_new();
    render->unData->addToDataArr(render->unData, posAttrib);
    render->unData->addToDataArr(render->unData, normAttrib);
    render->unData->addToDataArr(render->unData, textureAttrib);
    render->unData->addToDataArr(render->unData, texCrdAttrib);

    render->isReady = (render->tex == NULL) ? 1 : 2;
}

void RenderObjectSetPostion(Render* render, float x, float y, float z) {
    render->pos[0] = x;
    render->pos[1] = y;
    render->pos[2] = z;
}
void RenderObjectSetRotation(Render* render, float x, float y, float z) {
    render->rot[0] = x;
    render->rot[1] = y;
    render->rot[2] = z;
}
void RenderObjectSetScale(Render* render, float x, float y, float z) {
    render->size[0] = x;
    render->size[1] = y;
    render->size[2] = z;
}

void RenderObjectRend(Render* render) {

    render->shad->shaderBind(render->shad);
    glPushMatrix();

    glTranslatef(render->pos[0], render->pos[1], render->pos[2]);
    glRotatef(render->rot[0], 1, 0, 0);
    glRotatef(render->rot[1], 0, 1, 0);
    glRotatef(render->rot[2], 0, 0, 1);
    glScalef(render->size[0], render->size[1], render->size[2]);

    render->mesh->meshBind(render->mesh);

    GLuint* posAttrib = (GLuint*)render->unData->getByIndex(render->unData, 0);
    GLuint* normAttrib = (GLuint*)render->unData->getByIndex(render->unData, 1);
    GLuint* textureAttrib = (GLuint*)render->unData->getByIndex(render->unData, 2);
    GLuint* texCrdAttrib = (GLuint*)render->unData->getByIndex(render->unData, 3);
    glEnableVertexAttribArray(*posAttrib);
    glEnableVertexAttribArray(*normAttrib);
    glEnableVertexAttribArray(*texCrdAttrib);

    render->tex->textureBind(render->tex);

    GLint eboBind = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eboBind);

    glDrawElements(GL_TRIANGLES, render->mesh->indexCount, GL_UNSIGNED_BYTE, 0);

    glDisableVertexAttribArray(*posAttrib);
    glDisableVertexAttribArray(*normAttrib);
    glDisableVertexAttribArray(*texCrdAttrib);

    render->tex->textureUnBind(render->tex);
    render->mesh->unBindMesh(render->mesh);

    glPopMatrix();
    render->shad->shaderUnBind(render->shad);
}

//isReady 0 - not init
//isReady 1 - init without texture
//isReady 2 - init with texture
Render* Render_new(char* name) {
    if (!name) return NULL;
    Render* render = (Render*)malloc(sizeof(Render));
    if (!render) return NULL;

    render->mesh = NULL;
    render->shad = NULL;
    render->tex = NULL;

    render->name = NULL;
    render->name = _strdup(name);
    if (!render->name) { free(render); return NULL; }
    render->setMesh = RenderObjectSetMesh;
    render->setShader = RenderObjectSetShader;
    render->setTexture = RenderObjectSetTexture;
    render->renderInit = RenderObjectInit;
    render->rend = RenderObjectRend;

    render->unSize = 0;
    render->unData = NULL;

    render->pos[0] = 0.0f;
    render->pos[1] = 0.0f;
    render->pos[2] = 0.0f;
    render->rot[0] = 0.0f;
    render->rot[1] = 0.0f;
    render->rot[2] = 0.0f;
    render->size[0] = 1.0f;
    render->size[1] = 1.0f;
    render->size[2] = 1.0f;

    render->isReady = 0;
    return render;
}
