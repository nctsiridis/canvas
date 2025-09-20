#include "tab_stack.h"

void tab_stack_update(AppData* app_data) {
	SDL_SetRenderDrawColor(app_data->sdl_comp.renderer, 255, 0, 0, 255);
	SDL_Rect rect = {
		0, 0, 
		app_data->sdl_comp.window_width / 4, 
		app_data->sdl_comp.window_height
	};
	SDL_RenderDrawRect(app_data->sdl_comp.renderer, &rect);
	SDL_RenderFillRect(app_data->sdl_comp.renderer, &rect);
}

