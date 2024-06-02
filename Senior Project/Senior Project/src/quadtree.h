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
	int data = -1;
};

struct quadtree {
	double bound;
	std::vector<node> nodes;
	std::vector < std::vector<entity*>> residents;
};

quadtree makeTree(std::vector<entity*>* source,double size,int maxSubdivision);

std::vector<entity*> findInRadius(quadtree* tree, vector2 position, double radius);

void drawTree(quadtree* tree, SDL_Renderer* renderer);