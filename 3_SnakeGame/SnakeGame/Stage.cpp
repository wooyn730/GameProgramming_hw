#include "GameFunc.h"
#include "phase.h"

//배경
static SDL_Texture* g_texture_Stage; // the SDL_Texture 
static SDL_Rect g_source_rectangle_Stage; // the rectangle for source image
static SDL_Rect g_destination_rectangle_Stage; // for destination

//뱀
static SDL_Texture* g_tex_head;
static SDL_Texture* g_tex_body;
static SDL_Rect g_source_rect_body;
static SDL_Rect g_des_rect_body[100];

//개구리
static SDL_Texture* g_tex_frog;
static SDL_Rect g_source_rect_frog;
static SDL_Rect g_des_rect_frog;


//독개구리
static SDL_Texture* g_tex_redfrog;
static SDL_Rect g_source_rect_redfrog;
static SDL_Rect g_des_rect_redfrog;

int count;
char cnt_buffer[20];
SDL_Texture* g_cnt_tex;
SDL_Rect g_cnt_rect;

int s_move; //탱크 움직임 //좌우상하 1234, 멈춤 0

Mix_Chunk* g_m_sip;

class Frog
{
public:
	Frog() //파라미터 없는 생성자는 초기화
	{
		x = 0;
		y = 0;
	}
	Frog(int x_, int y_) //파라미터 있는 생성자
	{
		x = x_;
		y = y_;
	}
	int x;
	int y;
};

class RedFrog
{
public:
	RedFrog()
	{
		x = 0;
		y = 0;
	}
	RedFrog(int x_, int y_)
	{
		x = x_;
		y = y_;
	}
	int x;
	int y;
};

class Node //뱀의 몸통
{
public:
	int x;
	int y;
	Node* next;
};

Node* head = nullptr;
Frog frog;
RedFrog redfrog;

void AddNodeAtFront(int x_, int y_)
{
	Node* n = new Node;
	n->next = head;
	head = n;
	head->x = x_;
	head->y = y_;
}

void DeleteTail()
{

	Node* n = head;
	while (n->next->next != nullptr)
	{
		n = n->next;
	}
	delete n->next;
	n->next = nullptr;
}


void EatFrog()
{
	count++;
	Mix_PlayChannel(-1, g_m_sip, 0);

	// 몸통 늘어나기
	AddNodeAtFront(frog.x, frog.y);

	for (int i = count; i > 0; i--)
	{
		g_des_rect_body[i].x = g_des_rect_body[i - 1].x;
		g_des_rect_body[i].y = g_des_rect_body[i - 1].y;
	}

	SDL_Surface* temp3_surface = IMG_Load("../Resources/body.png");
	g_tex_body = SDL_CreateTextureFromSurface(g_renderer, temp3_surface);
	SDL_FreeSurface(temp3_surface);

	SDL_QueryTexture(g_tex_body, NULL, NULL, &g_source_rect_body.w, &g_source_rect_body.h);
	g_des_rect_body[count].x = frog.x;
	g_des_rect_body[count].y = frog.y;
	g_des_rect_body[count].w = g_source_rect_body.w;
	g_des_rect_body[count].h = g_source_rect_body.h;

	// 개구리 다시
	frog.x = (rand() % 20 + 2) * 30; //(3~24)
	frog.y = (rand() % 12 + 4) * 30; //(5~18)

	// 몸통 위치에 생기면 또다시
	for (int i = count; i > 0; i--)
	{
		if (frog.x == g_des_rect_body[i].x && frog.y == g_des_rect_body[i].y)
		{
			frog.x = (rand() % 20 + 2) * 30; //(3~24)
			frog.y = (rand() % 12 + 4) * 30; //(5~18)
		}
		else if (frog.x == redfrog.x && frog.y == redfrog.y)
		{
			frog.x = (rand() % 20 + 2) * 30; //(3~24)
			frog.y = (rand() % 12 + 4) * 30; //(5~18)
		}

		if (count % 3 == 0) {
			redfrog.x = (rand() % 20 + 2) * 30; //(3~24)
			redfrog.y = (rand() % 12 + 4) * 30; //(5~18)
			if (redfrog.x == g_des_rect_body[i].x && redfrog.y == g_des_rect_body[i].y)
			{
				redfrog.x = (rand() % 20 + 2) * 30; //(3~24)
				redfrog.y = (rand() % 12 + 4) * 30; //(5~18)
			}
			g_des_rect_redfrog.x = redfrog.x;
			g_des_rect_redfrog.y = redfrog.y;
		}
	}
	g_des_rect_frog.x = frog.x;
	g_des_rect_frog.y = frog.y;
}


