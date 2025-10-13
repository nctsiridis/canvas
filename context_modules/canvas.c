#include "canvas.h"

#define ROWS_ON_SCREEN 30
#define COLS_ON_SCREEN 50

LineNode* make_new_line(int size) {
	LineNode *res = malloc(sizeof(LineNode));
	res->prev = NULL;
	res->next = NULL;
	res->text = calloc(size, sizeof(char));
	res->length = 0;
	return res;
}

CanvasData* init_canvas_data(AppData *app_data, RelativeRect rect) {
	SDL_Rect true_rect = {
		app_data->sdl.window_width * rect.x_norm,
		app_data->sdl.window_height * rect.y_norm,
		app_data->sdl.window_width * rect.w_norm,
		app_data->sdl.window_height * rect.h_norm
	};
	CanvasData *canvas_data = malloc(sizeof(CanvasData));
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	TTF_Font* font = TTF_OpenFont("res/short_baby.ttf", 20);
	if (!font) printf("Font creationg failed\n");
	canvas_data->rows_on_screen = ROWS_ON_SCREEN; // TODO
	canvas_data->cols_on_screen = COLS_ON_SCREEN;
	canvas_data->sym_width = true_rect.w / COLS_ON_SCREEN;
	canvas_data->sym_height = true_rect.h / ROWS_ON_SCREEN;
	canvas_data->line_pos = 0;
	canvas_data->inline_pos = 0;
	canvas_data->frame_pos = 0;
	canvas_data->glyphs = malloc(sizeof(SDL_Texture*) * 127);
	canvas_data->line = make_new_line(COLS_ON_SCREEN);
	canvas_data->rect = true_rect;

	for (int ch = 32; ch <= 126; ch++) {
		char str[2] = {ch, '\0'};
		SDL_Surface* surf = TTF_RenderText_Solid(font, str, color);
		canvas_data->glyphs[ch] = SDL_CreateTextureFromSurface(app_data->sdl.renderer, surf);
	}

	return canvas_data;
}

void update_text(AppData* app_data, CanvasData *data) {
	if (app_data->sdl.event->type == SDL_TEXTINPUT) {
		if (data->inline_pos < data->cols_on_screen - 1) {
			data->line->text[data->inline_pos] = app_data->sdl.event->text.text[0];
			data->line->length++;
			data->inline_pos++;
		}
	}
	if (app_data->sdl.event->type == SDL_KEYDOWN) {
		SDL_Keycode key = app_data->sdl.event->key.keysym.sym;
		if (key == SDLK_BACKSPACE) {
			if (data->inline_pos > 0) {
				data->inline_pos--;
				data->line->length--;
				data->line->text[data->inline_pos] = '\0';
			} else if (data->line->prev) {
				// TODO make delete line a helper
				LineNode *temp = data->line;
				data->line->prev->next = data->line->next;
				data->line = data->line->prev;
				data->inline_pos = data->line->length + 1;
				data->line_pos--;
				free(temp);
			}
		} else if (key == SDLK_RETURN) {
			if (data->inline_pos < data->cols_on_screen) {
				data->line->text[data->inline_pos] = '\n';
				if (data->line->next == NULL) data->line->next = make_new_line(data->cols_on_screen);
				data->line->next->prev = data->line;
				data->line = data->line->next;
				data->line_pos++;
				data->inline_pos = 0;
			}
		}
	}
}

void draw_cursor(AppData* app_data, CanvasData* data) {
	SDL_SetRenderDrawColor(app_data->sdl.renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(
		app_data->sdl.renderer, 
		&((SDL_Rect){
			data->rect.x + data->sym_width * data->inline_pos, 
			data->rect.y + data->sym_height * data->line_pos,
			data->sym_width, 
			data->sym_height
		})
	); // TODO Offset
}

void draw_text(AppData* app_data, CanvasData *data) {
	SDL_Rect dst_base = {
		data->rect.x, 
		data->rect.y + data->sym_height * data->line_pos, 
		data->sym_width, 
		data->sym_height
	};
	LineNode *line_iter_down = data->line;
	LineNode *line_iter_up = data->line->prev;

	SDL_Rect dst = dst_base;

	// draw down phase
	while (line_iter_down) {
		for (char *p = line_iter_down->text; *p; p++) {
			if (*p == '\n') break;
			if (*p < 32 || *p > 126) continue;
			SDL_RenderCopy(app_data->sdl.renderer, data->glyphs[*p], NULL, &dst);
			dst.x += data->sym_width;
		}
		line_iter_down = line_iter_down->next;
		dst.x = 0;
		dst.y += data->sym_height;
	}

	dst = dst_base;
	dst.y -= data->sym_height;

	// draw up phase
	while (line_iter_up) {
		for (char *p = line_iter_up->text; *p; p++) {
			if (*p == '\n') break;
			if (*p < 32 || *p > 126) continue;
			SDL_RenderCopy(app_data->sdl.renderer, data->glyphs[*p], NULL, &dst);
			dst.x += data->sym_width;
		}
		line_iter_up = line_iter_up->prev;
		dst.x = 0;
		dst.y -= data->sym_height;
	}
}

void canvas_update(AppData* app_data, ContextForward context, RelativeRect rect, void** d) {
	if (*d == NULL || context.frame_update) *d = init_canvas_data(app_data, rect);
	CanvasData *data = (CanvasData*)*d;
	update_text(app_data, data);
	if (context.visible) {
		draw_cursor(app_data, data);
		draw_text(app_data, data);
	}
}
