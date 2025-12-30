#include "DynamicString.h"
#include "PythonHeader.h"
#include "Python.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void PythonObjectScriptInit(PythonScript* ps, const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return;

    int c = 0;
    dstr* str = dstr_new();
    while ((c = fgetc(f)) != EOF) {
        str->addSymbol(str, c);
    }
    str->addSymbol(str, '\0');
    ps->source = str->getString(str);
    fclose(f);
}

void PythonObjectScriptBind(PythonScript* ps) {
    if (!ps->source) return;
    Py_Initialize();
    PyRun_SimpleString(ps->source);
    Py_Finalize();
}

PythonScript* PythonScript_new(Entity* ent, ScriptType purpose, const char* name) {
    if (!name) return NULL;
    PythonScript* ps = (PythonScript*)malloc(sizeof(PythonScript));
    if (!ps) return NULL;

    ps->scriptName = NULL;
    ps->scriptName = _strdup(name);
    if (!ps->scriptName) return NULL;
    ps->source = NULL;

    ps->scriptInit = PythonObjectScriptInit;
    ps->scriptBind = PythonObjectScriptBind;
    
    ps->st = purpose;
    ps->ent = ent;
    return ps;

}