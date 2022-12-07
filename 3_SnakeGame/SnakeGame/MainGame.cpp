#include "SDL.h"
#include <iostream>
#include "GameFunc.h"
#include "phase.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;

int g_current_game_phase;
Mix_Music* g_bg_mus;

int main(int argc, char* argv[]) {

	// Initializing SDL library
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}

	g_window = SDL_CreateWindow("Snake Game", 100, 100, 780, 600, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);

	InitGame();
	Init_Intro();
	Init_Stage();
	Init_Ending();

	g_current_game_phase = PHASE_INTRO;

	g_bg_mus = Mix_LoadMUS("../Resources/bg_ms.mp3");
	if (g_bg_mus == 0) {
		std::cout << "Mix_LoadMUS(\"bg_ms.mp3\"): " << Mix_GetError() << std::endl;
	}
	Mix_VolumeMusic(80);

	Mix_PlayMusic(g_bg_mus, 1);

	g_last_time_ms = SDL_GetTicks();

	while (g_flag_running)
	{

		Uint32 cur_time_ms = SDL_GetTicks();
		if (cur_time_ms - g_last_time_ms < 33)
			continue;

		if (g_current_game_phase == PHASE_INTRO)
		{
			HandleEvents_Intro();
			Update_Intro();
			Render_Intro();
		}
		else if (g_current_game_phase == PHASE_STAGE)
		{
			HandleEvents_Stage();
			Update_Stage();
			Render_Stage();
		}
		else if (g_current_game_phase == PHASE_ENDING)
		{
			HandleEvents_Ending();
			Update_Ending();
			Render_Ending();
		}

		g_last_time_ms = cur_time_ms;

	}

	Clear_Intro();
	Clear_Stage();
	Clear_Ending();
	ClearGame();

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	Mix_FreeMusic(g_bg_mus);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();

	return 0;
}


