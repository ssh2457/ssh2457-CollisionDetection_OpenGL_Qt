#include "include/Particle2D.h"

const static int X = 0;
const static int Y = 1;

Particle2D::Particle2D(float x, float y, float radius,
	float xVelocity, float yVelocity,
	const glm::vec4& rgba)
	: mCenter({ x, y })
	, mRadius(radius)
	, mVelocity({ xVelocity, yVelocity })
	, mCollided(false)
{
	mRGBA = rgba;
}

Particle2D::Particle2D(const Particle2D& other)
	: Shape2D()
	, mCenter({ other.mCenter.vec2[X], other.mCenter.vec2[Y] })
	, mRadius(other.mRadius)
	, mVelocity({ other.mVelocity.vec2[X], other.mVelocity.vec2[Y] })
	, mCollided(false)
{
	mRGBA = other.mRGBA;
}

Particle2D& Particle2D::operator=(const Particle2D& other)
{
	mCenter.vec2[X] = other.mCenter.vec2[X];
	mCenter.vec2[Y] = other.mCenter.vec2[Y];

	mRadius = other.mRadius;

	mVelocity.vec2[X] = other.mVelocity.vec2[X];
	mVelocity.vec2[Y] = other.mVelocity.vec2[Y];

	mRGBA = other.mRGBA;

	mCollided = false;

	return *this;
}

void Particle2D::Update(const GLfloat delta)
{
	mCenter.vec2[X] = mCenter.vec2[X] + mVelocity.vec2[X] * delta;
	mCenter.vec2[Y] = mCenter.vec2[Y] + mVelocity.vec2[Y] * delta;
}

void Particle2D::Draw(Graphic* graphic)
{
	SimpleShader* shader = graphic->GetShader();
	shader->UseShader();
	glm::mat4 model(1.f);
	/*
	model = glm::translate(model, glm::vec3(mCenter.vec2[X] / HALF_SIMUL_WIDTH, mCenter.vec2[Y]/ HALF_SIMUL_HEIGHT, 0.f));
	model = glm::scale(model, glm::vec3(mRadius / HALF_SIMUL_WIDTH, mRadius / HALF_SIMUL_HEIGHT, 1.f));
	*/

	model = glm::translate(model, glm::vec3(mCenter.vec2[X], mCenter.vec2[Y], 0.f));
	model = glm::scale(model, glm::vec3(mRadius, mRadius, 1.f));

	glUniformMatrix4fv(shader->GetUniformWorld(), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->GetUniformProejction(), 1, GL_FALSE, glm::value_ptr(graphic->GetProjection()));
	GLfloat rgba[4] = {mRGBA.x, mRGBA.y ,mRGBA.z ,mRGBA.w };
	glUniform4fv(shader->GetUniformRGBA(), 1, rgba);

	glBindVertexArray(graphic->GetCircleVAO());
	glDrawArrays(GL_TRIANGLES, 0, graphic->GetCircleVertexNum());
	glBindVertexArray(0);
	glUseProgram(0);

	/*
	float xPos = 0.f;
	float yPos = 0.f;

	float xCoord = mCenter.vec2[X] / HALF_SIMUL_WIDTH;
	float yCoord = mCenter.vec2[Y] / HALF_SIMUL_HEIGHT;

	float prevX = xPos;
	float prevY = yPos;

	if (mCollided) {
		for (int i = 0; i <= steps; ++i) {
			float newX = mRadius / HALF_SIMUL_WIDTH * glm::sin(angle * i);
			float newY = -mRadius / HALF_SIMUL_HEIGHT * glm::cos(angle * i);

			glBegin(GL_TRIANGLES);
			glColor3f(1.f, 1.f, 0.f);

			glVertex3f(xCoord, yCoord, 0.f);
			glVertex3f(xCoord + prevX, yCoord + prevY, 0.f);
			glVertex3f(xCoord + newX, yCoord + newY, 0.f);

			glEnd();
			glFinish();

			prevX = newX;
			prevY = newY;
		}
	} else {
		for (int i = 0; i <= steps; ++i) {
			float newX = mRadius / HALF_SIMUL_WIDTH * glm::sin(angle * i);
			float newY = -mRadius / HALF_SIMUL_HEIGHT * glm::cos(angle * i);

			glBegin(GL_TRIANGLES);
			glColor3f(1.f, 0.f, 0.f);

			glVertex3f(xCoord, yCoord, 0.f);
			glVertex3f(xCoord + prevX, yCoord + prevY, 0.f);
			glVertex3f(xCoord + newX, yCoord + newY, 0.f);

			glEnd();
			glFinish();

			prevX = newX;
			prevY = newY;
		}
	}
	*/
}

