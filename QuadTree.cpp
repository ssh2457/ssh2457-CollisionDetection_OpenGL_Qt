#include "include/QuadTree.h"

#include <memory>
#include <iostream>

static const int X = 0;
static const int Y = 1;

QuadTree::QuadTree(Rectangle& boundary, const size_t capacity)
	: Rectangle(boundary)
	, mCapacity(capacity)
	, mParticles(nullptr)
	, mSize(0)
	, mNorthWest(nullptr)
	, mNorthEast(nullptr)
	, mSouthWest(nullptr)
	, mSouthEast(nullptr)
	, mDivided(false)
{
	mParticles = new Particle2D[mCapacity];
}

QuadTree::~QuadTree()
{
	delete[] mParticles; mParticles = nullptr;

	delete mNorthEast; mNorthEast = nullptr;
	delete mNorthWest; mNorthWest = nullptr;
	delete mSouthEast; mSouthEast = nullptr;
	delete mSouthWest; mSouthWest = nullptr;
}

bool QuadTree::Insert(const Particle2D& particle)
{
	if (!Contain(particle.GetCoords())) {
		return false;
	}

	if (mSize < mCapacity) {
		mParticles[mSize] = particle;
		++mSize;

		return true;
	} else {
		if (!mDivided) {
			SubDivide();
		}
		
		if (mNorthEast->Insert(particle)) {
			return true;
		} else if (mNorthWest->Insert(particle)) {
			return true;
		} else if (mSouthEast->Insert(particle)) {
			return true;
		} else if (mSouthWest->Insert(particle)) {
			return true;
		}
	}
}

void QuadTree::Query(std::vector<Particle2D*>& o_found, const Rectangle& range)
{
	if (!range.Intersect(*this)) {
		return;
	}

	for (size_t i = 0; i < mSize; ++i) {
		if (range.Contain(mParticles[i].GetCoords())) {
			o_found.push_back(&mParticles[i]);
		}
	}

	if (mDivided) {
		mNorthEast->Query(o_found, range);
		mNorthWest->Query(o_found, range);
		mSouthEast->Query(o_found, range);
		mSouthWest->Query(o_found, range);
	}
}

void QuadTree::Print() const
{
	std::cout << "tree: " << std::endl;
	for (size_t i = 0; i < mSize; ++i) {
		std::cout << "x: " << mParticles[i].GetCoords().vec2[X] << ", y: " << mParticles[i].GetCoords().vec2[Y] << std::endl;
	}

	if (mNorthEast) {
		mNorthEast->Print();
	}

	if (mNorthWest) {
		mNorthWest->Print();
	}

	if (mSouthEast) {
		mSouthEast->Print();
	}

	if (mSouthWest) {
		mSouthWest->Print();
	}
}

void QuadTree::Update(Rectangle& boundary, const GLfloat delta, QuadTree* const root)
{
	std::vector<Particle2D*> neighbors;
	neighbors.reserve(10);

	for (size_t i = 0; i < mSize; ++i) {
		point2D_t queryCoords = mParticles[i].GetCoords();
		point2D_t queryVelocity = mParticles[i].GetVelocity();
		const float radius = mParticles[i].GetRadius();

		Rectangle range(queryCoords.vec2[X], queryCoords.vec2[Y], 10.f * radius, 10.f * radius, mRGBA);
		root->Query(neighbors, range);


		for (auto& neighbor : neighbors) {
			if (mParticles[i].CheckCollision(*neighbor)) {
				boundary.CheckBoundary(*neighbor);
				neighbor->Update(delta);
			}
		}

		boundary.CheckBoundary(mParticles[i]);
		mParticles[i].Update(delta);

		if (!Contain(mParticles[i].GetCoords())) {
			root->Insert(mParticles[i]);

			mParticles[i] = mParticles[mSize - 1];
			--mSize;
		}

		neighbors.clear();
	}

	if (mNorthEast) {
		mNorthEast->Update(boundary, delta, root);
	}

	if (mNorthWest) {
		mNorthWest->Update(boundary, delta, root);
	}

	if (mSouthEast) {
		mSouthEast->Update(boundary, delta, root);
	}

	if (mSouthWest) {
		mSouthWest->Update(boundary, delta, root);
	}

}

void QuadTree::showParticle(Graphic* graphic)
{
	for (size_t i = 0; i < mSize; ++i) {
		mParticles[i].Draw(graphic);
	}

	if (mNorthEast) {
		mNorthEast->showParticle(graphic);
	}

	if (mNorthWest) {
		mNorthWest->showParticle(graphic);
	}

	if (mSouthEast) {
		mSouthEast->showParticle(graphic);
	}

	if (mSouthWest) {
		mSouthWest->showParticle(graphic);
	}
}

void QuadTree::ShowPartition(Graphic* graphic)
{
	if (mSize > 0) {
		Draw(graphic);
	}

	if (mNorthEast) {
		mNorthEast->ShowPartition(graphic);
	}

	if (mNorthWest) {
		mNorthWest->ShowPartition(graphic);
	}

	if (mSouthEast) {
		mSouthEast->ShowPartition(graphic);
	}

	if (mSouthWest) {
		mSouthWest->ShowPartition(graphic);
	}
}

void QuadTree::SubDivide()
{
	float x = mCenter.vec2[X];
	float y = mCenter.vec2[Y];
	float w = mWidth;
	float h = mHeight;

	Rectangle ne(x + w / 4.f, y + h / 4.f, w / 2.f, h / 2.f, mRGBA);
	Rectangle nw(x - w / 4.f, y + h / 4.f, w / 2.f, h / 2.f, mRGBA);
	Rectangle se(x + w / 4.f, y - h / 4.f, w / 2.f, h / 2.f, mRGBA);
	Rectangle sw(x - w / 4.f, y - h / 4.f, w / 2.f, h / 2.f, mRGBA);


	mNorthEast = new QuadTree(ne, mCapacity);
	mNorthWest = new QuadTree(nw, mCapacity);
	mSouthEast = new QuadTree(se, mCapacity);
	mSouthWest = new QuadTree(sw, mCapacity);

	mDivided = true;
}
