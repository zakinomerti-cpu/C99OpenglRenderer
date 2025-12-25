#include <Python.h>
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
	Entity* ent = Entity_new("13");
	ent->setMesh(ent, createCube());
	ent->entityInit(ent);
	scn->addEntity(scn, ent);
}

PyObject* PyPause(PyObject* self, PyObject* args) {

	PyObject* timePValue = PyTuple_GetItem(args, 0);
	long timeValue = PyLong_AsLong(timePValue);
	Sleep(timeValue);
	Py_RETURN_NONE;
}

PyObject* PyMoveCube(PyObject* self, PyObject* args) {

	PyObject* px = PyTuple_GetItem(args, 1);
	PyObject* py = PyTuple_GetItem(args, 2);
	PyObject* pz = PyTuple_GetItem(args, 3);
	PyObject* pname = PyTuple_GetItem(args, 0);

	long x = PyLong_AsLong(px);
	long y = PyLong_AsLong(py);
	long z = PyLong_AsLong(pz);
	const char* name = PyUnicode_AsUTF8(pname);

	scn->getEntity(scn, name)->setPosition(scn->getEntity(scn, name), 
		x, y, z);
	Py_RETURN_NONE;
}

PyObject* PyRotateCube(PyObject* self, PyObject* args) {

	PyObject* px = PyTuple_GetItem(args, 1);
	PyObject* py = PyTuple_GetItem(args, 2);
	PyObject* pz = PyTuple_GetItem(args, 3);
	PyObject* pname = PyTuple_GetItem(args, 0);

	long x = PyLong_AsLong(px);
	long y = PyLong_AsLong(py);
	long z = PyLong_AsLong(pz);
	const char* name = PyUnicode_AsUTF8(pname);

	scn->getEntity(scn, name)->setRotation(scn->getEntity(scn, name), x, y, z);
	Py_RETURN_NONE;
}

PyMethodDef def = {
	"PyCreateCube",
	PyMoveCube,
	METH_VARARGS,
	"some func"
};

PyMethodDef def2 = {
	"PyRotateCube",
	PyRotateCube,
	METH_VARARGS,
	"some func"
};

PyMethodDef def3 = {
	"PyPause",
	PyPause,
	METH_VARARGS,
	"some func"
};

DWORD WINAPI ThreadFunc(LPVOID lpParam) {
	Py_Initialize();
	PyObject* main = PyImport_AddModule("__main__");
	PyObject* globals = PyModule_GetDict(main);

	PyObject* PyPauseFunc = PyCFunction_New(&def3, NULL);
	PyObject* PyMoveCubeFunc = PyCFunction_New(&def, NULL);
	PyObject* PyRotateCubeFunc = PyCFunction_New(&def2, NULL);

	PyDict_SetItemString(globals, "PyPause", PyPauseFunc);
	PyDict_SetItemString(globals, "PyMoveCube", PyMoveCubeFunc);
	PyDict_SetItemString(globals, "PyRotateCube", PyRotateCubeFunc);

	FILE* f = fopen("script.py", "r");
	PyRun_SimpleFile(f, "sctipt.py");
	Py_Finalize();
	return 0;
}

int main(int argc, char** argv) {
	Platform* plt = GlutPlatform_new(argc, argv);
	plt->setDisplayFunc(plt, display);
	plt->setStartFunc(plt, init);

	plt->createWindow(plt, "window", 800, 600);
	plt->PlatformInit(plt);

	HANDLE hThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		ThreadFunc,
		NULL,
		0,
		NULL
	);

	while (!plt->shouldClose(plt)) {
		plt->render(plt);
	}

	return 0;

}