#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <SDL.h>
#include <algorithm>
#include "render.h"
#include "image_renderer.h"
#include "eventHandler.h"
#include "globals.h"
#include "sprite.h"
#include "entity.h"
#include "vector2.h"
#include "unit.h"
#include "group.h"
#include "god.h"
#include "player.h"
#include <random>

int window_width = 1920;
int window_height = 1080;
int mouse_x = 0;
int mouse_y = 0;
double camera_x = 0;
double camera_y = 0;
double camera_viewportWidth = 50;
double deltaTime = 0;
input keys = input();
SDL_Window* window = NULL;
Uint32 mouse_state=0;
Uint32 mouse_state_single=0;
//spritesheets have 128x128 cells and anims play at 10fps

double getTimeMillis() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;
}

int main(int argc, char* args[]) {
	bool run = true;

	sprite::init();
	window = render_initialize("SeniorProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,1920,1080,SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);

	SDL_Rect rect = { 150, 150, 100, 100 };

	SDL_Texture* infRun = loadTexture(renderer,"resources/images/inf_rifle_walk_sheet.bmp");

	double lastTime = getTimeMillis();

	sprite infantryRun = sprite(infRun, 10, 10, 8, 8, 128);
	sprite infantryIdle = sprite(infRun, 10, 10, 8, 8, 128);

	srand(3579623);


	for (int i = 0; i < 4;i++) {
		group* squad = createGroup();

		for (int i = 0; i < 7; i++) {

			unit* soldier = create<unit>(&infantryRun, vector2(40 * (rand() % 1000) / 1000.0, 40 * (rand() % 1000) / 1000.0), 0);
			soldier->speed = 1.1;
			soldier->acceleration = 100;
			soldier->moveAnimation = &infantryRun;
			soldier->moveState = { 0,6,0,10,0 };
			soldier->idleAnimation = &infantryIdle;
			soldier->idleState = { 0,6,0,0,0 };
			soldier->animating = true;
			soldier->moving = false;
			soldier->hitbox = vector2(0.65, 1.71);
			soldier->direction = (rand() % 8);
			squad->addMember(soldier);
		}
	}


	while (run) {

		SDL_GetWindowSizeInPixels(window, &window_width, &window_height);

		mouse_state_single = mouse_state;
		mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
		mouse_state_single = mouse_state & (~mouse_state_single);

		deltaTime = (getTimeMillis() - lastTime);
		lastTime = getTimeMillis();
		
		std::string newTitle = std::format("{} fps", std::round(std::min(1/deltaTime,1024.0)));

		SDL_SetWindowTitle(window,newTitle.c_str());

		SDL_Event event;
		resetSingleFrameEvents();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				run = false;
			}
			else {
				handleEvent(&event);
			}
		}

		//Clear the screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		vector2 cursorPos = cameraToWorld(vector2(mouse_x,mouse_y));

		std::sort(entities.begin(),entities.end(),entity::compareEntity);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		for (int x = 0; x < 100; x++) {
			for (int y = 0; y < 100; y++) {
				vector2 p1 = worldToCameraIso(vector2(x,y));
				vector2 p2 = worldToCameraIso(vector2(x+1, y));
				vector2 p3 = worldToCameraIso(vector2(x, y+1));
				vector2 p4 = worldToCameraIso(vector2(x+1, y+1));
				SDL_RenderDrawLine(renderer,p1.x,p1.y,p2.x,p2.y);
				SDL_RenderDrawLine(renderer, p2.x, p2.y, p4.x, p4.y);
				SDL_RenderDrawLine(renderer, p1.x, p1.y, p3.x, p3.y);
				SDL_RenderDrawLine(renderer, p3.x, p3.y, p4.x, p4.y);
			}
		}

		for (int i = 0; i < numEntities(); i++) {
			getEntity(i)->update(deltaTime);
			getEntity(i)->draw(renderer);
		}

		/*
		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		for (vector2 v : circleFormation(64)) {
			vector2 p = worldToCameraIso(v);
			SDL_RenderDrawPoint(renderer,p.x,p.y);
		}
		*/
		

		playerUpdate(deltaTime, renderer);

		SDL_RenderPresent(renderer);
	}

	freeAll();
	unloadTextures();
	SDL_DestroyWindowSurface(window);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

