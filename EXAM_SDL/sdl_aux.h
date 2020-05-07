#pragma once
#include <SDL.h>
#include <stdbool.h>

char const* Initialize(SDL_Window** window_indirect, SDL_Renderer** renderer_indirect, int width, int height, char const* ascii_title);
void Destroy(SDL_Window* window, SDL_Renderer* renderer);

typedef struct {
	int cx, cy, r;
} Circle;
typedef struct {
	Circle* circle;
} Scene;
typedef struct {
	double alpha, beta, gamma;
	double qm;
	double dx, dy, vx, vy;
} Model;

void DrawCircle(SDL_Renderer* renderer, Circle const* circle);

Uint32 timer_function_draw_event(Uint32 interval, void* _params);

void DrawScreen(SDL_Renderer* renderer);

void DrawScene(SDL_Renderer* renderer, Scene const* scene);

void map_model_to_scene(Model const* model, Scene* scene);

void Energy(Model const* model);

void Model_Step(Model* model, float dt, bool * moving);

