#include "core.h"

int main() {
	AppData app_data = default_app_data();

	if (!app_data.sdl_comp.window || !app_data.sdl_comp.renderer) {
		printf("Window or renderer creation failed\n");
		return 1;
	}

	while (app_data.running) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		parse_sdl_event(&app_data, &e);
		SDL_SetRenderDrawColor(app_data.sdl_comp.renderer, 0, 0, 0, 255);
		SDL_RenderClear(app_data.sdl_comp.renderer);
		context_update_all(&app_data); // handle widget related stuff and draw
		SDL_RenderPresent(app_data.sdl_comp.renderer);
	}

	close_sdl(&app_data);
	return 0;
}
