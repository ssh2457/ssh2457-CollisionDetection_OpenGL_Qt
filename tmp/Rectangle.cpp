#include "include/Rectangle.h"

static const int X = 0;
static const int Y = 1;

Rectangle::Rectangle(const point2D_t& p1, const point2D_t& p2)
	: Shape2D()
	, mP1({ p1.vec2[X], p1.vec2[Y] })
	, mP2({ p2.vec2[X], p2.vec2[Y] })
{

}

void Rectangle::Draw()
{
	glColor3f(0.f, 1.0f, 1.f);
	glPointSize(10.f);

	glBegin(GL_LINE_LOOP);

	glVertex2f(mP1.vec2[X] / HALF_SIMUL_WIDTH, mP1.vec2[Y] / HALF_SIMUL_HEIGHT);
	glVertex2f(mP1.vec2[X] / HALF_SIMUL_WIDTH, mP2.vec2[Y] / HALF_SIMUL_HEIGHT);
	glVertex2f(mP2.vec2[X] / HALF_SIMUL_WIDTH, mP2.vec2[Y] / HALF_SIMUL_HEIGHT);
	glVertex2f(mP2.vec2[X] / HALF_SIMUL_WIDTH, mP1.vec2[Y] / HALF_SIMUL_HEIGHT);

	glEnd();
	glFinish();
}
