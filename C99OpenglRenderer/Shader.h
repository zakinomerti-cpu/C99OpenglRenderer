#ifndef SHADERCLASS_H
#define SHADERCLASS_H

typedef struct Shader {
	char* shaderName;
	void (*setVertexShader)(struct Shader*, const char*);
	void (*setFragmentShader)(struct Shader*, const char*);
	void (*shaderBind)(struct Shader*);
	void (*shaderUnBind)(struct Shader*);
	unsigned int (*shaderInit)(struct Shader*);

	char* vertexShaderSource;
	char* fragmentShaderSource;
	unsigned int shaderProgram;
	char isReady;

} Shader;

Shader* Shader_new(const char* shaderName);
void Shader_delete(Shader*);

#endif