void Particle2D::SetCollided(const bool isCollided)
{
	mCollided = isCollided;
}

bool Particle2D::CheckCollision(Particle2D& other)
{
	point2D_t p1_coords = mCenter;
	point2D_t p1_velocity = mVelocity;
	const float p1_radius = mRadius;


	point2D_t p2_coords = other.GetCoords();
	point2D_t p2_velocity = other.GetVelocity();
	const float p2_radius = other.GetRadius();

	float x_sq = (p2_coords.vec2[X] - p1_coords.vec2[X]) * (p2_coords.vec2[X] - p1_coords.vec2[X]);
	float y_sq = (p2_coords.vec2[Y] - p1_coords.vec2[Y]) * (p2_coords.vec2[Y] - p1_coords.vec2[Y]);

	float length_sq = (p1_radius + p2_radius) * (p1_radius + p2_radius);
	//&& !(particle1.GetCollided() || particle2.GetCollided())
	if (x_sq + y_sq <= length_sq) {

		const float m1 = p1_radius;
		const float m2 = p2_radius;

		point2D_t unitNormal = { p2_coords.vec2[X] - p1_coords.vec2[X], p2_coords.vec2[Y] - p1_coords.vec2[Y] };

		float diff_x_sq = (p2_coords.vec2[X] - p1_coords.vec2[X]) * (p2_coords.vec2[X] - p1_coords.vec2[X]);
		float diff_y_sq = (p2_coords.vec2[Y] - p1_coords.vec2[Y]) * (p2_coords.vec2[Y] - p1_coords.vec2[Y]);
		float magnitude = sqrtf(diff_x_sq + diff_y_sq);
		unitNormal.vec2[X] /= magnitude;
		unitNormal.vec2[Y] /= magnitude;

		point2D_t rel_velocity = { p2_velocity.vec2[X] - p1_velocity.vec2[X], p2_velocity.vec2[Y] - p1_velocity.vec2[Y] };

		float dot = rel_velocity.vec2[X] * unitNormal.vec2[X] + rel_velocity.vec2[Y] * unitNormal.vec2[Y];
		if (dot < 0.f) {
			point2D_t unitTangent = { -unitNormal.vec2[Y], unitNormal.vec2[X] };

			float v1n = unitNormal.vec2[X] * p1_velocity.vec2[X] + unitNormal.vec2[Y] * p1_velocity.vec2[Y];
			float v1t = unitTangent.vec2[X] * p1_velocity.vec2[X] + unitTangent.vec2[Y] * p1_velocity.vec2[Y];

			float v2n = unitNormal.vec2[X] * p2_velocity.vec2[X] + unitNormal.vec2[Y] * p2_velocity.vec2[Y];
			float v2t = unitTangent.vec2[X] * p2_velocity.vec2[X] + unitTangent.vec2[Y] * p2_velocity.vec2[Y];


			float v1t_prime = v1t;
			float v2t_prime = v2t;

			float v1n_prime = (v1n * (m1 - m2) + 2.f * m2 * v2n) / (m1 + m2);
			float v2n_prime = (v2n * (m2 - m1) + 2.f * m1 * v1n) / (m1 + m2);

			point2D_t v1n_prime_vec = { v1n_prime * unitNormal.vec2[X], v1n_prime * unitNormal.vec2[Y] };
			point2D_t v1t_prime_vec = { v1t_prime * unitTangent.vec2[X], v1t_prime * unitTangent.vec2[Y] };

			point2D_t v2n_prime_vec = { v2n_prime * unitNormal.vec2[X], v2n_prime * unitNormal.vec2[Y] };
			point2D_t v2t_prime_vec = { v2t_prime * unitTangent.vec2[X], v2t_prime * unitTangent.vec2[Y] };


			p1_velocity = { v1n_prime_vec.vec2[X] + v1t_prime_vec.vec2[X], v1n_prime_vec.vec2[Y] + v1t_prime_vec.vec2[Y] };
			p2_velocity = { v2n_prime_vec.vec2[X] + v2t_prime_vec.vec2[X], v2n_prime_vec.vec2[Y] + v2t_prime_vec.vec2[Y] };

			//particle1.SetVelocity(p1_velocity);
			SetVelocity(p1_velocity);
			other.SetVelocity(p2_velocity);

			SetCollided(true);
			other.SetCollided(true);

			return true;
		}
	}

	SetCollided(false);
	other.SetCollided(false);

	return false;
}
