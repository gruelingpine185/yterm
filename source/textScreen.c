#include "textScreen.h"
#include "safe.h"

TextScreen TextScreen_New(uint32_t w, uint32_t h) {
	TextScreen ret;

	ret.cells  = SafeMalloc(w * h * sizeof(Cell));
	ret.size   = (Vec2) {.x = w, .y = h};
	ret.cursor = (Vec2) {.x = 0, .y = 0};

	for (size_t i = 0; i < w * h; ++ i) {
		ret.cells[i] = (Cell) {' '};
	}

	return ret;
}

void TextScreen_Free(TextScreen* text) {
	free(text->cells);
}

Cell TextScreen_GetCharacter(TextScreen* text, int x, int y) {
	return text->cells[(y * text->size.x) + x];
}

void TextScreen_SetCharacter(TextScreen* text, int x, int y, Cell cell) {
	if (
		(x < 0) ||
		(y < 0) ||
		(x >= text->size.x) ||
		(y >= text->size.y)
	) {
		return;
	}

	text->cells[(y * text->size.x) + x] = cell;
}

void TextScreen_PutCharacter(TextScreen* text, char ch) {
	switch (ch) {
		case '\n': {
			++ text->cursor.y;
			text->cursor.x = 0;
			break;
		}
		case '\r': {
			text->cursor.x = 0;
			break;
		}
		case '\t': {
			text->cursor.x += 4;
			break;
		}
		default: {
			TextScreen_SetCharacter(
				text, text->cursor.x, text->cursor.y, (Cell) {ch}
			);
			++ text->cursor.x;
			break;
		}
	}
	
	// wrap
	if (text->cursor.x >= text->size.x) {
		text->cursor.x = 0;
		++ text->cursor.y;
	}
}

void TextScreen_PutString(TextScreen* text, char* str) {
	for (size_t i = 0; i < strlen(str); ++ i) {
		TextScreen_PutCharacter(text, str[i]);
	}
}

void TextScreen_Render(TextScreen* text, Video* video) {
	SDL_SetRenderDrawColor(video->renderer, 0, 0, 0, 255);
	SDL_RenderClear(video->renderer);

	for (int y = 0; y < text->size.y; ++ y) {
		for (int x = 0; x < text->size.x; ++ x) {
			SDL_Rect rect;
			rect.x = x * video->charWidth;
			rect.y = y * video->charHeight;
			rect.w = video->charWidth;
			rect.h = video->charHeight;

			Cell cell = TextScreen_GetCharacter(text, x, y);

			SDL_Color fg = {255, 255, 255, 255};
			SDL_Color bg = {0,   0,   0,   255};

			if ((x == text->cursor.x) && (y == text->cursor.y)) {
				SDL_Color temp = fg;
				fg             = bg;
				bg             = temp;
			}

			SDL_SetRenderDrawColor(video->renderer, bg.r, bg.g, bg.b, bg.a);
			SDL_RenderFillRect(video->renderer, &rect);

			Video_DrawCharacter(video, rect.x, rect.y, cell.ch, fg);
		}
	}
}