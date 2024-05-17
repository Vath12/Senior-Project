#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

SDL_Window* render_initialize(const char title[], int x, int y, int width, int height, int flags);
SDL_Surface* get_surface(SDL_Window* window);