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

const int win_w = 800;
const int win_h = 640;

int init(); //initialise SDL
int create_main_win(SDL_Window*& _win, SDL_Surface*& _srf,
										const int& _w, const int& _h);
int create_win_renderer(SDL_Window* _win, SDL_Renderer*& _ren);
int load_media(vector<SDL_Surface*>& _srfs, SDL_Surface* _format);
int load_textures(vector<SDL_Texture*>& _texs, SDL_Renderer* _ren);
void close(SDL_Window*& _win, SDL_Renderer*& _ren,
           vector<SDL_Surface*>& _srfs, vector<SDL_Texture*> _texs);
SDL_Surface* load_surface(const string& _path);
SDL_Surface* optimise_surface(SDL_Surface* _srf, SDL_Surface* _format);
SDL_Texture* load_texture(const string& _path, SDL_Renderer* _ren);
void debug(const string& _s);

int main(int argc, char* args[])
{
	if(init() != 0) {
		cerr << "the application will now exit.\n";
		return 1;
	}
	
	SDL_Window* win_main = NULL;
	SDL_Surface* srf_main = NULL;
	SDL_Renderer* ren_main = NULL;
	
	vector<SDL_Surface*> srfs;
	vector<SDL_Texture*> texs;
	enum bmps {
		srf_action_x,
		srf_press_up, srf_press_dn, srf_press_lf, srf_press_rt
	};
	
	if(create_main_win(win_main, srf_main, win_w, win_h) != 0) {
		close(win_main, ren_main, srfs, texs);
		return 1;
	}
	
	if(create_win_renderer(win_main, ren_main) != 0) {
		close(win_main, ren_main, srfs, texs);
		return 1;
	}
	
	/*if(load_media(srfs, srf_main) != 0) {
		close(win_main, ren_main, srfs, texs);
		return 1;
	}*/
	
	if(load_textures(texs, ren_main) != 0) {
		close(win_main, ren_main, srfs, texs);
		return 1;
	}
	
	//main loop
	bool flag_quit = false;
	while(flag_quit == false) {
		SDL_Surface* srf_current = NULL;
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0) {
			if(event.type == SDL_QUIT) {
				flag_quit = true;
			}
			
			if(srf_current != NULL) {
				SDL_BlitSurface(srf_current, NULL, srf_main, NULL);
				SDL_UpdateWindowSurface(win_main);
			}
		}
	}
	
	//clear screen, copy texture on, refresh (present)
	for(unsigned short i = 0; i < win_h; ++i) {
		SDL_RenderClear(ren_main);
		SDL_RenderCopy(ren_main, texs[srf_action_x], NULL, NULL);
		
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
		SDL_Delay(5);
	}
	
	SDL_RenderPresent(ren_main);
	
	SDL_Delay(1500);
	
	close(win_main, ren_main, srfs, texs);
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

void close(SDL_Window*& _win, SDL_Renderer*& _ren,
           vector<SDL_Surface*>& _srfs, vector<SDL_Texture*> _texs)
{
	if(_win != NULL) {SDL_DestroyWindow(_win);}
	_win = NULL;
	
	if(_ren != NULL) {SDL_DestroyRenderer(_ren);}
	_ren = NULL;
	
	for(unsigned i = 0; i < 0; ++i) {
		if(_srfs[i] != NULL) {
			SDL_FreeSurface(_srfs[i]);
			_srfs[i] = NULL;
		}
		
		if(_texs[i] != NULL) {
			SDL_DestroyTexture(_texs[i]);
			_texs[i] = NULL;
		}
	}
	
	IMG_Quit();
	SDL_Quit();
}

int load_media(vector<SDL_Surface*>& _srfs, SDL_Surface* _format)
{	
	if(_srfs.size() > 0) {
		cerr << "ERROR: load_media received unclean array.\n";
		return 1;
	}
	if(_format == NULL) {
		cerr << "ERROR: load_media received NULL format.\n";
		return 1;
	}
	
	vector<string> fnames;
	fnames.push_back(string("action_x.png"));
	
	cout << "loading graphics assets... ";
	for(unsigned i = 0; i < fnames.size(); ++i) {
		SDL_Surface* temp_surf = load_surface(fnames[i]);
		_srfs.push_back(optimise_surface(temp_surf, _format));
		if(_srfs[i] == NULL) {
			return 1;
		}
	}
	cout << "DONE(" << _srfs.size() << " assets)\n";
	
	return 0;
}

int load_textures(vector<SDL_Texture*>& _texs, SDL_Renderer* _ren)
{
	if(_texs.size() > 0) {
		cerr << "load_textures received unclean array.\n";
		return 1;
	}
	if(_ren == NULL) {
		cerr << "load_textures received NULL renderer.\n";
		return 1;
	}
	
	//TODO fill the names array in a separate function
	//TODO consider using a map or an array of some double valued structure to store both the name and the data, but think about performance implications of doing so.
	vector<string> fnames;
	fnames.push_back(string("action_x.png"));
	
	cout << "loading textures... ";
	for(unsigned i = 0; i < fnames.size(); ++i) {
		_texs.push_back(load_texture(fnames[i], _ren));
		if(_texs[i] == NULL) {
			return 1;
		}
	}
	cout << "DONE(" << _texs.size() << " assets)\n";
	
	return 0;
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

SDL_Surface* optimise_surface(SDL_Surface* _srf, SDL_Surface* _format)
{
	SDL_Surface* optimised = NULL;
	
	optimised = SDL_ConvertSurface(_srf, _format->format, 0);
	if(optimised == NULL){
		cerr << "ERROR: Could not optimise surface!\n";
		cerr << "SDL error = " << SDL_GetError() << endl;
		return NULL;
	}
	
	return optimised;
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

void debug(const string& _s)
{
	cout << "DEBUG: " << _s << endl;
}
