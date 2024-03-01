#include "include/Rectangle.h"

static const int X = 0;
static const int Y = 1;

Rectangle::Rectangle(const float x, const float y, const float width, const float height, const glm::vec4& rgba)
	: Shape2D()
	, mWidth(width)
	, mHeight(height)
	, mCenter({ x, y })
{
	mRGBA = rgba;
}

Rectangle::Rectangle(const Rectangle& other)
	: Shape2D()
	, mWidth(other.mWidth)
	, mHeight(other.mHeight)
	, mCenter({ other.mCenter.vec2[X], other.mCenter.vec2[Y] })
{
	mRGBA = other.mRGBA;
}

bool Rectangle::Contain(const point2D_t& point) const
{
	return (point.vec2[X] > mCenter.vec2[X] - mWidth / 2.f &&
			point.vec2[X] <= mCenter.vec2[X] + mWidth / 2.f &&
			point.vec2[Y] > mCenter.vec2[Y] - mHeight / 2.f &&
			point.vec2[Y] <= mCenter.vec2[Y] + mHeight / 2.f);
}

bool Rectangle::Intersect(const Rectangle& other) const
{
	return !(other.mCenter.vec2[X] - other.mWidth / 2.f > mCenter.vec2[X] + mWidth / 2.f ||
		other.mCenter.vec2[X] + other.mWidth / 2.f < mCenter.vec2[X] - mWidth / 2.f ||
		other.mCenter.vec2[Y] - other.mHeight / 2.f > mCenter.vec2[Y] + mHeight / 2.f ||
		other.mCenter.vec2[Y] + other.mHeight / 2.f < mCenter.vec2[Y] - mHeight / 2.f);
}

float Rectangle::GetX() const
{
	return mCenter.vec2[X];
}

float Rectangle::GetY() const
{
	return mCenter.vec2[Y];
}

float Rectangle::GetWidth() const
{
	return mWidth;
}

float Rectangle::GetHeight() const
{
	return mHeight;
}

void Rectangle::CheckBoundary(Particle2D& particle)
{
	// Check boundary
	float halfWidth = mWidth / 2.f;
	float halfHeight = mHeight / 2.f;

	point2D_t pos = particle.GetCoords();
	point2D_t velocity = particle.GetVelocity();
	const float radius = particle.GetRadius();


	if (abs(mCenter.vec2[X] - halfWidth - pos.vec2[X]) <= radius && velocity.vec2[X] < 0.f) {
		velocity.vec2[X] = -velocity.vec2[X];
	}

	if (abs(mCenter.vec2[Y] + halfHeight - pos.vec2[Y]) <= radius && velocity.vec2[Y] > 0.f) {
		velocity.vec2[Y] = -velocity.vec2[Y];
	}

	if (abs(mCenter.vec2[X] + halfWidth - pos.vec2[X]) <= radius && velocity.vec2[X] > 0.f) {
		velocity.vec2[X] = -velocity.vec2[X];
	}

	if (abs(mCenter.vec2[Y] - halfHeight - pos.vec2[Y]) <= radius && velocity.vec2[Y] < 0.f) {
		velocity.vec2[Y] = -velocity.vec2[Y];
	}

	particle.SetVelocity(velocity);
}

void Rectangle::Draw(Graphic* graphic)
{
	SimpleShader* shader = graphic->GetShader();
	shader->UseShader();
	glm::mat4 model(1.f);
	
	model = glm::translate(model, glm::vec3(mCenter.vec2[X], mCenter.vec2[Y], 0.f));
	model = glm::scale(model, glm::vec3((0.f, 0.f, mWidth / 2.f)));
	

	glUniformMatrix4fv(shader->GetUniformWorld(), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->GetUniformProejction(), 1, GL_FALSE, glm::value_ptr(graphic->GetProjection()));
	GLfloat rgba[4] = { mRGBA.x, mRGBA.y ,mRGBA.z ,mRGBA.w };
	glUniform4fv(shader->GetUniformRGBA(), 1, rgba);

	glBindVertexArray(graphic->GetRectangleVAO());
	
	glDrawArrays(GL_LINES, 0, graphic->GetRectangleVertexNum());
	glBindVertexArray(0);
	glUseProgram(0);
}

void Rectangle::TestDraw()
{
	glColor3f(0.f, 1.0f, 0.f);
	glPointSize(15.f);

	glBegin(GL_LINE_LOOP);

	glVertex2f((mCenter.vec2[X] - (mWidth / 2.f)) / HALF_SIMUL_WIDTH, (mCenter.vec2[Y] + (mHeight / 2.f)) / HALF_SIMUL_HEIGHT);
	glVertex2f((mCenter.vec2[X] - (mWidth / 2.f)) / HALF_SIMUL_WIDTH, (mCenter.vec2[Y] - (mHeight / 2.f)) / HALF_SIMUL_HEIGHT);
	glVertex2f((mCenter.vec2[X] + (mWidth / 2.f)) / HALF_SIMUL_WIDTH, (mCenter.vec2[Y] - (mHeight / 2.f)) / HALF_SIMUL_HEIGHT);
	glVertex2f((mCenter.vec2[X] + (mWidth / 2.f)) / HALF_SIMUL_WIDTH, (mCenter.vec2[Y] + (mHeight / 2.f)) / HALF_SIMUL_HEIGHT);

	glEnd();
	glFinish();
}
