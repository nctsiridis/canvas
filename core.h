#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

struct ContextNode;
struct ViewNode;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int window_width;
	int window_height;
} SDLComp;

typedef struct {
	bool running;
	int context_index; // int -> Widget *, use hash function
	int tab_count;
	int tab_size; // percentage
	struct ContextNode* context_head;
	SDLComp sdl_comp;
} AppData;

typedef struct {
	void (*update)(AppData *app_data, bool draw, SDL_Rect rect, void* data);
	SDL_Rect rect;
	void* data;
} View;

typedef struct ViewNode {
	View view;
	struct ViewNode *next;
	struct ViewNode *prev;
} ViewNode;

typedef struct {
	char* name;
	int index;
	ViewNode* view_head;
} Context;

typedef struct ContextNode {
	Context context;
	struct ContextNode *next;
	struct ContextNode *prev;
} ContextNode;

SDLComp sdl_compose(
	char* window_name,
	SDL_Rect rect,
	int window_flags,
	int renderer_flags
);
AppData default_app_data();
void close_sdl(AppData* app_data);
void render_base(AppData* app_data);
void context_update_all(AppData* app_data);
void parse_sdl_event(AppData *app_data, SDL_Event *e);

#endif

