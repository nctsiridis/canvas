#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>

// TODO would a linked list be better...

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDLComp;

typedef struct {
	bool running;
	int widget_index; // int -> Widget *, use hash function
	int tab_count;
	int tab_size; // percentage
	SDLComp sdl_comp;
} AppData;

typedef struct {
	char* name;
	int index;
	void (*update)(); // every widget needs to pass an update function
} Widget;

SDLComp sdl_compose(
	char* window_name,
	int start_pos_x, int start_pos_y,
	int window_width, int window_height,
	int window_flags,
	int renderer_flags
) {
	SDL_Window *window = SDL_CreateWindow(
		window_name,
		start_pos_x,
		start_pos_y,
		window_width, window_height,
		window_flags
	);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDLComp res = {window, renderer};
	return res;
}

AppData default_app_data() {
	SDLComp sdl_comp = sdl_compose(
		"Canvas",
		0, 0, // window pos x, y
		500, 500, // window height height
		SDL_WINDOW_RESIZABLE, // window flags
		0 // renderer falgs
	);

	AppData res = {true, 0, 1, 10, sdl_comp};
	return res;
}

void close_sdl(AppData* app_data) {
	SDL_DestroyRenderer(app_data->sdl_comp.renderer);
	SDL_DestroyWindow(app_data->sdl_comp.window);
}

void render_base(AppData* app_data) {
	// render tab stack
	SDL_SetRenderDrawColor(app_data->sdl_comp.renderer, 255, 0, 0, 255);
	SDL_Rect rect = {50, 50, 50, 50};
	SDL_RenderDrawRect(app_data->sdl_comp.renderer, &rect);
	SDL_RenderFillRect(app_data->sdl_comp.renderer, &rect);
	// render close button
}

void update_widget(AppData* app_data) {
	printf("Update widget\n");
}

int main() {
	AppData app_data = default_app_data();

	if (!app_data.sdl_comp.window || !app_data.sdl_comp.renderer) {
		printf("Window or renderer creation failed\n");
		return 1;
	}

	while (app_data.running) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		if (e.type == SDL_QUIT) app_data.running = false;
		SDL_SetRenderDrawColor(app_data.sdl_comp.renderer, 0, 0, 0, 255);
		SDL_RenderClear(app_data.sdl_comp.renderer);
		render_base(&app_data); // draw tabs, close button, dividers
		update_widget(&app_data); // handle widget related stuff and draw
		SDL_RenderPresent(app_data.sdl_comp.renderer);
	}

	close_sdl(&app_data);
	return 0;
}
