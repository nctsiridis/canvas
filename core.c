#include "core.h"
#include "context_modules/canvas.h"

SDLComp sdl_compose(
	char* window_name,
	SDL_Rect rect,
	int window_flags,
	int renderer_flags
) {
	SDL_Window *window = SDL_CreateWindow(
		window_name,
		rect.x, rect.y, rect.w, rect.h,
		window_flags
	);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDLComp res = {window, renderer, rect.w, rect.h};
	return res;
}

AppData default_app_data() {
	SDL_Rect default_view_rect = {0, 0, 500, 500};
	SDLComp sdl_comp = sdl_compose("Canvas", default_view_rect, SDL_WINDOW_RESIZABLE, 0);

	View default_view = {.update=canvas_update, .rect=default_view_rect, .data=NULL};
	ViewNode *view_head = malloc(sizeof(*view_head));
	*view_head = (ViewNode){default_view, NULL, NULL};

	Context canvas = {.name = "Canvas", .index = 0, .view_head = view_head};

	ContextNode *context_head = malloc(sizeof(*context_head));
	*context_head = (ContextNode){canvas, NULL, NULL};

	AppData res = {true, 0, 1, 10, context_head, sdl_comp};
	return res;
}

void close_sdl(AppData* app_data) {
	SDL_DestroyRenderer(app_data->sdl_comp.renderer);
	SDL_DestroyWindow(app_data->sdl_comp.window);
}

void context_update_all(AppData* app_data) {
	ContextNode* context_iter = app_data->context_head;
	while (context_iter) {
		bool b = context_iter->context.index == app_data->context_index ? true : false;
		ViewNode* view_iter = context_iter->context.view_head;
		while (view_iter) {
			view_iter->view.update(app_data, b, view_iter->view.rect, view_iter->view.data);
			view_iter = view_iter->next;
		}
		context_iter = context_iter->next;
	}
}

void parse_sdl_event(AppData *app_data, SDL_Event *e) {
	if (e->type == SDL_QUIT) {
		app_data->running = false;
	} else if (e->type == SDL_WINDOWEVENT) {
		if (e->window.event == SDL_WINDOWEVENT_RESIZED) {
			app_data->sdl_comp.window_width = e->window.data1;
			app_data->sdl_comp.window_height = e->window.data2;
			printf("resized\n");
		}
	}
}
