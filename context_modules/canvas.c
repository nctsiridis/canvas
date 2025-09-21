#include "canvas.h"

CanvasData* init_canvas_data(SDL_Renderer* renderer) {
	CanvasData *canvas_data = malloc(sizeof(CanvasData));

	SDL_Color color = {255, 255, 255, 255};
	TTF_Font* font = TTF_OpenFont("res/DejaVuSans.ttf", 24);

	SDL_Texture *glyphs[127];
	for (int ch = 32; ch <= 126; ch++) {
		char str[2] = {ch, '\0'};
		SDL_Surface* surf = TTF_RenderText_Blended(font, str, color);
		glyphs[ch] = SDL_CreateTextureFromSurface(renderer, surf);
	}

	*canvas_data = (CanvasData) {
		.num_rows=100, .num_cols=100, 
		.row_pos=0, .col_pos=0, 
		.glyphs=glyphs, .text=""
	};

	return canvas_data;
}

void canvas_update(AppData* app_data, bool draw, SDL_Rect rect, void* data) {
	printf("looking\n");
	if (data == NULL) data = init_canvas_data(app_data->sdl_comp.renderer);
	CanvasData* d = (CanvasData*) data;
	printf("canvas data: %d, %d, %d, %d\n", d->num_rows, d->num_cols, d->row_pos, d->col_pos);
}
