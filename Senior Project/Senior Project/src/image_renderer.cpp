#include "image_renderer.h"
#include <stdio.h>
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

void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* rect){
	SDL_RenderCopy(renderer,texture,NULL,rect);
}

void renderTexturePortion(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* rect, SDL_Rect* srcRect) {
	SDL_RenderCopy(renderer, texture, srcRect, rect);
}

void unloadTextures() {
	while (loadedTextures.size() > 1){
		SDL_DestroyTexture(loadedTextures[loadedTextures.size()]);
		loadedTextures.pop_back();
	}
}