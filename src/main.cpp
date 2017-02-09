#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Timer.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"

const int win_w = 800;
const int win_h = 640;

int init(); //initialise SDL
int create_main_win(SDL_Window*& _win, SDL_Surface*& _srf,
                    const int _w, const int _h);
int create_win_renderer(SDL_Window* _win, SDL_Renderer*& _ren);
void close(SDL_Window*& _win, SDL_Renderer*& _ren);
SDL_Surface* load_surface(const string& _path);
SDL_Texture* load_texture(const string& _path, SDL_Renderer* _ren);
vector<shared_ptr<SDL_Texture>> load_textures(SDL_Renderer* _ren);
void run_game(SDL_Renderer* _ren, const int _win_w, const int _win_h,
              vector<shared_ptr<SDL_Texture>>* _texs);
void outro(SDL_Renderer* _ren, const int _win_w, const int _win_h);
int check_loss(SDL_Rect* _r, const int _max_h);

int main(int argc, char* args[])
{
	if(init() != 0) {
		cerr << "the application will now exit.\n";
		return 1;
	}
	
	SDL_Window* win_main = NULL;
	SDL_Surface* srf_main = NULL;
	SDL_Renderer* ren_main = NULL;
	
	if(create_main_win(win_main, srf_main, win_w, win_h) != 0) {
		close(win_main, ren_main);
		return 1;
	}
	
	if(create_win_renderer(win_main, ren_main) != 0) {
		close(win_main, ren_main);
		return 1;
	}
	
	vector<shared_ptr<SDL_Texture>> texs = load_textures(ren_main);
	if(texs.size() == 0) {
		return 1;
	}
	
	run_game(ren_main, win_w, win_h, &texs);
	
	outro(ren_main, win_w, win_h);
	
	close(win_main, ren_main);
	return 0;
}

int init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << "ERROR: FATAL - could not initialise SDL!\n";
		cerr << "SDL error = " << SDL_GetError() << endl;
		return 1;
	}
	
	int sdl_image_flags = IMG_INIT_PNG;
	if(!(IMG_Init(sdl_image_flags) & sdl_image_flags)) {
		cerr << "ERROR: FATAL - could not initialise SDL_image!\n";
		cerr << "SDL error = " << IMG_GetError() << endl;
		return 1;
	}
	
	return 0;
}

int create_main_win(SDL_Window*& _win, SDL_Surface*& _srf,
										const int _w, const int _h)
{
	_win = SDL_CreateWindow("breakout42",
	                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                        _w, _h,
	                         SDL_WINDOW_SHOWN);
	if(_win == NULL) {
		cerr << "ERROR: FATAL - SDL window could not be created!\n";
		cerr << "SDL error = " << SDL_GetError() << endl;
		return 1;
	}
	
	_srf = SDL_GetWindowSurface(_win);
	
	return 0;
}

int create_win_renderer(SDL_Window* _win, SDL_Renderer*& _ren)
{
	_ren = SDL_CreateRenderer(_win, -1, SDL_RENDERER_ACCELERATED);
	
	if(_ren == NULL) {
		cerr << "ERROR: Could not create renderer!\n";
		cerr << "SDL error = " << SDL_GetError() << endl;
		return 1;
	}
	
	//initialise renderer colour
	SDL_SetRenderDrawColor(_ren, 0xff, 0xff, 0xff, 0xff);
	
	return 0;
}

void close(SDL_Window*& _win, SDL_Renderer*& _ren)
{
	if(_win != NULL) {SDL_DestroyWindow(_win);}
	_win = NULL;
	
	if(_ren != NULL) {SDL_DestroyRenderer(_ren);}
	_ren = NULL;
	
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* load_surface(const string& _path)
{
	SDL_Surface* loaded = IMG_Load(_path.c_str());
	if(loaded == NULL){
		cerr << "ERROR: Could not load picture!\n";
		cerr << "file path: " << _path << endl;
		cerr << "SDL error = " << IMG_GetError() << endl;
		return NULL;
	}
	
	return loaded;
}

SDL_Texture* load_texture(const string& _path, SDL_Renderer* _ren)
{
	SDL_Texture* tex = NULL;
	
	SDL_Surface* tmp_surf = load_surface(_path);
	if(tmp_surf == NULL) {
		return NULL;
	}
	
	tex = SDL_CreateTextureFromSurface(_ren, tmp_surf);
	if(tex == NULL) {
		cerr << "ERROR: Could not create texture!\n";
		cerr << "file path: " << _path << endl;
		cerr << "SDL error = " << SDL_GetError() << endl;
		return NULL;
	}
	
	SDL_FreeSurface(tmp_surf);
	
	return tex;
}

void run_game(SDL_Renderer* _ren, const int _win_w, const int _win_h,
              vector<shared_ptr<SDL_Texture>>* _texs)
{
	//init game
	Timer loop_timer;
	const short fps = 60;
	Uint32 tgt_frame_len = 1000 / fps;
	
	Paddle paddle0(SDL_Rect{300, 600}, 7);
	Ball ball(SDL_Rect{400, 300}, 9, 45);
	vector<shared_ptr<Brick>> bricks;
	for(unsigned short i = 0; i < 10; ++i) {
		for(unsigned short j = 0; j < 1; ++j) {
			int x = 20 + (i * 60);
			int y = 20 + (j * 30);
			bricks.push_back(shared_ptr<Brick> (new Brick(SDL_Rect{x, y})));
		}
	}
	
	paddle0.assign_texture((*_texs)[0]);
	ball.assign_texture((*_texs)[1]);

	unsigned short lives0 = 3;

	for(unsigned short i = 0; i < bricks.size(); ++i) {
		bricks[i]->assign_texture((*_texs)[2]);
	}

	//main loop
	bool flag_quit = false;
	SDL_SetRenderDrawColor(_ren, 0x00, 0x20, 0x20, 0xff);
	while(flag_quit == false) {
		loop_timer.set_start(SDL_GetTicks());
		
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0) {
			if(event.type == SDL_QUIT) {
				flag_quit = true;
			}
		}
		
		//input and update
		const Uint8* key_states = SDL_GetKeyboardState(NULL);
		if(key_states[SDL_SCANCODE_LEFT]) {
			paddle0.move_l();
		}
		else if(key_states[SDL_SCANCODE_RIGHT]) {
			paddle0.move_r();
		}
		
		ball.update(win_w, win_h, &bricks, paddle0.get_rect());
		if(check_loss(ball.get_rect(), win_h)) {
			//if ball lost, decrement lives, end game if lives == 0
			if(--lives0 == 0) {
				flag_quit = true;
				cout << "GAME OVER!\nTRY AGAIN ;)\n"; //TODO game over graphics
				break;
			}
			cerr << "lives: " << lives0 << endl; //TODO print lives on GUI
			ball = Ball(SDL_Rect{400, 300}, 9, 45);
			ball.assign_texture((*_texs)[1]);
		}

		if(bricks.size() == 0) {
			flag_quit = true;
			cout << "YOU WIN!\n"; //TODO print on GUI
			break;
		}

		//render sequence
		loop_timer.set_end(SDL_GetTicks());
		Uint32 wait_len = tgt_frame_len - loop_timer.get_duration();
		if(wait_len > 0) {
			SDL_Delay(wait_len);
		}
		SDL_RenderClear(_ren);
		
		paddle0.render(_ren);
		ball.render(_ren);
		for(unsigned short i = 0; i < bricks.size(); ++i) {
			bricks[i]->render(_ren);
		}
		
		SDL_RenderPresent(_ren);
	}
}

