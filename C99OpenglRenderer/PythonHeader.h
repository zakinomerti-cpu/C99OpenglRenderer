#ifndef PYTHONSCRIPTCLASS_H
#define PYTHONSCRIPTCLASS_H

typedef struct Entity Entity;

typedef enum {
	SCRIPT_START = 10010,
	SCRIPT_UPDATE = 10011,
} ScriptType;

typedef struct PythonScript {
	char* scriptName;
	void (*scriptInit)(struct PythonScript*, const char*);
	void (*scriptBind)(struct PythonScript*);
	const char* source;

	ScriptType st;
	Entity* ent;
} PythonScript;

PythonScript* PythonScript_new(Entity* ent, ScriptType purpose, char* name);

#endif