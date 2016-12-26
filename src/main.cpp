#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Paddle.h"

const int win_w = 800;
const int win_h = 640;

int init(); //initialise SDL
int create_main_win(SDL_Window*& _win, SDL_Surface*& _srf,
										const int& _w, const int& _h);
int create_win_renderer(SDL_Window* _win, SDL_Renderer*& _ren);
void close(SDL_Window*& _win, SDL_Renderer*& _ren);
SDL_Surface* load_surface(const string& _path);
SDL_Texture* load_texture(const string& _path, SDL_Renderer* _ren);

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
	
	//TODO move game logic out of main
	Paddle paddle0(300, 550);
	paddle0.assign_texture(load_texture("assets/gfx/paddle.png", ren_main));
	if(paddle0.get_texture() == 0) cout << "WARNING: paddle texture NULL!\n";
	
	//main loop
	bool flag_quit = false;
	SDL_SetRenderDrawColor(ren_main, 0x00, 0x20, 0x20, 0xff);
	while(flag_quit == false) {
		SDL_Surface* srf_current = NULL;
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0) {
			if(event.type == SDL_QUIT) {
				flag_quit = true;
			}
			
			//render sequence
			SDL_RenderClear(ren_main);
			
			paddle0.render(ren_main);
			
			SDL_RenderPresent(ren_main);
		}
	}
	
	//clear screen, copy texture on, refresh (present)
	//TODO decide if there should be an exit animaiton, make a proper one if yes
	for(unsigned short i = 0; i < win_h; ++i) {
		SDL_SetRenderDrawColor(ren_main, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(ren_main);
		//SDL_RenderCopy(ren_main, texs[srf_action_x], NULL, NULL);
		
		//draw primitives
		SDL_Rect shape_rect0 = {i, (win_h / 2) + 20, win_w / 10, win_h / 10};
		SDL_Rect shape_rect1 = {win_w - i, (win_h / 2) - 20, 
		                        (win_w / 2) - i, (win_h / 2) - i};
		SDL_SetRenderDrawColor(ren_main, 0xcc, 0xcc, 0xff, 0xff);
		SDL_RenderFillRect(ren_main, &shape_rect0);
		
		SDL_SetRenderDrawColor(ren_main, 0x00, 0x00, 0xff, 0xff);
		SDL_RenderDrawRect(ren_main, &shape_rect1);
	
		SDL_SetRenderDrawColor(ren_main, 0xff, 0x00, 0x00, 0xff);
		SDL_RenderDrawLine(ren_main, 0, i, win_w, i);
		
		SDL_SetRenderDrawColor(ren_main, 0x55, 0xaa, 0x55, 0xff);
		unsigned short dot_line_x = (i * 2 < win_w)? (i * 2) : (i * 2) - win_w;
		for(unsigned short j = 0; j < win_h; j += 10) {
			SDL_RenderDrawPoint(ren_main, dot_line_x, j);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 1);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 3);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 5);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 6);
		}
		++dot_line_x;
		SDL_SetRenderDrawColor(ren_main, 0x55, 0xaa, 0x55, 0xff);
		for(unsigned short j = 0; j < win_h; j += 10) {
			SDL_RenderDrawPoint(ren_main, dot_line_x, j);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 1);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 3);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 5);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 6);
		}
		++dot_line_x;
		SDL_SetRenderDrawColor(ren_main, 0x55, 0xaa, 0x55, 0xff);
		for(unsigned short j = 0; j < win_h; j += 10) {
			SDL_RenderDrawPoint(ren_main, dot_line_x, j);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 1);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 3);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 5);
			SDL_RenderDrawPoint(ren_main, dot_line_x, j + 6);
		}
		
		SDL_RenderPresent(ren_main);
		SDL_Delay(3);
	}
	
	
	SDL_RenderPresent(ren_main);
	
	SDL_Delay(200);
	
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
		cout << "ERROR: FATAL - could not initialise SDL_image!\n";
		cerr << "SDL error = " << IMG_GetError() << endl;
		return 1;
	}
	
	return 0;
}

int create_main_win(SDL_Window*& _win, SDL_Surface*& _srf,
										const int& _w, const int& _h)
{
	_win = SDL_CreateWindow("hello SDL",
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