#include "GameFunc.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;

int main(int argc, char* argv[]) {

	// Initializing SDL library
	SDL_Init(SDL_INIT_EVERYTHING);
	g_window = SDL_CreateWindow("First Window", 100, 100, 800, 600, 0);


	g_renderer = SDL_CreateRenderer(g_window, -1, 0);

	InitGame();

	g_last_time_ms = SDL_GetTicks();

	while (g_flag_running) {

		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < 33)
			continue;

		HandleEvents();
		Update();
		Render();

		// SDL_Delay(30);	// 30 �и������� ��ٸ���.
		g_last_time_ms = cur_time_ms;
	}

	ClearGame();
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	ClearGame();

	return 0;
}


