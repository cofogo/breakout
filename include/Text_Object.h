#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <fstream>
using std::ifstream;

#include <utils.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Text_Object {
public:
	Text_Object(int _line_sep
	           , const string& _fnt_path, int _fnt_size
	           , SDL_Colour _col, SDL_Renderer* _ren
	           , SDL_Rect _rect);
	~Text_Object();

	void render();
	void render_stretched(SDL_Rect* _rec = NULL);
	void redraw();
	void set_text(vector<string>* _txt);
	void set_text_ln(unsigned _ln, const string& _s);
	void set_x(int _x);
	void set_y(int _y);
	void set_xy(int _x, int _y);
	vector<string>* get_text();
	SDL_Texture* make_txt_tex(vector<string>* _txt, TTF_Font* _fnt,
	             SDL_Color _col, SDL_Renderer* _ren);
	int load_file(const string& _fname);
	int save_file(const string& _fname);

private:
	SDL_Texture* m_tex;
	SDL_Rect m_rect;
	SDL_Colour m_col;
	TTF_Font* m_font;
	SDL_Renderer* m_ren;
	vector<string>* m_txt;
	int m_line_sep;
};
#endif //TEXT_OBJECT_H
