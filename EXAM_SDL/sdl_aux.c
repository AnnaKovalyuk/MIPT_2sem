#include "sdl_aux.h"
#include <stdio.h>
#include <SDL.h>

char const* Initialize(SDL_Window** window_indirect, SDL_Renderer** renderer_indirect, int width, int height, char const* ascii_title) {
	char const* error_message = "";

	SDL_ClearError();
	*window_indirect = SDL_CreateWindow(ascii_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (*(error_message = SDL_GetError())) goto END;
	SDL_ClearError();
	*renderer_indirect = SDL_CreateRenderer(*window_indirect, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (*(error_message = SDL_GetError())) goto END;

END:
	return error_message;
}

void Destroy(SDL_Window* window, SDL_Renderer* renderer) {
	if (NULL != renderer) SDL_DestroyRenderer(renderer);
	if (NULL != window) SDL_DestroyWindow(window);
}

void DrawCircle(SDL_Renderer* renderer, Circle const* circle) {
	for (int h = 0; h < circle->r; ++h) {
		int x_offset = (int)sqrt(circle->r * circle->r - h * h);
		SDL_RenderDrawLine(renderer, circle->cx - x_offset, circle->cy - h, circle->cx + x_offset, circle->cy - h);
		SDL_RenderDrawLine(renderer, circle->cx - x_offset, circle->cy + h, circle->cx + x_offset, circle->cy + h);
	}
}

Uint32 timer_function_draw_event(Uint32 interval, void* _params) {
	SDL_UserEvent userevent;
	userevent.type = SDL_USEREVENT;
	userevent.code = 0;

	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return interval;
}

void DrawScreen(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
}

void DrawScene(SDL_Renderer* renderer, Scene const* scene) {
	SDL_SetRenderDrawColor(renderer, 0x99, 0x33, 0xCC, 0xFF);
	DrawCircle(renderer, scene->circle);

	SDL_RenderPresent(renderer);
}

void map_model_to_scene(Model const* model, Scene* scene) {
	int circle_position_x = model->dx;
	int circle_position_y = model->dy;
	scene->circle->cx = circle_position_x;
	scene->circle->cy = circle_position_y;
	
}

void Energy(Model const* model) {
	printf("E=%f\n", model->qm * (model->vx * model->vx + model->vy * model->vy) / 2.);
}

void Model_Step(Model* model, float dt, bool * moving) {
	/*double A = dt; double B = dt * model->B / model->qm; //движение частицы при постоянной B, неявный метод Эйлера
	model->dx += model->vx * A/(B*B+1)+model->vy*A*B/(B * B + 1);
	model->dy += -model->vx * A*B / (B * B + 1) + model->vy * A / (B * B + 1);;
	float dvx = model->vx / (B * B + 1) + model->vy * B / (B * B + 1);
	float dvy = -model->vx * B / (B * B + 1) + model->vy / (B * B + 1);
	model->vx = dvx;
	model->vy = dvy;*/

	/*float dx0 = model->dx; //движение частицы в поле, индукция которого изменяется линейно/квардратично, мешанина методов
	float dy0 = model->dy;
	float dvxr = model->vx  + model->vy * dt / model->qm * (model->alpha + model->beta * sqrt(dx0 * dx0 + dy0 * dy0));
	float dvyr = model->vy - model->vx * dt / model->qm * (model->alpha + model->beta * sqrt(dx0 * dx0 + dy0 * dy0));
	model->dx = dx0 + dt * dvxr;
	model->dy = dy0 + dt * dvyr;
	float dvx = model->vx + dt / model->qm / 2 * ((model->alpha + model->beta * sqrt(dx0 * dx0 + dy0 * dy0))*model->vy + (model->alpha + model->beta * sqrt(model->dx * model->dx + model->dy * model->dy))*dvyr);
	float dvy = model->vy - dt / model->qm / 2 * ((model->alpha + model->beta * sqrt(dx0 * dx0 + dy0 * dy0)) * model->vx + (model->alpha + model->beta * sqrt(model->dx * model->dx + model->dy * model->dy)) * dvxr);
	model->vx = dvx;
	model->vy = dvy;*/

	/*float dx0 = model->dx; //движение частицы в поле, индукция которого изменяется линейно/квардратично, мешанина методов
	float dy0 = model->dy;
	float dvxr = model->vx + model->vy * dt * model->qm * ((model->alpha + model->beta * sqrt(dx0 * dx0 + dy0 * dy0) + model->gamma * (dx0 * dx0 + dy0 * dy0)));
	float dvyr = model->vy - model->vx * dt * model->qm * ((model->alpha + model->beta * sqrt(dx0 * dx0 + dy0 * dy0) + model->gamma * (dx0 * dx0 + dy0 * dy0)));
	model->dx = dx0 + dt * dvxr;
	model->dy = dy0 + dt * dvyr;
	float dvx = model->vx + dt * model->qm / 2 * ((model->alpha + model->beta * sqrt(dx0 * dx0 + dy0 * dy0) + model->gamma * (dx0 * dx0 + dy0 * dy0)) * model->vy + (model->alpha + model->beta * sqrt(model->dx * model->dx + model->dy * model->dy) + model->gamma * (model->dx * model->dx + model->dy * model->dy)) * dvyr);
	float dvy = model->vy - dt * model->qm / 2 * ((model->alpha + model->beta * sqrt(dx0 * dx0 + dy0 * dy0) + model->gamma * (dx0 * dx0 + dy0 * dy0)) * model->vx + (model->alpha + model->beta * sqrt(model->dx * model->dx + model->dy * model->dy) + model->gamma * (model->dx * model->dx + model->dy * model->dy)) * dvxr);
	model->vx = dvx;
	model->vy = dvy;*/
	
	float dx0 = model->dx;
	float dy0 = model->dy;
	float dvxr = model->vx + model->vy * dt * model->qm * (model->alpha + model->beta * dx0);
	float dvyr = model->vy - model->vx * dt * model->qm * (model->alpha + model->beta * dx0);
	model->dx = dx0 + dt * dvxr;
	model->dy = dy0 + dt * dvyr;
	float dvx = model->vx + dt * model->qm / 2 * ((model->alpha + model->beta * dx0) * model->vy + (model->alpha + model->beta * model->dx) * dvyr);
	float dvy = model->vy - dt * model->qm / 2 * ((model->alpha + model->beta * dx0) * model->vx + (model->alpha + model->beta * model->dx) * dvxr);
	model->vx = dvx;
	model->vy = dvy;

	if (model->dy < 0 || model->dy > 600 || model->dx < 0 || model->dx > 600) {
		moving = false;
		system("cls");
		printf("Instruction\n\n/Esc - Update the data\n/e - Show energy\n/Space - Pause/Continue\n/d - Clear console\n/c - Clear screen\n\n");
		printf("Enter qm, x0, y0, vx0, vy0:\n");
		scanf_s("%lf%lf%lf%lf%lf", &model->qm, &model->dx, &model->dy, &model->vx, &model->vy);
		system("cls");
		printf("Instruction\n\n/Esc - Update the data\n/e - Show energy\n/Space - Pause/Continue\n/d - Clear console\n/c - Clear screen\n\n");
	}
}

