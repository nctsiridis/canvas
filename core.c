#include "core.h"
#include "context_modules/canvas.h"

SDLData sdl_compose(
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
	SDLData res = {rect.w, rect.h, window, renderer, NULL};
	return res;
}

AppData default_app_data() {
	SDL_Rect default_view_rect = {0, 0, 500, 500};
	SDLData sdl_data = sdl_compose("Canvas", default_view_rect, SDL_WINDOW_RESIZABLE, 0);

	ContextComponent default_view = {.update=canvas_update, .rect=default_view_rect, .data=NULL};
	ContextComponentNode *context_component_head = malloc(sizeof(*context_component_head));
	*context_component_head = (ContextComponentNode){default_view, NULL, NULL};

	Context canvas = {.name = "1", .index = 0, .context_component_head = context_component_head};

	ContextNode *context_head = malloc(sizeof(*context_head));
	*context_head = (ContextNode){canvas, NULL, NULL};

	AppData res = {true, 0, 1, 10, context_head, sdl_data};
	return res;
}

void close_sdl(AppData* app_data) {
	SDL_DestroyRenderer(app_data->sdl.renderer);
	SDL_DestroyWindow(app_data->sdl.window);
}

void context_update_all(AppData* app_data, ContextForward context) {
	ContextNode* context_iter = app_data->context_head;
	while (context_iter) {
		if (context_iter->context.index == app_data->context_index) context.visible = true;
		ContextComponentNode* context_component_iter = context_iter->context.context_component_head;
		while (context_component_iter) {
			context_component_iter->context_component.update(
				app_data, 
				context, 
				context_component_iter->context_component.rect, 
				&context_component_iter->context_component.data
			);
			context_component_iter = context_component_iter->next;
		}
		context_iter = context_iter->next;
	}
}

void parse_sdl_event(AppData *app_data, SDL_Event *e, ContextForward *context) {
	app_data->sdl.event = e; // allow other modules to handle the event TODO This is bullshit rewrite
	if (e->type == SDL_QUIT) {
		app_data->running = false;
	} else if (e->type == SDL_WINDOWEVENT) {
		if (e->window.event == SDL_WINDOWEVENT_RESIZED) {
			context->frame_update = true;
			app_data->sdl.window_width = e->window.data1;
			app_data->sdl.window_height = e->window.data2;
		}
	} 
}
