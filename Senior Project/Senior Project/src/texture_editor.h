#pragma once
#include <SDL.h>
#include <vector>

SDL_Texture* colorReplace(SDL_Surface* S, SDL_Renderer* r, std::vector<SDL_Color> src, std::vector<SDL_Color> replace);