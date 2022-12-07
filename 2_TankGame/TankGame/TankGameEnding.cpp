#include "GameFunc.h"
#include "TankGameEnding.h"

static SDL_Texture* g_texture_ending; // the SDL_Texture 
static SDL_Rect g_source_rectangle_ending; // the rectangle for source image
static SDL_Rect g_destination_rectangle_ending; // for destination


void Init_Ending()
{
	SDL_Surface* temp_surface = IMG_Load("../Resources/ending.jpg");
	g_texture_ending = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(g_texture_ending, NULL, NULL, &g_source_rectangle_ending.w, &g_source_rectangle_ending.h);
	g_destination_rectangle_ending.x = g_source_rectangle_ending.x = 0;
	g_destination_rectangle_ending.y = g_source_rectangle_ending.y = 0;
	g_destination_rectangle_ending.w = g_source_rectangle_ending.w;
	g_destination_rectangle_ending.h = g_source_rectangle_ending.h;

}

void Update_Ending()
{
}


void Render_Ending()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color

	SDL_RenderCopy(g_renderer, g_texture_ending, &g_source_rectangle_ending, &g_destination_rectangle_ending);

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void HandleEvents_Ending()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				g_current_game_phase = PHASE_INTRO;
			}
			break;

		default:
			break;
		}
	}
}


void Clear_Ending()
{
	SDL_DestroyTexture(g_texture_ending);
}