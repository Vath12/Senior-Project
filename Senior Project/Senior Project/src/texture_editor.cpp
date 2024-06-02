#include "texture_editor.h"
#include <SDL.h>
#include <vector>
#include "image_renderer.h"

void setPixel(SDL_Surface* s, int x, int y, SDL_Color c) {
	
	Uint8* pixel = (Uint8*) s->pixels + (y * s->pitch) + (x * s->format->BytesPerPixel);
	(*(Uint32*)pixel) = SDL_MapRGBA(s->format,c.r,c.g,c.b,c.a);
}

void getPixel(SDL_Surface* s, int x, int y,SDL_Color* output) {
	Uint8* pixel = (Uint8*)s->pixels + (y * s->pitch) + (x * s->format->BytesPerPixel);
	return SDL_GetRGBA((*(Uint32*)pixel),s->format,&output->r,&output->g,&output->b,&output->a);
}

SDL_Texture* colorReplace(SDL_Surface* input, SDL_Renderer* r, std::vector<SDL_Color> src, std::vector<SDL_Color> replace) {
	
	SDL_Surface* surf = SDL_DuplicateSurface(input);

	for (int x = 0; x < surf->w; x++) {
		for (int y = 0; y < surf->h; y++) {
			SDL_Color C = SDL_Color();
			getPixel(surf,x,y,&C);
			for (int i = 0; i < src.size(); i++) {
				if (C.r == src[i].r && C.g == src[i].g && C.b == src[i].b && C.a == src[i].a) {
					setPixel(surf, x, y, replace[i]);
				}
			}
		}
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
	registerTexture(tex);
	SDL_FreeSurface(surf);
	return tex;
	
}