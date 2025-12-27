#ifndef SHADERCLASS_H
#define SHADERCLASS_H

typedef unsigned int uint;

typedef struct Shader {
	void (*setShaderName)(struct Shader*, const char*);
	void (*setVertexShader)(struct Shader*, const char*);
	void (*setFragmentShader)(struct Shader*, const char*);
	void (*shaderBind)(struct Shader*);
	void (*shaderUnBind)(struct Shader*);
	uint (*shaderInit)(struct Shader*);

	char* shaderName;
	char* vertexShaderSource;
	char* fragmentShaderSource;
	uint shaderProgram;
	char isReady;

} Shader;

Shader* Shader_new(const char* shaderName);
void Shader_delete(Shader* shd);

#endif