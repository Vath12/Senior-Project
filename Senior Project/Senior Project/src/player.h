#pragma once
#include <SDL.h>
#include <vector>

class unit;
class vector2;

void playerDrawBackgroundUI(SDL_Renderer* renderer);
void playerUpdate(double deltaTime, SDL_Renderer* renderer);
