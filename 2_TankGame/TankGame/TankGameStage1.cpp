#include "GameFunc.h"
#include "TankGameStage1.h"
#include <Windows.h>

int t_state; //��ũ ������ //�¿���� 1234, ���� 0
int t_dir; //��ũ ����
int t_x;
int t_y;

//�̻��� �迭 5����
int m_state[5]; //�̻��� ���� //0-�⺻, 1-�߻�
int m_dir[5]; //�߻� ����
int m_x[5];
int m_y[5];
int m_cnt; //���� �̻��� ���� (�ִ� 5 �ּ� 0)
int m_id; //�̻��� �ε���

char count[10];
POINT ptMouse;

SDL_Texture* g_sky_sheet_texture;
SDL_Rect g_sky_rect;
SDL_Rect g_sdestination_rect;

SDL_Texture* g_tank_sheet_texture;
SDL_Rect g_tank_rect;
SDL_Rect g_tdestination_rect;

SDL_Texture* g_missile_sheet_texture;
SDL_Rect g_missile_rect;
SDL_Rect g_mdestination_rect[5];

Mix_Chunk* g_m_shoot;

SDL_Texture* g_cnt_n;
SDL_Rect g_cnt_n_rect;
SDL_Rect tmp_r;

/////////////////////////////////////////////////////////////

void Init_Stage1() {

	//�ʱ� ����
	g_flag_running = true;
	t_dir = 3;
	t_x = 350;
	t_y = 250;
	for (int i = 0; i < 5; i++)
		m_state[i] = 0;
	m_cnt = 5;
	m_id = 0;


	///////////////////////////
	//��ũ
	g_tank_rect.x = 0;
	g_tank_rect.y = 0;
	g_tank_rect.w = 318;
	g_tank_rect.h = 483;

	g_tdestination_rect.w = 100;
	g_tdestination_rect.h = 100;

	//�̻���
	g_missile_rect.x = 251;
	g_missile_rect.y = 543;
	g_missile_rect.w = 589;
	g_missile_rect.h = 603;

	for (int i = 0; i < 5; i++) {
		g_mdestination_rect[i].w = 30;
		g_mdestination_rect[i].h = 30;
	}


	////////////////////////////
	//ȿ����
	g_m_shoot = Mix_LoadWAV("../Resources/shoot.mp3");
	Mix_VolumeMusic(128); //0~128

}



/////////////////////////////////////////////////////////////

void Update_Stage1() {

	//tank
	SDL_Surface* tank_sheet_surface = IMG_Load("../Resources/tank.png");
	g_tank_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, tank_sheet_surface);
	SDL_FreeSurface(tank_sheet_surface);

	//missile
	SDL_Surface* missile_sheet_surface = IMG_Load("../Resources/missile.png");
	SDL_SetColorKey(missile_sheet_surface, SDL_TRUE, SDL_MapRGB(missile_sheet_surface->format, 255, 255, 255));
	g_missile_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, missile_sheet_surface);
	SDL_FreeSurface(missile_sheet_surface);


	/////////////////////////
	//�̻���
	for (int i = 0; i < 5; i++) {
		//�̻��� �߻� ��
		if (m_state[i] == 0) {		//�¿����
			if (t_dir == 1) {
				m_x[i] = t_x + 50;
				m_y[i] = t_y + 35;
			}
			else if (t_dir == 2) {
				m_x[i] = t_x + 20;
				m_y[i] = t_y + 35;
			}
			else if (t_dir == 3) {
				m_x[i] = t_x + 35;
				m_y[i] = t_y + 50;
			}
			else if (t_dir == 4) {
				m_x[i] = t_x + 35;
				m_y[i] = t_y + 20;
			}
		}

		//�̻��� �߻�
		else if (m_state[i] == 1) {
			if (m_dir[i] == 1) {
				m_x[i] -= 10; //��
			}
			else if (m_dir[i] == 2) {
				m_x[i] += 10; //��
			}
			else if (m_dir[i] == 3) {
				m_y[i] -= 10; //���� �߻�
			}
			else if (m_dir[i] == 4) {
				m_y[i] += 10; //��
			}

			//�̻��� �Ҹ� -> m_state = 0;
			if (m_x[i] <= -20 || m_x[i] >= 820 || m_y[i] <= -20 || m_y[i] >= 620)
				m_state[i] = 0;

			g_mdestination_rect[i].x = m_x[i];
			g_mdestination_rect[i].y = m_y[i];
		}
	}

	//////////////////////////////
	//��ũ �̵�
	if (t_state == 1) {
		t_x -= 5; //��
	}
	else if (t_state == 2) {
		t_x += 5; //��
	}
	else if (t_state == 3) {
		t_y -= 5; //��
	}
	else if (t_state == 4) {
		t_y += 5; //��
	}

	//��ũ ���� ����
	if (t_x <= 10)
		t_x = 10;
	if (t_x >= 690)
		t_x = 690;
	if (t_y <= 10)
		t_y = 10;
	if (t_y >= 490)
		t_y = 490;

	g_tdestination_rect.x = t_x;
	g_tdestination_rect.y = t_y;


	//////////////////////////////
	TTF_Font* font1 = TTF_OpenFont("../Resources/DungGeunMo.ttf", 45);
	SDL_Color red = { 255, 0, 0, 0 };

	sprintf_s(count, sizeof(count), "%d", m_cnt);
	SDL_Surface* count_surface = TTF_RenderText_Blended(font1, count, red);

	g_cnt_n_rect.x = 0;
	g_cnt_n_rect.y = 0;
	g_cnt_n_rect.w = count_surface->w;
	g_cnt_n_rect.h = count_surface->h;

	//cnt ��ġ
	if (t_dir == 1) //��
	{
		tmp_r.x = t_x + 110;
		tmp_r.y = t_y + 30;
	}
	else if (t_dir == 2) //��
	{
		tmp_r.x = t_x - 30;
		tmp_r.y = t_y + 30;
	}
	else if (t_dir == 4) //��
	{
		tmp_r.x = t_x + 40;
		tmp_r.y = t_y - 50;
	}
	else //��
	{
		tmp_r.x = t_x + 40;
		tmp_r.y = t_y + 100;
	}

	g_cnt_n = SDL_CreateTextureFromSurface(g_renderer, count_surface);

	//����
	SDL_FreeSurface(count_surface);
	TTF_CloseFont(font1);
}