void Init_Stage()
{
	// background
	SDL_Surface* temp_surface = IMG_Load("../Resources/bg.png");
	g_texture_Stage = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(g_texture_Stage, NULL, NULL, &g_source_rectangle_Stage.w, &g_source_rectangle_Stage.h);
	g_destination_rectangle_Stage.x = g_source_rectangle_Stage.x = 0;
	g_destination_rectangle_Stage.y = g_source_rectangle_Stage.y = 0;
	g_destination_rectangle_Stage.w = g_source_rectangle_Stage.w;
	g_destination_rectangle_Stage.h = g_source_rectangle_Stage.h;
	
	// Frog
	frog.x = (rand() % 20 + 2) * 30; //(3~24)
	frog.y = (rand() % 12 + 4) * 30; //(5~18)

	SDL_Surface* frog_surface = IMG_Load("../Resources/frog.png");
	g_tex_frog = SDL_CreateTextureFromSurface(g_renderer, frog_surface);
	SDL_FreeSurface(frog_surface);

	SDL_QueryTexture(g_tex_frog, NULL, NULL, &g_source_rect_frog.w, &g_source_rect_frog.h);
	g_des_rect_frog.x = frog.x;
	g_des_rect_frog.y = frog.y;
	g_des_rect_frog.w = g_source_rect_frog.w;
	g_des_rect_frog.h = g_source_rect_frog.h;

	// Red Frog
	redfrog.x = (rand() % 20 + 2) * 30; //(3~24)
	redfrog.y = (rand() % 12 + 4) * 30; //(5~18)

	SDL_Surface* redfrog_surface = IMG_Load("../Resources/red_frog.png");
	g_tex_redfrog = SDL_CreateTextureFromSurface(g_renderer, redfrog_surface);
	SDL_FreeSurface(redfrog_surface);

	SDL_QueryTexture(g_tex_redfrog, NULL, NULL, &g_source_rect_redfrog.w, &g_source_rect_redfrog.h);
	g_des_rect_redfrog.x = redfrog.x;
	g_des_rect_redfrog.y = redfrog.y;
	g_des_rect_redfrog.w = g_source_rect_redfrog.w;
	g_des_rect_redfrog.h = g_source_rect_redfrog.h;

	// Snake
	AddNodeAtFront(150, 210);

	SDL_Surface* head_surface = IMG_Load("../Resources/head.png");
	g_tex_head = SDL_CreateTextureFromSurface(g_renderer, head_surface);
	SDL_FreeSurface(head_surface);

	SDL_Surface* body_surface = IMG_Load("../Resources/body.png");
	g_tex_body = SDL_CreateTextureFromSurface(g_renderer, body_surface);
	SDL_FreeSurface(body_surface);

	SDL_QueryTexture(g_tex_head, NULL, NULL, &g_source_rect_body.w, &g_source_rect_body.h);
	g_des_rect_body[0].x = head->x;
	g_des_rect_body[0].y = head->y;
	g_des_rect_body[0].w = g_source_rect_body.w;
	g_des_rect_body[0].h = g_source_rect_body.h;

	s_move = 0;
	count = 0;

	g_m_sip = Mix_LoadWAV("../Resources/sip.mp3");
	Mix_VolumeMusic(128); //0~128
}

