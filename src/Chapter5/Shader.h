#pragma once

#include <GL/glew.h>
#include <string>
#include "Math.h"


class Shader
{
public:
	Shader();
	~Shader();
	// load the vertex/fragment shaders with the given names
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	// set this as the active shader program
	void SetActive();
	// sets a matrix uniform
	void SetMatrixUniform(const char* name, const Matrix4& matrix);

private:
	// tries to compute the specified shader
	bool CompileShader(const std::string& fileName,
						GLenum shaderType,
						GLuint& outShader);
	// tests whether shader compiled successfully
	bool IsCompiled(GLuint shader);
	// tests whether vertex/fragment program link
	bool IsValidProgram();

private:
	// store the shader object IDs
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};