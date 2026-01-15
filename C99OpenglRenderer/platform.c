#include "platform.h"
#include <stdlib.h>
#include <stdio.h>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "EngineContext.h"

EngineCtx ectx = {0};
void(*displayFunc)(void) = NULL;
unsigned char* shouldClose_ = NULL;

//privateFunc
void onClose() {
    shouldClose_ = 1;
}

void keyboard(unsigned char key, int x, int y) {
    ectx.keys[key] = 1;
}

static void keyboard_up(unsigned char key, int x, int y) {
    ectx.keys[key] = 0;  // для release (glutKeyboardUpFunc)
}

static void timer_cb(int value) {
    ectx.dt += 1;
    glutTimerFunc(16, timer_cb, 0);  // ← перезапуск для следующего кадра!
}


void privateDisplayFunc() {

    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(displayFunc)
    displayFunc();

    glutSwapBuffers();
    glutPostRedisplay();
}

void PlatformInit(Platform* plt) {
    glewInit();
    glutDisplayFunc(privateDisplayFunc);
    if (plt->startFunc)
        plt->startFunc();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutTimerFunc(16, timer_cb, 0);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);  // optional
    glutCloseFunc(onClose);

}
void render(Platform* plt) {
    glutMainLoopEvent();
}
void setStartFunc(Platform* plt, void (*func)(void)) {
    plt->startFunc = func;
}
void setDisplayFunc(Platform* plt, void (*func)(void)) {
    displayFunc = func;
}
unsigned char shouldClose(Platform* plt) {
    return shouldClose_;
}

void createWindow(Platform* plt, const char* title, int w, int h) {
    glutInitWindowSize(w, h);
    glutCreateWindow(title);
}

EngineCtx* getEngineContext(Platform* plt) {
    return &ectx;
}

Platform* GlutPlatform_new(int argc, char** argv) {
    Platform* plt = (Platform*)malloc(sizeof(Platform));
    if (!plt) return NULL;
        
    glutInit(&argc, argv);
    glutInitContextVersion(2, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    plt->render = render;
    plt->PlatformInit = PlatformInit;
    plt->setStartFunc = setStartFunc;
    plt->setDisplayFunc = setDisplayFunc;

    plt->createWindow = createWindow;
    plt->shouldClose = shouldClose;

    plt->getEngineContext = getEngineContext;

    shouldClose_ = (unsigned char*)malloc(sizeof(char));
    shouldClose_ = 0;

    plt->startFunc = NULL;
    return plt;
}
