#pragma once

#include <gl/glew.h>
#include <spdlog/spdlog.h>

#include <string>
#include <fstream>

#include "Common.h"

class SimpleShader {
public:
	SimpleShader();
	virtual ~SimpleShader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	virtual void CreateFromFiles(const char* vertexPath, const char* fragmentPath);
	std::string ReadFile(const char* filePath);

	void Validate();

	void UseShader();
	virtual void ClearShader();

	GLuint GetUniformWorld() const { return mUniformWorld; }
	GLuint GetUniformProejction() const { return mUniformProjection; }
	GLuint GetUniformRGBA() const { return mUniformRGBA; }

protected:
	GLuint mShaderID, mUniformWorld, mUniformProjection, mUniformRGBA;
	virtual void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};