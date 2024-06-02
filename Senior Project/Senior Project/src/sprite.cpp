#include "sprite.h"
#include "image_renderer.h"
#include "render.h"
#include "stdio.h"
#include <vector>
#include <algorithm>

std::vector<sprite*> allSprites;
std::vector<drawData> drawQueue;

sprite::sprite() {
	allSprites.push_back(this);
}

sprite::sprite(SDL_Texture* _texture, double _w, double _h) {
	allSprites.push_back(this);
	texture = _texture;
	w = _w;
	h = _h;
}

sprite::sprite(SDL_Texture* _texture, double _w, double _h, int _cellSize) {
	allSprites.push_back(this);
	texture = _texture;
	w = _w;
	h = _h;
	cellSize = _cellSize;
}

void sprite::draw(SDL_Renderer* renderer, vector2 position, int srcX, int srcY, int z) {

	SDL_Rect rect = worldToCamera(position.x,position.y,w,h);
	rect.x -= rect.w * centerX;
	rect.y -= rect.h * centerY;

	drawQueue.push_back(drawData(this,rect,0,srcX,srcY));
}


bool compareDrawData(drawData A, drawData B) {
	if (A.z != B.z) {
		return A.z < B.z;
	}
	return  A.rect.y + (A.rect.h * A.img->max_bound_y) < B.rect.y + (B.rect.h*B.img->max_bound_y) ;
	//return  (A.rect.y + A.rect.h) < (B.rect.y + B.rect.h);
}

void drawSprites(SDL_Renderer* renderer) {

	std::sort(drawQueue.begin(), drawQueue.end(), compareDrawData);
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	for (int i = 0; i < drawQueue.size(); i++) {
		drawData* d = &drawQueue[i];
		SDL_Rect srcrect = { d->srcX * d->img->cellSize,d->srcY * d->img->cellSize,d->img->cellSize,d->img->cellSize };
		renderTexturePortion(renderer, d->img->texture, &d->rect, &srcrect);
		
		//SDL_Rect debugRect = {
		//	d->rect.x + d->rect.w * d->img->min_bound_x,
		//	d->rect.y + d->rect.h * d->img->min_bound_y,
		//	d->rect.w * (d->img->max_bound_x - d->img->min_bound_x),
		//	d->rect.h * (d->img->max_bound_y - d->img->min_bound_y)};
		//SDL_RenderDrawRect(renderer,&debugRect);
	}
	drawQueue.clear();
}

void sprite::init() {
	allSprites = std::vector<sprite*>();
}

