#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <vector>
#include "vector2.h"

void drawEllipsoidPolygon(SDL_Renderer* renderer,  vector2 center, vector2 radii);

vector2 cameraToWorldIso(vector2 a);
vector2 worldToCameraIso(vector2 a);

SDL_Rect worldToCamera(SDL_Rect* input);
SDL_Rect worldToCamera(double x,double y,double w,double h);
SDL_Rect cameraToWorld(SDL_Rect* input);

vector2 worldToCamera(vector2 position);
vector2 cameraToWorld(vector2 position);

double worldToCameraScale(double x);
double cameraToWorldScale(double x);

vector2 worldToCameraScale(vector2 x);
vector2 cameraToWorldScale(vector2 x);

SDL_Window* render_initialize(const char title[], int x, int y, int width, int height, int flags);
SDL_Surface* get_surface(SDL_Window* window);