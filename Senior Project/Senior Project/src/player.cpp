#include "player.h"
#include "globals.h"
#include "god.h"
#include "entity.h"
#include "group.h"
#include "unit.h"
#include "util.h"
#include "render.h"
#include <SDL.h>
#include <iostream>
#include "math.h"

bool selectDirection = false;
vector2 movePosition;

void playerUpdate(double deltaTime,SDL_Renderer* renderer) {

	int w = 1;
	int h = 1;

	SDL_GetWindowSizeInPixels(window, &w, &h);

	if (keys.release[SDL_SCANCODE_ESCAPE]) {
		SDL_ShowCursor(false);
	}

	if (keys.key[SDL_SCANCODE_W]) { camera_y -= camera_viewportWidth * 0.2 * deltaTime; }
	if (keys.key[SDL_SCANCODE_S]) { camera_y += camera_viewportWidth * 0.2 * deltaTime; }
	if (keys.key[SDL_SCANCODE_A]) { camera_x -= camera_viewportWidth * 0.2 * deltaTime; }
	if (keys.key[SDL_SCANCODE_D]) { camera_x += camera_viewportWidth * 0.2 * deltaTime; }

	double netZoomChange = 0;
	double zoomSpeed = camera_viewportWidth / 2;

	if (keys.key[SDL_SCANCODE_Z]) {
		netZoomChange = zoomSpeed * deltaTime;
	}
	if (keys.key[SDL_SCANCODE_C]) {
		netZoomChange = -zoomSpeed * deltaTime;
	}


	netZoomChange = std::min(netZoomChange,max_viewportWidth - camera_viewportWidth);
	netZoomChange = std::max(netZoomChange, min_viewportWidth - camera_viewportWidth);
	

	camera_x = (camera_x - netZoomChange / 2.0);
	camera_y = (camera_y - (netZoomChange * ((double)h / w)) / 2.0);
	camera_viewportWidth += netZoomChange;


	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


	vector2 mousePos = vector2(mouse_x, mouse_y);
	vector2 mouseWorldPos = cameraToWorldIso(vector2(mouse_x, mouse_y));


	bool select = mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT);
	bool hoverTerrain = true;


	
	//selection
	for (entity* e : entities) {
		vector2 scrPos = worldToCameraIso(e->position);
		if (pointInBox(mousePos, scrPos +  worldToCameraScale(e->hitboxCenter), worldToCameraScale(e->hitbox / 2.0))) {
			if (!selectDirection && select && e != nullptr) {
				unit* u = dynamic_cast<unit*>(e);
				if (u!=nullptr) {
					if (u->parent != nullptr) {
						u->parent->selected = true;
						u->parent->setFlag(ENTITYFLAGS::selected, true);
					}
				}
				e->setFlag(ENTITYFLAGS::selected, true);
				
			}
			hoverTerrain = false;
			break;
		}
	}
	
	//deselection
	if (mouse_state_single & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		for (entity* e : entities) {
			e->setFlag(ENTITYFLAGS::selected, false);
		}
		for (group* g : groups) {
			g->selected = false;
		}
		selectDirection = false;
	}

	SDL_SetRenderDrawColor(renderer,255,255,255,255);

	int numSelected = 0;
	for (group* g : groups) {
		if (g->selected) {
			for (unit* u : g->members) {
				u->debugDraw(renderer);
			}
			numSelected++;
		}
	}

	if (mouse_state_single & SDL_BUTTON(SDL_BUTTON_LEFT) && numSelected>0) {
		if (selectDirection) {
			
			selectDirection = false;
			vector2 dir = (mouseWorldPos - movePosition).normalized();
			std::vector<vector2> macroFormation = rectangleFormation(numSelected,dir);
			int i = 0;
			for (group* g : groups) {
				if (g->selected) {
					g->moveTo(movePosition + macroFormation[i]*4, dir);
					i++;
				}
			}
			
		}
		else {
			if (hoverTerrain) {
				movePosition = mouseWorldPos;
				selectDirection = true;
			}
		}
	}

		

	if (selectDirection) {
		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		vector2 a = worldToCameraIso(mouseWorldPos);
		vector2 b = worldToCameraIso(movePosition);
		SDL_RenderDrawLine(renderer,a.x,a.y,b.x,b.y);
	}
}
