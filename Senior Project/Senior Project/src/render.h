#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <vector>

SDL_Rect worldToCamera(SDL_Rect* input);
SDL_Rect cameraToWorld(SDL_Rect* input);

SDL_Window* render_initialize(const char title[], int x, int y, int width, int height, int flags);
SDL_Surface* get_surface(SDL_Window* window);