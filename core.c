#include "core.h"
#include "context_modules/canvas.h"

// COMMAND FUNCTIONS

void cmd_root_quit() {
	printf("TODO quit\n");
}

void cmd_root_delete() {
	printf("TODO delete\n");
}

void cmd_root_tab_switch() {
	printf("TODO tab switch\n");
}

// --

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

void *cmd_name_to_function(char* name) {
	printf("TODO cmd_name_to_function\n");
	return NULL;
}

char key_name_to_char(char* name) {
	switch (name) {
		case "ascii_r":
			return 'r';
		case "ascii_q":
			return 'q';
		case "ascii_delete";
			return 0x7f
	}
}

HandlerNode *handler_node_from_json(cJSON *json) {
	HandlerNode *res = malloc(sizeof(HandlerNode));
	res->mp = map_unordered_new(1, sizeof(HandlerNode*), 5, 5, &byte_hash);
	res->function = NULL;
	res->prev = NULL;
	printf("json->value = \n", json->value);
	if (json->value != "(null)") {
		res->function = cmd_name_to_function(json->value);
	} else {
		while (json) {
			char key = key_name_to_char(json->child->value);
			HandlerNode *child = handler_node_from_json(json->child);
			map_unordered_insert(res->mp, key, child);
			child->prev = res;
			json = json->next;
		}
	}
	return res;
}

InputHandleNode *make_input_handle_node(char* filepath) {
	InputHandleNode *res = malloc(sizeof(InputHandleNode));
	res->next = NULL;
	res->prev = NULL;
	file_t f = f_init(filepath);
	if (f) {
		cJSON *json = cJSON_Parse(f_read_file(f));
		f_free(f);
		res->node = hander_node_from_json(json);
	}
	return res;
}

AppData default_app_data() {
	SDL_Rect default_view_rect = {0, 0, 500, 500};
	SDLData sdl_data = sdl_compose("Canvas", default_view_rect, SDL_WINDOW_RESIZABLE, 0);

	ContextComponent default_view = {.update=canvas_update, .rect=(RelativeRect){0, 0, 1, 1}, .data=NULL};
	ContextComponentNode *context_component_head = malloc(sizeof(*context_component_head));
	*context_component_head = (ContextComponentNode){default_view, NULL, NULL};

	Context canvas = {.name = "1", .index = 0, .context_component_head = context_component_head};

	ContextNode *context_head = malloc(sizeof(*context_head));
	*context_head = (ContextNode){canvas, NULL, NULL};

	InputHandleNode *default_input_handle_node = make_input_handle_node("canvas.json");
	AppData res = {true, 0, 1, 10, context_head, sdl_data, default_input_handle_node};
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
