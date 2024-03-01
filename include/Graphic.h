#pragma once

#include <GL/glew.h>

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimpleShader.h"

class Graphic {
	friend std::unique_ptr<Graphic> std::make_unique<Graphic>();
	friend std::unique_ptr<Graphic>::deleter_type;

	static const int step;
	static const float angle;

public:
	Graphic(const Graphic& other) = delete;
	Graphic& operator=(const Graphic& other) = delete;

	static std::unique_ptr<Graphic> GetInstance();

	void Initialize(const GLint width, const GLint height);

	glm::mat4 GetProjection() const { return mProjection; }

	GLuint GetCircleVAO() const { return mVAOs[0]; }
	GLuint GetRectangleVAO() const { return mVAOs[1]; }

	SimpleShader* GetShader() const { return mShader; }
	GLuint GetCircleVertexNum() const { return mCircleVertexNum; }
	GLuint GetRectangleVertexNum() const { return mRectangleVertexNum; }

private:
	Graphic();
	virtual ~Graphic();

	static std::unique_ptr<Graphic> instance;

	GLuint mVAOs[2], mVBOs[2];
	SimpleShader* mShader;

	glm::mat4 mProjection;

	GLuint mCircleVertexNum;
	GLuint mRectangleVertexNum;

	void SetProjection(const GLint width, const GLint height);

	void CreateCircle();
	void CreateRectangle(const GLint width, const GLint height);
	void CreatePolygons(const GLint width, const GLint height);
};