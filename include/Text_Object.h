#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include <string>
using std::string;

#include <utils.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Text_Object {
public:
	Text_Object(const string& _s,
	            string _fnt_path, int _fnt_size,
	            SDL_Colour _col, SDL_Renderer* _ren,
	            SDL_Rect _rect);
	~Text_Object();

	void render();
	void render_stretched(SDL_Rect* _rec = NULL);
	void redraw(const string& _s);
	void set_x(int _x);
	void set_y(int _y);
	void set_xy(int _x, int _y);
	SDL_Texture* load_txt_texture(string _s, TTF_Font* _fnt,
	             SDL_Color _col, SDL_Renderer* _ren);

private:
	SDL_Texture* m_tex;
	SDL_Rect m_rect;
	SDL_Colour m_col;
	TTF_Font* m_font;
	SDL_Renderer* m_ren;
};
#endif //TEXT_OBJECT_H
