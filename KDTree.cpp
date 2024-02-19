#include "include/KDTree.h"

#include <memory>
#include <iostream>
#include <stack>
#include <queue>

const size_t KDTree::k = 2;

static const size_t X = 0;
static const size_t Y = 1;

KDTree::KDTree() : mRoot(nullptr)
{

}

KDTree::KDTree(const point2D_t& point) : mRoot(nullptr)
{
	mRoot = static_cast<node_t*>(malloc(sizeof(node_t)));
	memset(mRoot, 0, sizeof(node_t));
	mRoot->point.coords[X] = point.coords[X];
	mRoot->point.coords[Y] = point.coords[Y];
	mRoot->depth = 0;
}

KDTree::KDTree(const KDTree& other) : mRoot(nullptr)
{
	mRoot = DeepCopyRecursive(other.mRoot);
}

node_t* KDTree::DeepCopyRecursive(node_t* node)
{
	if (node == nullptr) {
		return nullptr;
	}

	node_t* newNode = static_cast<node_t*>(malloc(sizeof(node_t)));
	newNode->point.coords[X] = node->point.coords[X];
	newNode->point.coords[Y] = node->point.coords[Y];
	newNode->depth = node->depth;
	newNode->left = DeepCopyRecursive(node->left);
	newNode->right = DeepCopyRecursive(node->right);

	return newNode;
}

KDTree::~KDTree()
{
	ClearKDTreeRecursive(mRoot);
	mRoot = nullptr;
}

void KDTree::ClearKDTreeRecursive(node_t* node)
{
	if (node == nullptr) {
		return;
	}

	if (node->left == nullptr && node->right == nullptr) {
		// memset(node, 0, sizeof(node_t));
		free(node);
		node = nullptr;
		return;
	}

	ClearKDTreeRecursive(node->left);
	ClearKDTreeRecursive(node->right);
	node->left = nullptr;
	node->right = nullptr;
	ClearKDTreeRecursive(node);
}

void KDTree::Insert(const point2D_t& point)
{
	if (mRoot == nullptr) {
		mRoot = static_cast<node_t*>(malloc(sizeof(node_t)));
		memset(mRoot, 0, sizeof(node_t));
		mRoot->point.coords[X] = point.coords[X];
		mRoot->point.coords[Y] = point.coords[Y];
		mRoot->depth = 0;
		return;
	}

	InsertRecursive(mRoot, point, mRoot->depth);
}

node_t* KDTree::InsertRecursive(node_t* node, const point2D_t& point, const size_t depth)
{
	if (node == nullptr) {
		node = static_cast<node_t*>(malloc(sizeof(node_t)));
		memset(node, 0, sizeof(node_t));

		node->point.coords[X] = point.coords[X];
		node->point.coords[Y] = point.coords[Y];
		node->depth = depth;
		return node;
	}

	size_t cd = depth % k;
	if (point.coords[cd] < node->point.coords[cd]) {
		node->left = InsertRecursive(node->left, point, depth + 1);
	}
	else {
		node->right = InsertRecursive(node->right, point, depth + 1);
	}

	return node;
}

point2D_t* KDTree::NeareastNeighborOrNull(const point2D_t& point)
{
	node_t* best = NeareastNeighborOrNullRecursive(mRoot, point, mRoot->depth);
	if (best) {
		return &(best->point);
	}
	
	return nullptr;
}

node_t* KDTree::NeareastNeighborOrNullRecursive(node_t* node, const point2D_t& point, const size_t depth)
{
	if (node == nullptr) {
		return nullptr;
	}

	size_t cd = depth % k;

	node_t* nextBranch;
	node_t* otherBranch;

	if (point.coords[cd] < node->point.coords[cd]) {
		nextBranch = node->left;
		otherBranch = node->right;
	} else {
		nextBranch = node->right;
		otherBranch = node->left;
	}


	node_t* temp = NeareastNeighborOrNullRecursive(nextBranch, point, node->depth + 1);
	node_t* best = Closest(temp, node, point);

	const float radius_sq = GetSquaredDistance(best->point, point);
	const float dist_sq = GetSquaredDistance(node->point, point);

	if (radius_sq > dist_sq) {
		temp = NeareastNeighborOrNullRecursive(otherBranch, point, node->depth + 1);
		best = Closest(temp, best, point);
	}

	return best;
}

node_t* KDTree::Closest(const node_t* node0, const node_t* node1, const point2D_t& point)
{
	if (node0 == nullptr) {
		return (node_t*)node1;
	}

	if (node1 == nullptr) {
		return (node_t*)node0;
	}

	const float d0 = GetSquaredDistance(node0->point, point);
	const float d1 = GetSquaredDistance(node1->point, point);

	if (d0 < d1) {
		return (node_t*)node0;
	}


	return (node_t*)node1;
}

float KDTree::GetSquaredDistance(const point2D_t& p0, const point2D_t& p1)
{
	return (p0.coords[X] - p1.coords[X]) * (p0.coords[X] - p1.coords[X])
			+ (p0.coords[Y] - p1.coords[Y]) * (p0.coords[Y] - p1.coords[Y]);
}

void KDTree::PrintByDFS(void) const
{
	if (mRoot == nullptr) {
		return;
	}

	std::cout << "Print by DFS" << std::endl;
	std::stack<node_t*> nodeStack;
	nodeStack.push(mRoot);
	while (!nodeStack.empty()) {
		node_t* next = nodeStack.top();
		nodeStack.pop();

		for (int i = 0; i < next->depth; ++i) {
			std::cout << ' ';
		}

		std::cout << '-' << "point[x, y]: "  << next->point.coords[X] << ", " << next->point.coords[Y] << '\n';

		if (next->left != nullptr) {
			nodeStack.push(next->left);
		}

		if (next->right != nullptr) {
			nodeStack.push(next->right);
		}
	}

	std::cout << "Print by DFS end\n";
}

void KDTree::PrintByBFS(void) const
{
	if (mRoot == nullptr) {
		return;
	}

	std::cout << "Print by BFS" << std::endl;
	std::queue<node_t*> nodeQueue;
	nodeQueue.push(mRoot);
	while (!nodeQueue.empty()) {
		node_t* next = nodeQueue.front();
		nodeQueue.pop();

		for (int i = 0; i < next->depth; ++i) {
			std::cout << ' ';
		}

		std::cout << '-' << "point[x, y]: " << next->point.coords[X] << ", " << next->point.coords[Y] << '\n';

		if (next->left != nullptr) {
			nodeQueue.push(next->left);
		}

		if (next->right != nullptr) {
			nodeQueue.push(next->right);
		}
	}

	std::cout << "Print by BFS end\n";
}
