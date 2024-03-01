#pragma once

#include "Particle2D.h"
#include "Node.h"
#include "Rectangle.h"

class KDTree {
public:
	KDTree();
	KDTree(const Particle2D& particle);
	KDTree(const KDTree& other);
	virtual ~KDTree();


	void Insert(const Particle2D* particle);

	Particle2D* NeareastNeighborOrNull(const Particle2D* particle);
	

	void PrintByDFS(void) const;
	void PrintByBFS(void) const;

	void DrawAllParticles();
	void DrawPartition();

	void Update(const Rectangle* rectangle, const GLfloat delta);

private:
	static const size_t k;
	node_t* mRoot;

	void CollisionCheck();

	node_t* InsertRecursive(node_t* node, const Particle2D* particle, const size_t depth);

	node_t* NeareastNeighborOrNullRecursive(node_t* node, const Particle2D* particle, const size_t depth);

	node_t* Closest(const node_t* node0, const node_t* node1, const Particle2D* particle);

	float GetSquaredDistance(const Particle2D* p0, const Particle2D* p1);

	void ClearKDTreeRecursive(node_t* node);

	node_t* DeepCopyRecursive(node_t* node);

	node_t* DrawAllParticlesRecursive(node_t* node);
	node_t* DrawPartitionRecursive(node_t* node, const size_t depth);

	void GetLineVertexRecursive(node_t* node, const size_t depth, 
		const point2D_t& pos, const size_t pos_depth,
		point2D_t& oMinPos, point2D_t& oMaxPos);

	node_t* UpdateRecursive(const Rectangle* rectangle, const node_t* node, const GLfloat delta);
};