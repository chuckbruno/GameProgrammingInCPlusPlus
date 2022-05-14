
#include "Shader.h"
#include "Texture.h"
#include <SDL2/SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader()
	:mShaderProgram(0)
	, mVertexShader(0)
	, mFragShader(0)
{

}


Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// compile vertex and pixel shaders
	if (!CompileShader(vertName,
		GL_VERTEX_SHADER,
		mVertexShader) ||
		!CompileShader(fragName,
			GL_FRAGMENT_SHADER,
			mFragShader))
	{
		return false;
	}

	// now create a shader program that links together the vertexs/frag shaders
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);
	// verify that the program linked successfully
	if (!IsValidProgram())
	{
		return false;
	}

	return true;
}

void Shader::Unload()
{
	// delete the program/shaders
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	// set this program as the active one
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	// find the uniform by this name
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// send the matrix data to the uniform
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	// open file
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// read all the text into a string
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// create a shader of the specified type
		outShader = glCreateShader(shaderType);
		// set the source characters and try to compile
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else 
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// query the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed: \n%s ", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;
	// query the link status
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}

