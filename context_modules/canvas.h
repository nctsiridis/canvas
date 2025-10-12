#ifndef CANVAS_H
#define CANVAS_H
#include "../core.h"

typedef struct {
	int num_rows, num_cols, row_pos, col_pos;
	char* text;
	SDL_Texture **glyphs;
} CanvasData;

void canvas_update(AppData* app_data, bool draw, SDL_Rect rect, void** data);

CanvasData* canvas_default_data();

#endif