void outro(SDL_Renderer* _ren, const int _win_w, const int _win_h)
{
	//clear screen, copy texture on, refresh (present)
	//TODO decide if there should be an exit animaiton, make a proper one if yes
	for(unsigned short i = 0; i < _win_h; ++i) {
		SDL_SetRenderDrawColor(_ren, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(_ren);
		//SDL_RenderCopy(_ren, texs[srf_action_x], NULL, NULL);
		
		//draw primitives
		SDL_Rect shape_rect0 = {i, (_win_h / 2) + 20, _win_w / 10, _win_h / 10};
		SDL_Rect shape_rect1 = {_win_w - i, (_win_h / 2) - 20, 
		                        (_win_w / 2) - i, (_win_h / 2) - i};
		SDL_SetRenderDrawColor(_ren, 0xcc, 0xcc, 0xff, 0xff);
		SDL_RenderFillRect(_ren, &shape_rect0);
		
		SDL_SetRenderDrawColor(_ren, 0x00, 0x00, 0xff, 0xff);
		SDL_RenderDrawRect(_ren, &shape_rect1);
	
		SDL_SetRenderDrawColor(_ren, 0xff, 0x00, 0x00, 0xff);
		SDL_RenderDrawLine(_ren, 0, i, _win_w, i);
		
		//draw wide dashed line
		SDL_SetRenderDrawColor(_ren, 0x55, 0xaa, 0x55, 0xff);
		unsigned short dot_line_x = (i * 2 < _win_w)? (i * 2) : (i * 2) - _win_w;
		for(unsigned short j = 0; j < _win_h; j += 10) {
			SDL_RenderDrawPoint(_ren, dot_line_x, j);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 1);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 3);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 5);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 6);
		}
		++dot_line_x;
		SDL_SetRenderDrawColor(_ren, 0x55, 0xaa, 0x55, 0xff);
		for(unsigned short j = 0; j < _win_h; j += 10) {
			SDL_RenderDrawPoint(_ren, dot_line_x, j);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 1);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 3);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 5);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 6);
		}
		++dot_line_x;
		SDL_SetRenderDrawColor(_ren, 0x55, 0xaa, 0x55, 0xff);
		for(unsigned short j = 0; j < _win_h; j += 10) {
			SDL_RenderDrawPoint(_ren, dot_line_x, j);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 1);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 3);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 5);
			SDL_RenderDrawPoint(_ren, dot_line_x, j + 6);
		}
		
		SDL_RenderPresent(_ren);
		SDL_Delay(3);
	}
	
	SDL_RenderPresent(_ren);
	SDL_Delay(200);	
}

vector<shared_ptr<SDL_Texture>> load_textures(SDL_Renderer* _ren) {
	vector<shared_ptr<SDL_Texture>> texs;
	try {
		//0
		SDL_Texture* tex = load_texture("assets/gfx/paddle.png", _ren);
		shared_ptr<SDL_Texture> shared_tex;
		shared_tex.reset(tex, SDL_DestroyTexture);
		texs.push_back(std::move(shared_tex));
		//1
		tex = load_texture("assets/gfx/ball.png", _ren);
		shared_tex.reset(tex, SDL_DestroyTexture);
		texs.push_back(shared_tex);
		//2
		tex = load_texture("assets/gfx/brick.png", _ren);
		shared_tex.reset(tex, SDL_DestroyTexture);
		texs.push_back(shared_tex);
	}
	catch(const char* err_msg) {
		cerr << err_msg << endl;
		cerr << "The program will now quit.\n";
		
		return vector<shared_ptr<SDL_Texture>>(0);
	}
	
	return texs;
}

int check_loss(SDL_Rect* _r, const int _max_y)
{
	if(_r->y > _max_y) {return 1;}

	return 0;
}
