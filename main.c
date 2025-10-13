#include "core.h"

int main() {
	AppData app_data = default_app_data();

	if (!app_data.sdl.window || !app_data.sdl.renderer) {
		printf("Window or renderer creation failed\n");
		return 1;
	}

	if (TTF_Init() == -1) {
		printf("TTF_Init failed: %s\n", TTF_GetError());
		return 1;
	}

	SDL_StartTextInput();
	while (app_data.running) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		ContextForward context = {false, false};
		parse_sdl_event(&app_data, &e, &context);
		SDL_SetRenderDrawColor(app_data.sdl.renderer, 0, 0, 0, 255);
		SDL_RenderClear(app_data.sdl.renderer);
		context_update_all(&app_data, context);
		SDL_RenderPresent(app_data.sdl.renderer);
	}

	close_sdl(&app_data);
	return 0;
}
