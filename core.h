#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <map_unordered.h>
#include <cJSON.h>
#include <fileutils.h>

struct HandlerNode;
struct ContextNode;
struct ContextComponent;
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

typedef struct HandlerNode {
	struct HandlerNode *prev;
	MapUnordered *mp;
	void (*function); // NULL if none
} HandlerNode;

typedef struct InputHandleNode {
	struct InputHandleNode *next, *prev;
	HandlerNode *node;
} InputHandleNode;

typedef struct {
	bool running;
	int context_index; // int -> Widget *, use hash function
	int tab_count;
	int tab_size; // percentage
	struct ContextNode *context_head;
	SDLData sdl;
	InputHandleNode *input_handler;
} AppData;


typedef struct ContextComponent {
	bool visible;
	void (*update)(AppData *app_data, struct ContextComponent *d);
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


SDLData sdl_compose(
	char* window_name,
	SDL_Rect rect,
	int window_flags,
	int renderer_flags
);
AppData default_app_data();
void close_sdl(AppData *app_data);
void render_base(AppData *app_data);
void context_update_all(AppData *app_data);
void parse_sdl_event(AppData *app_data);

#endif

