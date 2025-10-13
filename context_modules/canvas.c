#include "canvas.h"

#define ROWS_ON_SCREEN 30
#define COLS_ON_SCREEN 50

CanvasData* init_canvas_data(SDL_Renderer* renderer) {
	CanvasData *canvas_data = malloc(sizeof(CanvasData));
	SDL_Color color = {0x00, 0x00, 0x00, 0xFF};
	SDL_Color background = {0xFF, 0xFF, 0xFF, 0xFF};
	TTF_Font* font = TTF_OpenFont("res/short_baby.ttf", 20);
	if (!font) printf("Font creationg failed\n");
	canvas_data->rows_on_screen = ROWS_ON_SCREEN; // TODO
	canvas_data->cols_on_screen = COLS_ON_SCREEN;
	canvas_data->line_pos = 0;
	canvas_data->inline_pos = 0;
	canvas_data->frame_pos = 0;
	canvas_data->glyphs = malloc(sizeof(SDL_Texture*) * 127);

	canvas_data->line = malloc(sizeof(LineNode));
	canvas_data->line->prev = NULL;
	canvas_data->line->next = NULL;
	canvas_data->line->text = calloc(100, sizeof(char)); // TODO rows_on_screen should be modifiable

	for (int ch = 32; ch <= 126; ch++) {
		char str[2] = {ch, '\0'};
		SDL_Surface* surf = TTF_RenderText_Shaded(font, str, color, background);
		canvas_data->glyphs[ch] = SDL_CreateTextureFromSurface(renderer, surf);
	}

	return canvas_data;
}

void update_text(AppData* app_data, CanvasData *data) {
	if (app_data->sdl.event->type == SDL_TEXTINPUT) {
		data->line->text[data->inline_pos] = app_data->sdl.event->text.text[0];
		data->inline_pos++;
	}
}

void draw_text(AppData* app_data, CanvasData *data, SDL_Rect rect) {
	int width = app_data->sdl.window_width / data->cols_on_screen;
	int height = app_data->sdl.window_height / data->rows_on_screen;
	SDL_Rect dst = {0, 0, width, height};
	LineNode *line_iter = data->line;
	while (line_iter) {
		for (char *p = data->line->text; *p; p++) {
			if (*p < 32 || *p > 126) continue;
			SDL_RenderCopy(app_data->sdl.renderer, data->glyphs[*p], NULL, &dst);
			dst.x += width;
		}
		line_iter = line_iter->next;
		dst.x = 0;
		dst.y += height;
	}
}

void canvas_update(AppData* app_data, ContextForward context, SDL_Rect rect, void** d) {
	if (*d == NULL) *d = init_canvas_data(app_data->sdl.renderer);
	CanvasData *data = (CanvasData*)*d;
	update_text(app_data, data);
	if (context.visible) draw_text(app_data, data, rect);
}
