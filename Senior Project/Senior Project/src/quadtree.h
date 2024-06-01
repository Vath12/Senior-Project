#pragma once
#include <vector>
#include "vector2.h"
#include <SDL.h>
class entity;

struct node {
	int parent = -1;
	int children[2][2]{
		{-1,-1},
		{-1,-1}
	};
};

struct leaf : node {
	std::vector<entity*> residents;
};

struct quadtree {
	double bound;
	std::vector<node> nodes;
};

quadtree makeTree(std::vector<entity*>* source,double size,int maxSubdivision);

std::vector<entity*> findInRadius(quadtree* tree, vector2 position, double radius);

void drawTree(quadtree* tree, SDL_Renderer* renderer);