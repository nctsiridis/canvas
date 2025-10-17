#ifndef CANVAS_H
#define CANVAS_H
#include "../core.h"

typedef struct CmdNode {
	void (*cmd);
	MapUnordered *mp;
} CmdNode;

typedef struct LineNode {
	struct LineNode *prev, *next;
	char* text;
	int length; // size = capacity, length = num chars
	int gbuff_pos, gbuff_size;
} LineNode;

typedef struct {
	int rows_on_screen, cols_on_screen;
	int sym_width, sym_height;
	int line_pos, line_size, frame_pos; // frame_pos = index of first line in the visible frame
	LineNode *line;
	SDL_Texture **glyphs;
	SDL_Rect rect;
} CanvasData;

void canvas_update(AppData* app_data, ContextForward context, RelativeRect rect, void** d);

CanvasData* canvas_default_data();

#endif
