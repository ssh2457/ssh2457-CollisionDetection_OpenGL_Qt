#pragma once

#include "Common.h"
#include "Point2D.h"
#include "Particle2D.h"
#include "Shape2D.h"

class Rectangle : public Shape2D {
public:
	Rectangle(const float x, const float y, const float width, const float height, const glm::vec4& rgba);
	Rectangle(const Rectangle& other);
	virtual ~Rectangle() = default;

	bool Contain(const point2D_t& point) const;
	bool Intersect(const Rectangle& other) const;


	float GetX() const;
	float GetY() const;
	float GetWidth() const;
	float GetHeight() const;

	void CheckBoundary(Particle2D& particle);

	virtual void Draw(Graphic* graphic) override;
	void TestDraw();
protected:
	point2D_t mCenter;
	float mWidth;
	float mHeight;
};