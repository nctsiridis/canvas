#ifndef CANVAS_H
#define CANVAS_H
#include "../core.h"

typedef struct LineNode {
	struct LineNode *prev, *next;
	char* text;
	int length;
} LineNode;

typedef struct {
	int rows_on_screen, cols_on_screen;
	int sym_width, sym_height;
	int line_pos, inline_pos, frame_pos; // frame_pos = index of first line in the visible frame
	LineNode *line;
	SDL_Texture **glyphs;
} CanvasData;

void canvas_update(AppData* app_data, ContextForward context, SDL_Rect rect, void** d);

CanvasData* canvas_default_data();

#endif
