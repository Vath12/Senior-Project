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

void sprite::playAnimation(int row, int _numFrames, int _frameRate) {
	playing = true;
	frameTimer = 0;
	currentFrame = 0;
	currentRow = row;
	numFrames = _numFrames;
	frameRate = _frameRate;
}
void sprite::updateAnimation(double deltaTime) {
	frameTimer += deltaTime;
	if (frameTimer > (1.0/frameRate)) {
		currentFrame = (currentFrame + 1) % numFrames;
		frameTimer = 0;
	}
}

void sprite::draw(SDL_Renderer* renderer) {

	SDL_Rect rect = worldToCamera(x,y,w,h);
	SDL_Rect srcrect = {cellSize*currentFrame,cellSize*currentRow,cellSize,cellSize};

	renderTexturePortion(renderer,texture,&rect,&srcrect);
}

void sprite::init() {
	allSprites = std::vector<sprite*>();
}
void sprite::update(SDL_Renderer* renderer, double deltaTime) {
	for (int i = 0; i < allSprites.size(); i++) {
		if (allSprites[i]->playing) {
			allSprites[i]->updateAnimation(deltaTime);
		}
		allSprites[i]->draw(renderer);
	}
}
