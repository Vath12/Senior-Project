#include "quadtree.h"
#include "math.h"
#include "entity.h"
#include "vector2.h"
#include <vector>
#include <iostream>
#include <SDL.h>
#include "render.h"
#include "util.h"

quadtree makeTree(std::vector<entity*>* source, double size, int maxSubdivision) {

	quadtree out = quadtree{
		size,
		std::vector<node>(),
		std::vector < std::vector<entity*>>()
	};
	
	out.nodes.push_back(node());

	for (entity* e : *source) {
		int depth = 0;
		double bound = size;
		vector2 position = vector2();

		int current = 0;

		while (depth < maxSubdivision) {

			bool h = e->position.x > position.x;
			bool v = e->position.y < position.y;
			int qx = 0;
			int qy = 0;
			// 0 1
			// 0 0
			if (h && v) { qx = 1; qy = 0; };
			// 1 0
			// 0 0
			if (!h && v) { qx = 0; qy = 0; };
			// 0 0
			// 0 1
			if (h && !v) { qx = 1; qy = 1; };
			// 0 1
			// 1 0
			if (!h && !v) { qx = 0; qy = 1; };
				
			depth++;

			

			if (depth == maxSubdivision) {
				if (out.nodes[current].children[qx][qy] == -1) {
					out.residents.push_back(std::vector<entity*>());
					out.nodes.push_back(node());
					out.nodes.back().data = out.residents.size() - 1;
					out.nodes[current].children[qx][qy] = out.nodes.size()-1;
				}
				out.residents[ out.nodes[out.nodes[current].children[qx][qy]].data].push_back(e);
			}
			else {
				position = position + vector2( ((qx * 2) - 1) * bound/2.0, ((qy * 2) - 1) * bound/2.0);
				bound /= 2.0;
				if (out.nodes[current].children[qx][qy] == -1) {
					out.nodes.push_back(node());
					out.nodes[current].children[qx][qy] = out.nodes.size()-1;
					out.nodes.back().parent = current;
				}
				current = out.nodes[current].children[qx][qy];
			}
		}
	}

	return out;

}

bool boxIntersectCircle(vector2 origin, double boxSize, vector2 circlePos, double radius) {
	vector2 quadrant = vector2(1,1);
	circlePos = circlePos - origin;
	if (circlePos.x < 0) { quadrant.x = -1; }
	if (circlePos.y < 0) { quadrant.y = -1; }
	double corner = 0.70710678118 * boxSize;
	return (circlePos - (quadrant * corner)).getMagnitude() < radius;
}

bool circleEncloseBox(vector2 origin, double boxSize, vector2 circlePos, double radius) {
	vector2 quadrant = vector2(-1, -1);
	circlePos = circlePos - origin;
	if (circlePos.x < 0) { quadrant.x = 1; }
	if (circlePos.y < 0) { quadrant.y = 1; }
	double corner = 0.70710678118 * boxSize;
	return (circlePos - (quadrant * corner)).getMagnitude() < radius;
}


void findInRadiusRecursive(std::vector<entity*>* output, quadtree* tree, int node, vector2 position, double radius, vector2 bPos, double bound, bool enclosed) {
	
	bool isLeaf = true;
	double subBound = bound / 2.0;
	
	for (int i = 0; i < 4; i++) {

		int sx = 0;
		int sy = 0;

		if (i == 1 || i == 3) { sy = 1; }
		if (i == 2 || i == 3) { sx = 1; }

		int subNode = tree->nodes[node].children[sx][sy];

		if (subNode != -1) {
			isLeaf = false;
			vector2 subBPos = bPos + (vector2(sx, sy) * subBound);

			bool intersection = boxIntersectCircle(subBPos, subBound, position, radius);
			bool subEnclosed = enclosed || circleEncloseBox(subBPos, subBound, position, radius);

			if (subEnclosed || intersection || pointInBox(position,subBPos + vector2(subBound, subBound)/2.0,vector2(subBound,subBound)/2.0)) {
				findInRadiusRecursive(output, tree, subNode, position, radius, subBPos, subBound, subEnclosed);
			}
		}
		
	}

	if (isLeaf) {

		if (enclosed) {
			for (entity* e : tree->residents[tree->nodes[node].data]) {
				if (e != nullptr) {
					output->push_back(e);
				}
			}
		}
		else {
			for (entity* e : tree->residents[tree->nodes[node].data]) {
				if (e != nullptr && (e->position - position).getMagnitude() <= radius) {
					output->push_back(e);
				}
			}
		}
	}
}

std::vector<entity*> findInRadius(quadtree* tree, vector2 position, double radius) {
	std::vector<entity*> output = std::vector<entity*>();
	findInRadiusRecursive(&output,tree,0,position,radius,vector2(-tree->bound,-tree->bound),tree->bound*2,false);
	return output;
}

void drawTreeRecursive(quadtree* tree, int node,vector2 a,vector2 b,SDL_Renderer* r) {
	
	bool isLeaf = true;

	for (int i = 0; i < 4; i++) {

		int sx = 0;
		int sy = 0;

		if (i == 1 || i == 3) { sy = 1; }
		if (i == 2 || i == 3) { sx = 1; }

		int subNode = tree->nodes[node].children[sx][sy];

		if (subNode != -1) {
			isLeaf = false;
			vector2 subBPos = a + (vector2(sx*b.x/2.0, sy*b.y/2.0));
			drawTreeRecursive(tree, subNode, subBPos, b / 2.0, r);
		}

	}
	
	//if (leaf) {

	SDL_Point points[5];
	vector2 pts[] =
	{
		a,
		a + vector2(b.x,0),
		a + b,
		a + vector2(0,b.y),
	};
	for (int i = 0; i < 4; i++) {
		pts[i] = worldToCameraIso(pts[i]);
		points[i] = { (int)pts[i].x,(int)pts[i].y };
	}
	points[4] = points[0];
	SDL_RenderDrawLines(r,points,5);

	//}

}

void drawTree(quadtree* tree, SDL_Renderer* renderer) {
	drawTreeRecursive(tree,0,vector2(-tree->bound,-tree->bound),vector2(tree->bound*2, tree->bound*2),renderer);
}