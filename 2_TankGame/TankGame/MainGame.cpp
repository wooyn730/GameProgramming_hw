#include "SDL.h"
#include <iostream>
#include "GameFunc.h"
#include "TankGameIntro.h"
#include "TankGameStage1.h"
#include "TankGameEnding.h"

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

	g_window = SDL_CreateWindow("First Window", 100, 100, 800, 600, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);

	InitGame();
	Init_Intro();
	Init_Stage1();
	Init_Ending();

	g_current_game_phase = PHASE_INTRO;

	g_bg_mus = Mix_LoadMUS("../Resources/UntilThisGame.mp3");
	if (g_bg_mus == 0) {
		std::cout << "Mix_LoadMUS(\"UntilThisGame.mp3\"): " << Mix_GetError() << std::endl;
	}
	Mix_VolumeMusic(128);

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
		else if (g_current_game_phase == PHASE_STAGE1)
		{
			Mix_PauseMusic(); //����� �Ͻ�����
			HandleEvents_Stage1();
			Update_Stage1();
			Render_Stage1();
		}
		else if (g_current_game_phase == PHASE_ENDING)
		{
			Mix_ResumeMusic(); //����� �ٽ����
			HandleEvents_Ending();
			Update_Ending();
			Render_Ending();
		}

		g_last_time_ms = cur_time_ms;

	}

	Clear_Intro();
	Clear_Stage1();
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


