#include "Render.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include "GL/glew.h"

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
    if (!(render->shad && render->mesh)) {
        render->mesh == NULL ?
            printf("Render Object %s does not init because of mesh", render->name) :
            printf("Render Object %s does not init because of shader", render->name);
    };
    render->isReady = (render->tex == NULL) ? 1 : 2;
}

void RenderObjectRend(Render* render) {
    glBegin(GL_TRIANGLES);

    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0);

    glVertex3f(-0.5f, -0.5f, .0);
    glVertex3f(0.5f, -0.5f, .0);
    glVertex3f(0.0f, 0.5f, .0);

    glEnd();
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
    render->isReady = 0;
    return render;
}
