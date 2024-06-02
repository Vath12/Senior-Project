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
#include "effects.h"
#include <random>
#include "quadtree.h"
#include "util.h"

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
quadtree entity_quadtree;

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
	SDL_Texture* infIdle = loadTexture(renderer, "resources/images/inf_rifle_idle_sheet.bmp");
	SDL_Texture* infFire = loadTexture(renderer, "resources/images/inf_rifle_fire_sheet.bmp");

	double lastTime = getTimeMillis();

	//speed at 10fps is 1.3 m/s
	sprite infantryRun = sprite(infRun, 10, 10, 8, 8, 128);
	infantryRun.centerY = 0.55;
	sprite infantryIdle = sprite(infIdle, 10, 10, 8, 8, 128);
	infantryIdle.centerY = 0.55;
	sprite infantryFire = sprite(infFire, 10, 10, 8, 8, 128);
	infantryFire.centerY = 0.55;

	weapon rifle = {
		0.2, //blunt damage
		6, //penetration
		0.7, //post penetration damage
		100, //range
		0.1, //accuracy
		0.75, //handling
		0.6,//leading
		30, //fireRate
		5, //magazineSize
		4, //reloadTime
		0.8,//movementSpeedPenalty
		0.7 //movementAccuracyPenalty
	};

	srand((unsigned int) getTimeMillis());

	for (int i = 0; i < 64;i++) {
		group* squad = createGroup();

		for (int i = 0; i < 7; i++) {

			unit* soldier = create<unit>(&infantryRun, vector2(10 * (rand() % 1000) / 1000.0, 10 * (rand() % 1000) / 1000.0), 0);
			soldier->speed = 1.6;
			soldier->acceleration = 100;
			soldier->moveAnimation = &infantryRun;
			soldier->moveState = { 0,6,0,12,0 };
			soldier->idleAnimation = &infantryIdle;
			soldier->idleState = { 0,1,0,10,0 };
			soldier->fireAnimation = &infantryFire;
			soldier->fireState = { 0,5,0,10,0 };
			soldier->animating = true;
			soldier->moving = false;
			soldier->hitboxCenter = vector2(0,-0.35);
			soldier->hitbox = vector2(0.65, 1.71);
			soldier->direction = (rand() % 8);
			soldier->armament = &rifle;
			soldier->team = 0;
			squad->addMember(soldier);
		}
	}

	for (int i = 0; i < 64; i++) {
		group* squad = createGroup();

		for (int i = 0; i < 7; i++) {

			unit* soldier = create<unit>(&infantryRun,vector2(110,110) + vector2(10 * (rand() % 1000) / 1000.0, 10 * (rand() % 1000) / 1000.0), 0);
			soldier->speed = 1.6;
			soldier->acceleration = 100;
			soldier->moveAnimation = &infantryRun;
			soldier->moveState = { 0,6,0,12,0 };
			soldier->idleAnimation = &infantryIdle;
			soldier->idleState = { 0,1,0,10,0 };
			soldier->fireAnimation = &infantryFire;
			soldier->fireState = { 0,5,0,10,0 };
			soldier->animating = true;
			soldier->moving = false;
			soldier->hitboxCenter = vector2(0, -0.35);
			soldier->hitbox = vector2(0.65, 1.71);
			soldier->direction = (rand() % 8);
			soldier->armament = &rifle;
			soldier->team = 1;
			squad->addMember(soldier);
		}
	}

	quadtree entity_quadtree = makeTree(&entities,1024,7);


	while (run) {

		SDL_GetWindowSizeInPixels(window, &window_width, &window_height);

		mouse_state_single = mouse_state;
		mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
		mouse_state_single = mouse_state & (~mouse_state_single);

		deltaTime = (getTimeMillis() - lastTime);
		lastTime = getTimeMillis();

		std::string newTitle = std::format("{} fps", std::round(std::min(1 / deltaTime, 1024.0)));

		SDL_SetWindowTitle(window, newTitle.c_str());

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

		vector2 cursorPos = cameraToWorld(vector2(mouse_x, mouse_y));

		std::sort(entities.begin(), entities.end(), entity::compareEntity);

		SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);

		//Drawing Grid
		double scale = 5;
		for (int x = -500; x < 500; x++) {
			vector2 p1 = worldToCameraIso(vector2(x, -500) * scale);
			vector2 p2 = worldToCameraIso(vector2(x, 500) * scale);
			SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
		}
		for (int y = -500; y < 500; y++) {
			vector2 p1 = worldToCameraIso(vector2(-500, y) * scale);
			vector2 p2 = worldToCameraIso(vector2(500, y) * scale);
			SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
		}
		
		//Drawing Quadtree
		SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
		//drawTree(&entity_quadtree, renderer);

		entity_quadtree = makeTree(&entities, 1024, 8);

		for (group* g : groups) {
			g->update();
		}

		for (entity* e : entities) {
			unit* u = dynamic_cast<unit*>(e);
			if (u != nullptr && u->target == nullptr && u->searchCooldown<=0) {
				u->findTarget(&entity_quadtree);
				u->searchCooldown = 0.1 + (randomDouble() * 0.4);
			}
		}

		//loops are separate in case units are deleted
		for (int i = 0; i < entities.size();i++) {
			int tmp = entities.size();
			entities[i]->update(deltaTime);
			if (tmp != entities.size()) {
				i--;
			}
		}
		for (int i = 0; i < entities.size(); i++) {
			entities[i]->draw(renderer);
		}

		
		/*
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		vector2 tmpPos = vector2(80, 80);
		double tmpRadius = 90;
		SDL_Point points[17];

		for (int i = 0; i < 17; i++) {
			vector2 p = (worldToCameraIso(tmpPos + vector2(sin(M_PI * 2 * i / 16.0), cos(M_PI * 2 * i / 16.0)) * tmpRadius));
			points[i] = { (int)p.x,(int)p.y };
		}
		SDL_RenderDrawLines(renderer, points, 17);
		std::vector<entity*> result = findInRadius(&entity_quadtree, tmpPos, tmpRadius);
		for (entity* e : result) {
			unit* u = dynamic_cast<unit*>(e);
			if (u != nullptr) {
				u->debugDraw(renderer);
			}
		}
		*/

		drawFX(renderer, deltaTime);
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

