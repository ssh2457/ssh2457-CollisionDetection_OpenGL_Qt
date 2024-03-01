#pragma once

#include "Particle2D.h"

typedef struct Node {
	Particle2D* particle;
	Node* left;
	Node* right;
} node_t;