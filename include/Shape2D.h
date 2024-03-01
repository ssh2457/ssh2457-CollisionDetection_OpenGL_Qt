#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphic.h"

class Shape2D {
public:
	Shape2D() = default;
	virtual ~Shape2D() = default;

	virtual void Draw(Graphic* graphic) = 0;
protected:
	glm::vec4 mRGBA;
};