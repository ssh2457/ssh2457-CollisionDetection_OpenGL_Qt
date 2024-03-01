#include "include/SimpleShader.h"


SimpleShader::SimpleShader()
	: mShaderID(0)
	, mUniformWorld(0)
	, mUniformProjection(0)
	, mUniformRGBA(0)
{

}

SimpleShader::~SimpleShader() {
	ClearShader();
}

void SimpleShader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
	CompileShader(vertexCode, fragmentCode);
}

void SimpleShader::CreateFromFiles(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode = ReadFile(vertexPath);
	std::string fragmentCode = ReadFile(fragmentPath);

	CreateFromString(vertexCode.c_str(), fragmentCode.c_str());
}

std::string SimpleShader::ReadFile(const char* filePath) {
	std::string content = "";
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		SPDLOG_ERROR("Failed to read {}! File doesn't exist", filePath);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + '\n');
	}
	fileStream.close();

	return content;
}

void SimpleShader::Validate()
{
	GLint result = 0;
	GLchar eLog[512] = { 0, };

	glValidateProgram(mShaderID);
	glGetProgramiv(mShaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(mShaderID, sizeof(eLog), NULL, eLog);
		SPDLOG_ERROR("Error validating program: {}", eLog);
		return;
	}
}

void SimpleShader::UseShader() {
	glUseProgram(mShaderID);
}

void SimpleShader::ClearShader() {
	if (mShaderID != 0) {
		glDeleteProgram(mShaderID);
		mShaderID = 0;
	}
}

void SimpleShader::CompileShader(const char* vertexCode, const char* fragmentCode) {
	mShaderID = glCreateProgram();
	if (!mShaderID) {
		SPDLOG_ERROR("Error creating SimpleShader program!");
		return;
	}
	SPDLOG_INFO("Shader ID: {}", mShaderID);

	AddShader(mShaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(mShaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[512] = { 0, };

	glLinkProgram(mShaderID);
	glGetProgramiv(mShaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(mShaderID, sizeof(eLog), NULL, eLog);
		SPDLOG_ERROR("Error linking program: {}", eLog);
		return;
	}

	mUniformWorld = glGetUniformLocation(mShaderID, "world");
	mUniformProjection = glGetUniformLocation(mShaderID, "projection");
	mUniformRGBA = glGetUniformLocation(mShaderID, "rgba");
}

void SimpleShader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theSimpleShader = glCreateShader(shaderType);

	const GLchar* const* theCode = &shaderCode;
	GLint codeLength = strlen(shaderCode);

	glShaderSource(theSimpleShader, 1, theCode, &codeLength);
	glCompileShader(theSimpleShader);

	GLint result = 0;
	GLchar eLog[512] = { 0, };

	glGetShaderiv(theSimpleShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theSimpleShader, sizeof(eLog), NULL, eLog);
		SPDLOG_ERROR("Error compiling the {} SimpleShader: {}", shaderType, eLog);
		//printf("Error compiling the %d SimpleShader: %s\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theSimpleShader);
}