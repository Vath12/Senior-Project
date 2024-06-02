#pragma once
#include <SDL.h>
#include <stdlib.h>
#include <vector>

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char path[]);
void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* rect);
void renderTexturePortion(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* rect, SDL_Rect* srcRect);
void unloadTextures();
void registerTexture(SDL_Texture* T);


