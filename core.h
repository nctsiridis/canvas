#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

struct ContextNode;
struct ContextComponentNode;

typedef struct {
	int x_norm, y_norm;
	float w_norm, h_norm;
} RelativeRect;

typedef struct {
	int window_width, window_height;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event *event;
} SDLData;

typedef struct {
	bool running;
	int context_index; // int -> Widget *, use hash function
	int tab_count;
	int tab_size; // percentage
	struct ContextNode* context_head;
	SDLData sdl;
} AppData;

typedef struct ContextForward {
	bool visible;
	bool frame_update;
} ContextForward;

typedef struct {
	void (*update)(AppData *app_data, ContextForward context, RelativeRect rect, void** d);
	RelativeRect rect;
	void* data;
} ContextComponent;

typedef struct ContextComponentNode {
	ContextComponent context_component;
	struct ContextComponentNode *next;
	struct ContextComponentNode *prev;
} ContextComponentNode;

typedef struct {
	char* name;
	int index;
	ContextComponentNode* context_component_head;
} Context;

typedef struct ContextNode {
	Context context;
	struct ContextNode *next;
	struct ContextNode *prev;
} ContextNode;

typedef struct ContextBackward {
} ContextBackward;

SDLData sdl_compose(
	char* window_name,
	SDL_Rect rect,
	int window_flags,
	int renderer_flags
);
AppData default_app_data();
void close_sdl(AppData *app_data);
void render_base(AppData *app_data);
void context_update_all(AppData *app_data, ContextForward context);
void parse_sdl_event(AppData *app_data, SDL_Event *e, ContextForward *context);

#endif

