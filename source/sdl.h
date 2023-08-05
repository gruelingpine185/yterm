#ifndef YTERM_SDL_H
#define YTERM_SDL_H

#include "components.h"

typedef struct Video {
	SDL_Window*   window;
	SDL_Renderer* renderer;
	TTF_Font*     font;
	SDL_Texture*  characters[256];
	int           charWidth;
	int           charHeight;
} Video;

Video Video_Init(void);
void  Video_Free(Video* video);
void  Video_OpenFont(Video* video, char* path);
void  Video_FreeFont(Video* video);
void  Video_DrawCharacter(Video* video, int x, int y, uint8_t ch, SDL_Color colour);

#endif
