#pragma once

#include "Common.h"
#include "Shape2D.h"
#include "Point2D.h"


class Particle2D : public Shape2D {
	static const int step = 50;
	static const float angle;

public:
	Particle2D() = default;
	Particle2D(float x, float y, float radius, 
				float xVelocity, float yVelocity,
				const glm::vec4& rgba);
	Particle2D(const Particle2D& other);
	Particle2D& operator= (const Particle2D& other);
	virtual ~Particle2D() = default;

	point2D_t GetCoords() const { return mCenter; }
	void SetCoords(const float x, const float y) { 
		mCenter.vec2[0] = x; 
		mCenter.vec2[1] = y; }

	void SetCoords(const point2D_t& point) { 
		mCenter.vec2[0] = point.vec2[0]; 
		mCenter.vec2[1] = point.vec2[1]; }

	float GetRadius() const { return mRadius; }

	point2D_t GetVelocity() const { return mVelocity; }
	void SetVelocity(const point2D_t& velocity) {
		mVelocity.vec2[0] = velocity.vec2[0];
		mVelocity.vec2[1] = velocity.vec2[1];
	}

	void Update(const GLfloat delta);

	virtual void Draw(Graphic* graphic) override;

	bool GetCollided() const { return mCollided; }
	void SetCollided(const bool isCollided);

	bool CheckCollision(Particle2D& other);


private:
	point2D_t mCenter;
	float mRadius;
	point2D_t mVelocity;
	bool mCollided;
};