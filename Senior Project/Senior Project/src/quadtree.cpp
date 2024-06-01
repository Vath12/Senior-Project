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
		std::vector<node>()
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
					out.nodes.push_back(leaf());
					out.nodes[current].children[qx][qy] = out.nodes.size()-1;
					out.nodes.back().parent = current;
					((leaf)out.nodes.back()).residents = std::vector<entity*>();
				}
				leaf cell = (leaf)out.nodes[out.nodes[current].children[qx][qy]];
				cell.residents.push_back(e);
			}
			else {
				position = position + vector2( ((qx * 2) - 1) * bound/2.0, ((qy * 2) - 1) * bound/2.0);
				bound /= 2.0;
				if (out.nodes[current].children[qx][qy] == -1) {
					out.nodes.push_back(node());
					out.nodes[current].children[qx][qy] = out.nodes.size()-1;
					out.nodes.back().parent = current;
					//std::cout << qx << "," << qy << " ";
					//std::cout << position.x << "," << position.y << std::endl;
				}
				current = out.nodes[current].children[qx][qy];
			}
		}
	}
	return out;

}

double boxIntersectCircle(vector2 origin, double boxSize, vector2 circlePos, double radius) {
	vector2 quadrant = vector2(1,1);
	circlePos = circlePos - origin;
	if (circlePos.x < 0) { quadrant.x = -1; }
	if (circlePos.y < 0) { quadrant.y = -1; }
	double corner = 0.70710678118 * boxSize;
	return (circlePos - (quadrant * corner)).getMagnitude();
}


void findInRadiusRecursive(std::vector<entity*>* output, quadtree* tree, int node, vector2 position, double radius, vector2 bPos, double bound, bool enclosed) {
	
	bool isLeaf = true;
	double subBound = bound / 2.0;

	for (int sx = 0; sx < 2; sx++) {
		for (int sy = 0; sy < 2; sy++) {
			int subNode = tree->nodes[node].children[sx][sy];

			if (subNode != -1) {
				isLeaf = false;
				vector2 subBPos = bPos + vector2(sx, sy) * subBound;

				double intersection = boxIntersectCircle(bPos, bound, position, radius);
				bool subEnclosed = enclosed || intersection >= (1.41421356237 * bound)-radius;

				if (subEnclosed || intersection <= radius || pointInBox(position,bPos + vector2(subBound, subBound)/2.0,vector2(subBound,subBound)/2.0)) {
					std::cout << subBPos.x << " " << subBPos.y << std::endl;
					findInRadiusRecursive(output, tree, subNode, position, radius, subBPos, subBound, subEnclosed);
				}
			}
		}
	}

	if (isLeaf) {
		for (int i = 0; i < ((leaf)tree->nodes[node]).residents.size(); i++) {
			output->push_back(((leaf)tree->nodes[node]).residents[i]);
		}
	}
}

std::vector<entity*> findInRadius(quadtree* tree, vector2 position, double radius) {
	std::vector<entity*> output = std::vector<entity*>();
	findInRadiusRecursive(&output,tree,0,position,radius,vector2(-tree->bound,-tree->bound),tree->bound*2,false);
	return output;
}

void drawTreeRecursive(quadtree* tree, int node,vector2 a,vector2 b,SDL_Renderer* r) {
	
	bool leaf = true;
	if (tree->nodes[node].children[0][0] != -1) {
		leaf = false;
		drawTreeRecursive(tree, tree->nodes[node].children[0][0], a, b/2.0,r);
	}
	if (tree->nodes[node].children[1][0] != -1) {
		leaf = false;
		drawTreeRecursive(tree, tree->nodes[node].children[1][0], a+vector2(b.x / 2.0,0), b / 2.0,r);
	}
	if (tree->nodes[node].children[0][1] != -1) {
		leaf = false;
		drawTreeRecursive(tree, tree->nodes[node].children[0][1], a + vector2(0, b.x / 2.0), b/2.0,r);
	}
	if (tree->nodes[node].children[1][1] != -1) {
		leaf = false;
		drawTreeRecursive(tree, tree->nodes[node].children[1][1], a + b/2.0, b/2.0,r);
	}
	//if (leaf) {
		//std::cout << "{leaf:"<<a.x<<","<<a.y<<","<< b.x << "," << b.y << "}"<<std::endl;
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