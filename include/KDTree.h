#pragma once

#include "Point.h"
#include "Node.h"

class KDTree {
public:
	KDTree();
	KDTree(const point2D_t& point);
	KDTree(const KDTree& other);
	virtual ~KDTree();


	void Insert(const point2D_t& point);
	node_t* InsertRecursive(node_t* node, const point2D_t& point, const size_t depth);

	point2D_t* NeareastNeighborOrNull(const point2D_t& point);
	

	void PrintByDFS(void) const;
	void PrintByBFS(void) const;

private:
	static const size_t k;
	node_t* mRoot;

	node_t* NeareastNeighborOrNullRecursive(node_t* node, const point2D_t& point, const size_t depth);

	node_t* Closest(const node_t* node0, const node_t* node1, const point2D_t& point);

	float GetSquaredDistance(const point2D_t& p0, const point2D_t& p1);

	void ClearKDTreeRecursive(node_t* node);

	node_t* DeepCopyRecursive(node_t* node);
};