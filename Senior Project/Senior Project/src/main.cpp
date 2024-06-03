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
#include "texture_editor.h"
#include "building.h"
#include "combat.h"
#include <chrono>

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


double getTimeNano() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}
double getTimeMillis() {
	return getTimeNano() / 1000000.0;
}
double getTime() {
	return getTimeNano() / 1000000000.0;
}

int main(int argc, char* args[]) {
	bool run = true;

	sprite::init();
	window = render_initialize("SeniorProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,1920,1080,SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);

	SDL_Rect rect = { 150, 150, 100, 100 };

	SDL_Texture* bunkerTexture = loadTexture(renderer,"resources/images/bunker.bmp");
	SDL_Texture* infRun = loadTexture(renderer,"resources/images/inf_rifle_walk_sheet.bmp");
	SDL_Texture* infIdle = loadTexture(renderer, "resources/images/inf_rifle_idle_sheet.bmp");
	SDL_Texture* infFire = loadTexture(renderer, "resources/images/inf_rifle_fire_sheet.bmp");
	SDL_Texture* infDead = loadTexture(renderer, "resources/images/inf_dead_sheet.bmp");
	SDL_Texture* blood = loadTexture(renderer, "resources/images/blood_sheet.bmp");

	std::vector<SDL_Color> srcPallate = std::vector<SDL_Color>();
	srcPallate.push_back(SDL_Color(122, 129, 65, 255));
	srcPallate.push_back(SDL_Color(143, 151, 74, 255));
	//backpack
	srcPallate.push_back(SDL_Color(205, 190, 51, 255));
	srcPallate.push_back(SDL_Color(168, 156, 49, 255));
	srcPallate.push_back(SDL_Color(157, 146, 39, 255));
	srcPallate.push_back(SDL_Color(121, 131, 37, 255));
	std::vector<SDL_Color> gerPallate = std::vector<SDL_Color>();
	gerPallate.push_back(SDL_Color(82, 93, 104, 255));
	gerPallate.push_back(SDL_Color(103, 117, 130, 255));

	gerPallate.push_back(SDL_Color(139, 110, 57, 255));
	gerPallate.push_back(SDL_Color(91, 71, 34, 255));
	gerPallate.push_back(SDL_Color(115, 91, 48, 255));
	gerPallate.push_back(SDL_Color(72, 55, 24, 255));


	SDL_Surface* tmp = SDL_LoadBMP("resources/images/inf_rifle_walk_sheet.bmp");
	SDL_Texture* infRun_G = colorReplace(tmp, renderer, srcPallate, gerPallate);
	tmp = SDL_LoadBMP("resources/images/inf_rifle_idle_sheet.bmp");
	SDL_Texture* infIdle_G = colorReplace(tmp, renderer, srcPallate, gerPallate);
	tmp = SDL_LoadBMP("resources/images/inf_rifle_fire_sheet.bmp");
	SDL_Texture* infFire_G = colorReplace(tmp, renderer, srcPallate, gerPallate);
	tmp = SDL_LoadBMP("resources/images/inf_dead_sheet.bmp");
	SDL_Texture* infDead_G = colorReplace(tmp, renderer, srcPallate, gerPallate);

	//speed at 10fps is 1.3 m/s
	sprite infantryRun = sprite(infRun, 6, 6, 128);
	infantryRun.centerY = 0.57;
	sprite infantryIdle = sprite(infIdle, 6, 6, 128);
	infantryIdle.centerY = 0.57;
	sprite infantryFire = sprite(infFire, 6, 6, 128);
	infantryFire.centerY = 0.57;
	sprite infantryDead = sprite(infDead, 6, 6, 128);

	infantryIdle.min_bound_x = 0.45;
	infantryIdle.min_bound_y = 0.38;
	infantryIdle.max_bound_x = 0.55;
	infantryIdle.max_bound_y = 0.6;

	infantryRun.min_bound_x = 0.45;
	infantryRun.min_bound_y = 0.38;
	infantryRun.max_bound_x = 0.55;
	infantryRun.max_bound_y = 0.6;

	infantryFire.min_bound_x = 0.45;
	infantryFire.min_bound_y = 0.38;
	infantryFire.max_bound_x = 0.55;
	infantryFire.max_bound_y = 0.6;

	sprite infantryRun_G = infantryRun;
	infantryRun_G.texture = infRun_G;
	sprite infantryIdle_G = infantryIdle;
	infantryIdle_G.texture = infIdle_G;
	sprite infantryFire_G = infantryFire;
	infantryFire_G.texture = infFire_G;
	sprite infantryDead_G = infantryDead;
	infantryDead_G.texture = infDead_G;

	sprite bunker = sprite(bunkerTexture, 6, 6, 128);
	bunker.min_bound_x = 0.32;
	bunker.min_bound_y = 0.25;
	bunker.max_bound_x = 0.65;
	bunker.max_bound_y = 0.58;

	sprite bloodPuddle = sprite(blood, 8, 4, 128);
	bloodPuddle.frames = 5;

	weapon rifle = {
		0.2, //blunt damage
		6, //penetration
		0.7, //post penetration damage
		100, //range
		0.7, //accuracy
		0.75, //handling
		0.6,//leading
		30, //fireRate
		5, //magazineSize
		4, //reloadTime
		0.8,//movementSpeedPenalty
		0.6 //movementAccuracyPenalty
	};

	weapon machine_gun = {
		0.8, //blunt damage
		11, //penetration
		1.3, //post penetration damage
		120, //range
		0.2, //accuracy
		0.13, //handling
		0.6,//leading
		600, //fireRate
		120, //magazineSize
		6, //reloadTime
		0.8,//movementSpeedPenalty
		0.7 //movementAccuracyPenalty
	};

	srand((unsigned int) getTimeNano());

	for (int i = 0; i < 16;i++) {
		group* squad = createGroup();

		for (int i = 0; i < 7; i++) {

			unit* soldier = create<unit>(&infantryRun, vector2(200,200) + vector2(50 * (rand() % 1000) / 1000.0, 50 * (rand() % 1000) / 1000.0), 0);
			soldier->speed = 1.6;
			soldier->acceleration = 100;
			soldier->moveAnimation = &infantryRun_G;
			soldier->moveState = { 0,6,0,12,0 };
			soldier->idleAnimation = &infantryIdle_G;
			soldier->idleState = { 0,1,0,10,0 };
			soldier->fireAnimation = &infantryFire_G;
			soldier->fireState = { 0,5,0,10,0 };
			soldier->dead.push_back(&bloodPuddle);
			soldier->dead.push_back(&infantryDead_G);
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

	for (int i = 0; i < 128; i++) {
		group* squad = createGroup();

		for (int i = 0; i < 7; i++) {

			unit* soldier = create<unit>(&infantryRun,vector2(0,0) + vector2(50 * (rand() % 1000) / 1000.0, 50 * (rand() % 1000) / 1000.0), 0);
			soldier->speed = 1.6;
			soldier->acceleration = 100;
			soldier->moveAnimation = &infantryRun;
			soldier->moveState = { 0,6,0,12,0 };
			soldier->idleAnimation = &infantryIdle;
			soldier->idleState = { 0,1,0,10,0 };
			soldier->fireAnimation = &infantryFire;
			soldier->fireState = { 0,5,0,10,0 };
			soldier->dead.push_back(&bloodPuddle);
			soldier->dead.push_back(&infantryDead);
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

	for (int i = 0; i < 8; i++) {
		double ang = (M_PI * 2 * i / 8.0) - 3*M_PI/4;
		building* b = create<building>(&bunker, vector2(200,200)+vector2(30*cos(ang), 30*sin(ang)), i);
		b->idle = &bunker;
		b->idleState = { 0,1,0,1,0 };
		b->fire = &bunker;
		b->fireState = { 0,2,0,23,0 };
		b->team = 0;
		b->addToGarrison(&machine_gun, 35, 45);
		b->addToGarrison(&machine_gun, -35, 45);
	}

	quadtree entity_quadtree = makeTree(&entities, 1024, 9);

	auto start = std::chrono::high_resolution_clock::now();

	int framerate = 0;
	double framerateDisplayInterval=0;

	while (run) {

		SDL_GetWindowSizeInPixels(window, &window_width, &window_height);

		mouse_state_single = mouse_state;
		mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
		mouse_state_single = mouse_state & (~mouse_state_single);

		deltaTime = (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count()) / 1000000000.0;
		start = std::chrono::high_resolution_clock::now();

		if (framerateDisplayInterval <= 0) {
			framerate = std::round(1 / deltaTime);;
			framerateDisplayInterval = 0.2;
		}
		framerateDisplayInterval -= deltaTime;

		std::string newTitle = std::format("{} fps",framerate);

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
		SDL_SetRenderDrawColor(renderer, 0, 70, 0, 255);
		SDL_RenderClear(renderer);

		//Drawing Grid
		/*
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 40 * (1.0 - fmin((camera_viewportWidth - min_viewportWidth) / 100.0, 1.0)));
		double scale = 1;
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
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
		*/

		entity_quadtree = makeTree(&entities, 1024, 9);

		//Drawing Quadtree
		//SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
		//drawTree(&entity_quadtree, renderer);

		for (group* g : groups) {
			g->update();
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (entity* e : entities) {
			unit* u = dynamic_cast<unit*>(e);
			if (u != nullptr && u->target == nullptr && u->searchCooldown<=0) {
				u->findTarget(&entity_quadtree);
				u->searchCooldown = 0.1 + (randomDouble() * 0.4);
			}
			building* b = dynamic_cast<building*>(e);
			if (b != nullptr) {
				for (int i = 0; i < b->fieldsOfFire.size(); i++) {
					vector2 p1 = worldToCameraIso(b->position);
					vector2 p2 = worldToCameraIso(b->position + b->fieldsOfFire[i].direction * b->maxRange);
					//SDL_RenderDrawLine(renderer,p1.x,p1.y,p2.x,p2.y);
				}
				b->findTargets(&entity_quadtree);
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

		updateCorpses(deltaTime);
		drawCorpses(renderer);
		for (int i = 0; i < entities.size(); i++) {
			entities[i]->draw(renderer);
		}
		playerDrawBackgroundUI(renderer);
		drawSprites(renderer);
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

