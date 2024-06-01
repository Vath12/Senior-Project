#include "sprite.h"
#include "image_renderer.h"
#include "render.h"
#include "stdio.h"
#include <vector>

std::vector<sprite*> allSprites;

sprite::sprite() {
	allSprites.push_back(this);
}

sprite::sprite(SDL_Texture* _texture, double _x, double _y, double _w, double _h) {
	allSprites.push_back(this);
	texture = _texture;
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

sprite::sprite(SDL_Texture* _texture, double _x, double _y, double _w, double _h, int _cellSize) {
	allSprites.push_back(this);
	texture = _texture;
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	cellSize = _cellSize;
}

void sprite::draw(SDL_Renderer* renderer) {

	SDL_Rect rect = worldToCamera(x, y, w, h);

	renderTexturePortion(renderer, texture, &rect, NULL);
}

void sprite::draw(SDL_Renderer* renderer, int srcX, int srcY) {

	SDL_Rect rect = worldToCamera(x,y,w,h);
	rect.x -= rect.w/2.0;
	rect.y -= rect.h / 2.0;
	SDL_Rect srcrect = {srcX*cellSize,srcY*cellSize,cellSize,cellSize};

	renderTexturePortion(renderer,texture,&rect,&srcrect);
}

void sprite::init() {
	allSprites = std::vector<sprite*>();
}

