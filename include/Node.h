#pragma once

#include "Point.h"

typedef struct Node {
	point2D_t point;
	Node* left;
	Node* right;
	size_t depth;
} node_t;