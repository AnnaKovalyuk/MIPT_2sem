#include "sdl_aux.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int agc, char** argv) {
	printf("Instruction\n\n/Esc - Update the data\n/e - Show energy\n/Space - Pause/Continue\n/d - Clear console\n/c - Clear screen\n\n");
	double alpha, beta, gamma;
	printf("The equation for induction is B = alpha + beta * r + gamma * r^2\nPlease enter alpha, beta and gamma:\n");
	scanf_s("%lf%lf%lf", &alpha, &beta, &gamma);
	system("cls");
BEGIN:
	if (0 != SDL_Init(SDL_INIT_VIDEO)) {
		printf("Can't initialize SDL.\nCause: %s\n", SDL_GetError());
		goto END;
	}

	{
		SDL_Window* main_window = NULL;
		SDL_Renderer* main_renderer = NULL;
		SDL_TimerID timer_id;

		printf("Instruction\n\n/Esc - Update the data\n/e - Show energy\n/Space - Pause/Continue\n/d - Clear console\n/c - Clear screen\n\n");
		double qm, x0, y0, vx0, vy0;
		printf("Enter qm, x0, y0, vx0, vy0:\n");
		scanf_s("%lf%lf%lf%lf%lf", &qm, &x0, &y0, &vx0, &vy0);
		system("cls");
		printf("Instruction\n\n/Esc - Update the data\n/e - Show energy\n/Space - Pause/Continue\n/d - Clear console\n/c - Clear screen\n\n");

		int const SCREEN_WIDTH = 600, SCREEN_HEIGHT = 600;
		const char* error_message = "";

		if (*(error_message = Initialize(&main_window, &main_renderer, SCREEN_WIDTH, SCREEN_HEIGHT, "SDL Render Draw Example"))) {
			printf("Can't create widnow or renderer.\nCause: %s\n", error_message);
			goto CLEANUP;
		}
		DrawScreen(main_renderer);

		Model model = { alpha, beta, gamma, qm, x0, y0, vx0, vy0 };
		double dt = 2.5e-3;

		Circle circle = { 400, 300, 2 };
		Scene scene = { &circle};

		timer_id = SDL_AddTimer(20, timer_function_draw_event, NULL);

		SDL_Event e;
		bool energy = false;
		bool moving = false;
		bool proceed = true;
		while (proceed) {
			if (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT:
				{
					proceed = false;
					break;
				}

				case SDL_USEREVENT:
				{
					if (moving) if (energy) Energy(&model);
					map_model_to_scene(&model, &scene);
					DrawScene(main_renderer, &scene);
					break;
				}

				case SDL_KEYDOWN:
				{
					SDL_KeyboardEvent key = e.key;
					switch (key.keysym.sym) {
					case SDLK_SPACE:{
						moving = !moving;
						break;
					}
					case SDLK_e:{
						energy = !energy;
						break;
					}
					case SDLK_ESCAPE:{
						moving = false;
						Destroy(main_window, main_renderer);
						SDL_RemoveTimer(timer_id);
						SDL_Quit();
						system("cls");
						goto BEGIN;
						break;
					}
					case SDLK_d:{
						system("cls");
						printf("Instruction\n\n/Esc - Update the data\n/e - Show energy\n/Space - Pause/Continue\n/d - Clear console\n/c - Clear screen\n\n");
						break;
					}
					case SDLK_c:{
						DrawScreen(main_renderer);
						break;
					}
					case SDLK_UP:{
						if (!moving) model.dy -= 5;
						break;
					}
					case SDLK_DOWN:{
						if (!moving) model.dy += 5;
						break;
					}
					case SDLK_RIGHT:{
						if (!moving) model.dx += 5;
						break;
					}
					case SDLK_LEFT:{
						if (!moving) model.dx -= 5;
						break;
					}
					}
					break;
				}
				}
			}
			if (moving)
				Model_Step(&model, dt, &moving);
				

		}

	CLEANUP:
		Destroy(main_window, main_renderer);
		SDL_RemoveTimer(timer_id);
	}

END:
	SDL_Quit();
	return 0;
}
