#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/glm.hpp>

#include "Common.h"
#include "Shape2D.h"
#include "Point2D.h"

class Rectangle : public Shape2D {
public:
	Rectangle() = delete;
	Rectangle(const point2D_t& p1, const point2D_t& p2);
	virtual ~Rectangle() = default;

	point2D_t GetLeftUpVertex() const { return mLeftUpVertex; }
	point2D_t GetRightBottomVertex() const { return mRightBottomVertex; }

	virtual void Draw() override;

private:
	point2D_t mLeftUpVertex;
	point2D_t mRightBottomVertex;
};