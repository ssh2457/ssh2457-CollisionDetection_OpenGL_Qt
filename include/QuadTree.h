#pragma once

#include <vector>
#include "Particle2D.h"

#include "Rectangle.h"

#include "Graphic.h"

class QuadTree : public Rectangle {
public:
	QuadTree(Rectangle& boundary, const size_t capacity);
	virtual ~QuadTree();

	bool Insert(const Particle2D& particle);
	void Query(std::vector<Particle2D*>& o_found, const Rectangle& range);

	void Print() const;

	void Update(Rectangle& boundary, const GLfloat delta, QuadTree* const root);

	void showParticle(Graphic* graphic);
	void ShowPartition(Graphic* graphic);

private:
	size_t mCapacity;
	size_t mSize;
	Particle2D* mParticles;

	QuadTree* mNorthWest;
	QuadTree* mNorthEast;
	QuadTree* mSouthWest;
	QuadTree* mSouthEast;

	bool mDivided;

	void SubDivide();
};