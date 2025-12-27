#include "Shader.h"
#include "GL/glew.h"
#include "stdlib.h"
#include "stdio.h"

#include "string.h"

GLuint compileShader(GLenum type, const char* src) {
	GLuint shader = glCreateShader(type);
	if (shader == 0) {
		printf("glCreateShader failed\n");
		return 0;
	}
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);

	int success;
	char log[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, log);
		printf("%s\n", log);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

unsigned char createShader(Shader* shader) {
	if (!shader->vertexShaderSource) return 0;
	if (!shader->fragmentShaderSource) return 0;
	GLuint vshader = compileShader(GL_VERTEX_SHADER,
		shader->vertexShaderSource);
	if (vshader == 0) return 0;
	GLuint fshader = compileShader(GL_FRAGMENT_SHADER,
		shader->fragmentShaderSource);
	if (fshader == 0) return 0;
	shader->shaderProgram = glCreateProgram();
	if (shader->shaderProgram == 0) return 0;
	glAttachShader(shader->shaderProgram, vshader);
	glAttachShader(shader->shaderProgram, fshader);
	glLinkProgram(shader->shaderProgram);

	int success;
	char log[1024];
	glGetProgramiv(shader->shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader->shaderProgram, 1024, NULL, log);
		printf("%s link error:\n%s\n", shader->shaderName, log);
		glDeleteProgram(shader->shaderProgram);
		shader->shaderProgram = 0;
		return 0;
	}

	glDeleteShader(vshader);
	glDeleteShader(fshader);
	return 1;
}

void shaderInit(Shader* self) {
	if (!self->vertexShaderSource) {
		printf("%s error!\n", self->shaderName);
		printf("vertex shader source does not initialized!!!\n");
		return;
	}

	if (!self->fragmentShaderSource) {
		printf("%s error!\n", self->shaderName);
		printf("fragment shader source does not initialized!!!\n");
		return;
	}

	if (!createShader(self)) {
		printf("%s error!\n", self->shaderName);
		printf("Shader compilation issue!\n");
		return;
	}
	self->isReady = 1;
}

void setFragSh(Shader* shader, const char* src) {

	if (!shader) return;
	if (!src) {
		printf("%s error!\n", shader->shaderName);
		printf("fragment shader source code refers to NULL ptr\n");
		return;
	}
	size_t len = strlen(src);
	shader->fragmentShaderSource = (char*)malloc(sizeof(char) * (len + 1));
	if (!shader->fragmentShaderSource) return;
	memcpy(shader->fragmentShaderSource, src, len + 1);
}

void setVertSh(Shader* shader, const char* src) {

	if (!shader) return;
	if (!src) {
		printf("%s error!\n", shader->shaderName);
		printf("vertex shader source code refers to NULL ptr\n");
		return;
	}
	size_t len = strlen(src);
	shader->vertexShaderSource = (char*)malloc(sizeof(char) * (len + 1));
	if (!shader->vertexShaderSource) return;
	memcpy(shader->vertexShaderSource, src, len + 1);
}

void ShaderObjectBind(Shader* shd) {
	if(shd->isReady)
		glUseProgram(shd->shaderProgram);
}

void ShaderObjectUnBind(Shader* shd) {
	if(shd->isReady)
		glUseProgram(0);
}

Shader* Shader_new(const char* shaderName) {
	if (shaderName == NULL) {
		printf("Shader must have own name to init!\n");
		printf("its necessary for debug...");
		return NULL;
	}
	Shader* shd = (Shader*)malloc(sizeof(Shader));
	if (!shd) return NULL;
	shd->vertexShaderSource = NULL;
	shd->fragmentShaderSource = NULL;
	shd->shaderProgram = 0;
	shd->isReady = 0;

	size_t nameStringLen = strlen(shaderName);
	shd->shaderName = (char*)malloc(sizeof(char) * (nameStringLen + 1));
	if (!shd->shaderName) {
		free(shd);
		return NULL;
	}
	memcpy(shd->shaderName, shaderName, nameStringLen + 1);

	shd->setVertexShader = setVertSh;
	shd->setFragmentShader = setFragSh;
	shd->shaderInit = shaderInit;

	shd->shaderBind = ShaderObjectBind;
	shd->shaderUnBind = ShaderObjectUnBind;

	return shd;
}

void Shader_delete(Shader* shd) {
	if (!shd) return;
	shd->setFragmentShader = NULL;
	shd->setVertexShader = NULL;
	shd->shaderInit = NULL;

	if (shd->shaderProgram != 0) {
		glDeleteProgram(shd->shaderProgram);
		shd->shaderProgram = 0;
	}

	free(shd->vertexShaderSource);
	shd->vertexShaderSource = NULL;
	free(shd->fragmentShaderSource);
	shd->fragmentShaderSource = NULL;
	free(shd->shaderName);
	shd->shaderName = NULL;
	free(shd);
	shd = NULL;
}