void Update_Stage()
{
	// 뱀 움직임
	if (s_move == 1) //좌
	{
		if (head->x == (frog.x + 30) && head->y == frog.y)
		{
			EatFrog();
		}
		else if (head->x == (redfrog.x + 30) && head->y == redfrog.y)
		{
			Clear_Stage();
			Init_Ending();
			Sleep(120);
			g_current_game_phase = PHASE_ENDING;
		}
		else {
			Sleep(120);
			head->x -= 30;
		}
	}
	else if (s_move == 2) //우
	{
		if (head->x == (frog.x - 30) && head->y == frog.y)
		{
			EatFrog();
		}
		else if (head->x == (redfrog.x - 30) && head->y == redfrog.y)
		{
			Clear_Stage();
			Init_Ending();
			Sleep(120);
			g_current_game_phase = PHASE_ENDING;
		}
		else {
			Sleep(120);
			head->x += 30;
		}
	}
	else if (s_move == 3) //상
	{
		if (head->x == frog.x && head->y == (frog.y + 30))
		{
			EatFrog();
		}
		else if (head->x == redfrog.x && head->y == (redfrog.y + 30))
		{
			Clear_Stage();
			Init_Ending();
			Sleep(120);
			g_current_game_phase = PHASE_ENDING;
		}
		else {
			Sleep(120);
			head->y -= 30;
		}
	}
	else if (s_move == 4) //하
	{
		if (head->x == frog.x && head->y == (frog.y - 30))
		{
			EatFrog();
		}
		else if (head->x == redfrog.x && head->y == (redfrog.y - 30))
		{
			Clear_Stage();
			Init_Ending();
			Sleep(120);
			g_current_game_phase = PHASE_ENDING;
		}
		else {
			Sleep(120);
			head->y += 30;
		}
	}

	for (int i = count; i > 0; i--)
	{
		g_des_rect_body[i].x = g_des_rect_body[i-1].x;
		g_des_rect_body[i].y = g_des_rect_body[i-1].y;
	}
	g_des_rect_body[0].x = head->x;
	g_des_rect_body[0].y = head->y;
	

	// 벽에 닿으면 죽기
	if (head->x < 60 || head->x > 23 * 30 || head->y < 120 || head->y > 17 * 30)
	{
		Clear_Stage();
		Init_Ending();
		Sleep(120);
		g_current_game_phase = PHASE_ENDING;
	}

	// 몸통에 닿으면 죽기
	for (int i = count; i > 0; i--)
	{
		if (head->x == g_des_rect_body[i].x && head->y == g_des_rect_body[i].y)
		{
			Clear_Stage();
			Init_Ending();
			Sleep(120);
			g_current_game_phase = PHASE_ENDING;
		}
	}

	// 점수
	TTF_Font* font1 = TTF_OpenFont("../Resources/Kyobo Handwriting 2019.ttf", 40);
	SDL_Color black = { 0, 0, 0, 0 };

	sprintf_s(cnt_buffer, sizeof(cnt_buffer), "SCORE : %04d", count*10);
	SDL_Surface* score_surface = TTF_RenderText_Blended(font1, cnt_buffer, black);

	g_cnt_rect.x = 0;
	g_cnt_rect.y = 0;
	g_cnt_rect.w = score_surface->w;
	g_cnt_rect.h = score_surface->h;

	g_cnt_tex = SDL_CreateTextureFromSurface(g_renderer, score_surface);
	//해제
	SDL_FreeSurface(score_surface);
	TTF_CloseFont(font1);

}


void Render_Stage()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color

	SDL_RenderCopy(g_renderer, g_texture_Stage, &g_source_rectangle_Stage, &g_destination_rectangle_Stage);

	if (s_move == 1) //좌
		SDL_RenderCopyEx(g_renderer, g_tex_head, &g_source_rect_body, &g_des_rect_body[0], 90, 0, SDL_FLIP_NONE);
	else if (s_move == 2) //우
		SDL_RenderCopyEx(g_renderer, g_tex_head, &g_source_rect_body, &g_des_rect_body[0], 270, 0, SDL_FLIP_NONE);
	else if (s_move == 3) //상
		SDL_RenderCopyEx(g_renderer, g_tex_head, &g_source_rect_body, &g_des_rect_body[0], 180, 0, SDL_FLIP_NONE);
	else
		SDL_RenderCopy(g_renderer, g_tex_head, &g_source_rect_body, &g_des_rect_body[0]);
	
	for (int i=1; i<=count; i++)
		SDL_RenderCopy(g_renderer, g_tex_body, &g_source_rect_body, &g_des_rect_body[i]);

	SDL_RenderCopy(g_renderer, g_tex_frog, &g_source_rect_frog, &g_des_rect_frog);
	SDL_RenderCopy(g_renderer, g_tex_redfrog, &g_source_rect_redfrog, &g_des_rect_redfrog);

	{	//폰트
		SDL_Rect tmp_r;	//새로 위치 지정
		tmp_r.x = 270;
		tmp_r.y = 50;
		tmp_r.w = g_cnt_rect.w;
		tmp_r.h = g_cnt_rect.h;
		SDL_RenderCopy(g_renderer, g_cnt_tex, &g_cnt_rect, &tmp_r);
	}

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void HandleEvents_Stage()
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
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				s_move = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				s_move = 2;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				s_move = 3;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				s_move = 4;
			}
			break;

		default:
			break;
		}
	}
}


void Clear_Stage()
{
	SDL_DestroyTexture(g_texture_Stage);
	SDL_DestroyTexture(g_tex_head);
	SDL_DestroyTexture(g_tex_body);
	SDL_DestroyTexture(g_tex_frog);
	SDL_DestroyTexture(g_tex_redfrog);
	SDL_DestroyTexture(g_cnt_tex);
	Mix_FreeChunk(g_m_sip);
}