#include "image_renderer.h"
#include <iostream>
#include <SDL.h>

std::vector<SDL_Texture*> loadedTextures;

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char path[]) {
	SDL_Surface* tmp = SDL_LoadBMP( path );
	if (!tmp) {
		printf(SDL_GetError());
		return 0;
	}

	SDL_Texture* loaded = SDL_CreateTextureFromSurface(renderer,tmp);

	SDL_FreeSurface(tmp);

	loadedTextures.push_back(loaded);

	return loaded;
}

void registerTexture(SDL_Texture* T) {
	loadedTextures.push_back(T);
}

void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* rect){
	SDL_RenderCopy(renderer,texture,NULL,rect);
}

void renderTexturePortion(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* rect, SDL_Rect* srcRect) {
	SDL_RenderCopy(renderer, texture, srcRect, rect);
}

void unloadTextures() {
	int count = 0;
	while (!loadedTextures.empty()){
		SDL_DestroyTexture(loadedTextures.back());
		loadedTextures.pop_back();
		count++;
	}
	std::cout << count << " textures freed" << std::endl;
}