#include "GameFunc.h"
#include <windows.h>
#include "SDL_image.h"

int g_input;

int f_state; //비행기 상태 //좌우상하 1234 멈춤 0
int f_x = 340;
int f_y = 340;

int m_state; //미사일 상태 //0일때 비행기 안, 1일때 발사
int m_x = f_x + 30;
int m_y = f_y + 10;

SDL_Texture* g_sky_sheet_texture;
SDL_Rect g_sky_rect;
SDL_Rect g_sdestination_rect;

SDL_Texture* g_flight_sheet_texture;
SDL_Rect g_flight_rect;
SDL_Rect g_fdestination_rect;

SDL_Texture* g_missile_sheet_texture;
SDL_Rect g_missile_rect;
SDL_Rect g_mdestination_rect;


/////////////////////////////////////////////////////////////

void InitGame() {
	g_input = 0;

	g_flag_running = true;




	//flight

	SDL_Surface* flight_sheet_surface = IMG_Load("../Resources/flight.jpg");
	SDL_SetColorKey(flight_sheet_surface, SDL_TRUE, SDL_MapRGB(flight_sheet_surface->format, 255, 255, 255));
	g_flight_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, flight_sheet_surface);
	SDL_FreeSurface(flight_sheet_surface);

	g_flight_rect.x = 165;
	g_flight_rect.y = 224;
	g_flight_rect.w = 683;
	g_flight_rect.h = 549;

	g_fdestination_rect.x = 100;
	g_fdestination_rect.y = 100;
	g_fdestination_rect.w = 120;
	g_fdestination_rect.h = 120;
	

	//missile
	SDL_Surface* missile_sheet_surface = IMG_Load("../Resources/missile.jpg");
	SDL_SetColorKey(missile_sheet_surface, SDL_TRUE, SDL_MapRGB(missile_sheet_surface->format, 255, 255, 255));
	g_missile_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, missile_sheet_surface);
	SDL_FreeSurface(missile_sheet_surface);

	g_missile_rect.x = 198;
	g_missile_rect.y = 64;
	g_missile_rect.w = 916;
	g_missile_rect.h = 1201;

	g_mdestination_rect.x = m_x;
	g_mdestination_rect.y = m_y;
	g_mdestination_rect.w = 60;
	g_mdestination_rect.h = 60;

	SDL_Surface* sky_sheet_surface = IMG_Load("../Resources/sky.jpg");
	g_sky_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, sky_sheet_surface);
	SDL_FreeSurface(sky_sheet_surface);

	//bg
	g_sky_rect.x = 0;
	g_sky_rect.y = 0;
	g_sky_rect.w = 600;
	g_sky_rect.h = 400;

	g_sdestination_rect.x = 0;
	g_sdestination_rect.y = 0;
	g_sdestination_rect.w = 800;
	g_sdestination_rect.h = 600;



}






/////////////////////////////////////////////////////////////

void Update() {

	if (f_state == 1) {
		f_x -= 5;
	}
	else if (f_state == 2) {
		f_x += 5;
	}
	else if (f_state == 3) {
		f_y -= 5;
	}
	else if (f_state == 4) {
		f_y += 5;
	}

	if (m_state == 0) {
		m_x = f_x + 30;
		m_y = f_y + 10;
	}

	else if (m_state == 1) {
		m_y -= 7; //위로 발사	
	}

	if (m_y <= -55) //미사일 소멸
		m_state = 0;


	if (f_x <= 0)
		f_x = 0;
	else if (f_x >= 680)
		f_x = 680;
	else if (f_y <= 0)
		f_y = 0;
	else if (f_y >= 480)
		f_y = 480;



	g_fdestination_rect.x = f_x;
	g_fdestination_rect.y = f_y;

	g_mdestination_rect.x = m_x;
	g_mdestination_rect.y = m_y;

}




/////////////////////////////////////////////////////////////

void Render() {

	//SDL_SetRenderDrawColor(g_renderer, 203, 242, 245, 255); //skyblue
	//SDL_RenderClear(g_renderer);
	SDL_RenderCopy(g_renderer, g_sky_sheet_texture, &g_sky_rect, &g_sdestination_rect);

	if (m_state == 1) {
		SDL_RenderCopy(g_renderer, g_missile_sheet_texture, &g_missile_rect, &g_mdestination_rect);
	}

	SDL_RenderCopy(g_renderer, g_flight_sheet_texture, &g_flight_rect, &g_fdestination_rect);

	SDL_RenderPresent(g_renderer);
}



/////////////////////////////////////////////////////////////

void HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				f_state = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				f_state = 2;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				f_state = 3;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				f_state = 4;
			}
			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				m_state = 1;
			}
			break;

		case SDL_KEYUP:
			f_state = 0;
			break;
		}
	}
}




/////////////////////////////////////////////////////////////

void ClearGame()
{
	SDL_DestroyTexture(g_sky_sheet_texture);
	SDL_DestroyTexture(g_flight_sheet_texture);
	SDL_DestroyTexture(g_missile_sheet_texture);
}