/////////////////////////////////////////////////////////////

void Render_Stage1() {

	//���
	SDL_SetRenderDrawColor(g_renderer, 120, 100, 0, 255);	//Ȳ���
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color

	//////////////////
	//�̻���
	for (int i = 0; i < 5; i++) {
		if (m_state[i] == 1) {
			SDL_RenderCopy(g_renderer, g_missile_sheet_texture, &g_missile_rect, &g_mdestination_rect[i]);
		}
	}

	//////////////////
	//��ũ (����)
	if (t_dir == 1) //��
		SDL_RenderCopyEx(g_renderer, g_tank_sheet_texture, &g_tank_rect, &g_tdestination_rect, 270, 0, SDL_FLIP_NONE);
	else if (t_dir == 2) //��
		SDL_RenderCopyEx(g_renderer, g_tank_sheet_texture, &g_tank_rect, &g_tdestination_rect, 90, 0, SDL_FLIP_NONE);
	else if (t_dir == 4) //��
		SDL_RenderCopyEx(g_renderer, g_tank_sheet_texture, &g_tank_rect, &g_tdestination_rect, 180, 0, SDL_FLIP_NONE);
	else //3, ��
		SDL_RenderCopy(g_renderer, g_tank_sheet_texture, &g_tank_rect, &g_tdestination_rect);
	
	//////////////////
	//��Ʈ
	{	
		tmp_r.w = g_cnt_n_rect.w;
		tmp_r.h = g_cnt_n_rect.h;
		SDL_RenderCopy(g_renderer, g_cnt_n, &g_cnt_n_rect, &tmp_r);
	}

	SDL_RenderPresent(g_renderer);

	//���� �� ���� �ؽ�ó ���� -> �޸� ���� ����
	SDL_DestroyTexture(g_tank_sheet_texture);
	SDL_DestroyTexture(g_missile_sheet_texture);
}



/////////////////////////////////////////////////////////////

void HandleEvents_Stage1()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {

		switch (event.type)
		{
		case SDL_QUIT:
			break;

		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (m_cnt > 0) {
					m_state[m_id] = 1;
					if (t_dir == 1)
						m_dir[m_id] = 1;
					else if (t_dir == 2)
						m_dir[m_id] = 2;
					else if (t_dir == 3)
						m_dir[m_id] = 3;
					else if (t_dir == 4)
						m_dir[m_id] = 4;
					Mix_PlayChannel(-1, g_m_shoot, 0);
					m_cnt--;
					m_id++;
				}
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) {
				if (m_cnt == 0 && m_state[0] == 0 && m_state[1] == 0 && m_state[2] == 0 && m_state[3] == 0 && m_state[4] == 0) { //������ �̻��� 0��, ��� �̻��� ȭ�� ��
					GetCursorPos(&ptMouse);
					if (t_x <= ptMouse.x && (t_x + 200) >= ptMouse.x && t_y <= ptMouse.y && (t_y + 200) >= ptMouse.y) { //��ũ �׸� ���� ���콺 ������ ��ġ	
						m_cnt = 5;
						m_id = 0;
					}
				}
			}

			break;
		}

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				t_state = 1;
				t_dir = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				t_state = 2;
				t_dir = 2;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				t_state = 3;
				t_dir = 3;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				t_state = 4;
				t_dir = 4;
			}
			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				Init_Stage1();
				g_current_game_phase = PHASE_ENDING;
			}
			break;
		case SDL_KEYUP:
			t_state = 0;
		}
	}
}




/////////////////////////////////////////////////////////////

void Clear_Stage1()
{
	SDL_DestroyTexture(g_tank_sheet_texture);
	SDL_DestroyTexture(g_missile_sheet_texture);
	Mix_FreeChunk(g_m_shoot);
	SDL_DestroyTexture(g_cnt_n);
}