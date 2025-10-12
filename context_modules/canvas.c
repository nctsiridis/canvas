#include "canvas.h"

CanvasData* init_canvas_data(SDL_Renderer* renderer) {
	CanvasData *canvas_data = malloc(sizeof(CanvasData));
	SDL_Color color = {0x00, 0x00, 0x00, 0xFF};
	SDL_Color background = {0xFF, 0xFF, 0xFF, 0xFF};
	TTF_Font* font = TTF_OpenFont("res/short_baby.ttf", 20);
	if (!font) printf("Font creationg failed\n");
	canvas_data->num_rows = 100;
	canvas_data->num_cols = 100;
	canvas_data->row_pos = 0;
	canvas_data->col_pos = 0;
	canvas_data->glyphs = malloc(sizeof(SDL_Texture*) * 127);
	canvas_data->text = "This is some example text\n";

	for (int ch = 32; ch <= 126; ch++) {
		char str[2] = {ch, '\0'};
		SDL_Surface* surf = TTF_RenderText_Shaded(font, str, color, background);
		canvas_data->glyphs[ch] = SDL_CreateTextureFromSurface(renderer, surf);
	}

	return canvas_data;
}

void canvas_update(AppData* app_data, bool draw, SDL_Rect rect, void** data) {
	if (*data == NULL) *data = init_canvas_data(app_data->sdl_comp.renderer);
	CanvasData *d = (CanvasData*)*data;
	SDL_Rect dst = {0, 0, 15, 15};
	for (char *p = d->text; *p; p++) {
		//printf("%d\n", *p);
		if (*p < 32 || *p > 126) continue;
		SDL_RenderCopy(app_data->sdl_comp.renderer, d->glyphs[*p], NULL, &dst);
		dst.x += 25;
	}
}
