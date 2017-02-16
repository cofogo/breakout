#include <Text_Object.h>

Text_Object::Text_Object(const string& _s,
             TTF_Font* _fnt, SDL_Colour _col, SDL_Renderer* _ren,
             SDL_Rect _rect)
: m_tex(utils::load_txt_texture(_s, _fnt, _col, _ren))
, m_rect(_rect)
, m_col(_col)
, m_font(_fnt)
, m_ren(_ren)
{
	int w, h;
	SDL_QueryTexture(m_tex, NULL, NULL, &w, &h);
	m_rect.w = w;
	m_rect.h = h;
}

Text_Object::~Text_Object()
{
	TTF_CloseFont(m_font);
	m_font = NULL; // to be safe

	SDL_DestroyTexture(m_tex);
	m_tex = NULL; // to be safe
}

void Text_Object::render()
{
	SDL_RenderCopy(m_ren, m_tex, NULL, &m_rect);
}

void Text_Object::redraw(const string& _s)
{
	m_tex = utils::load_txt_texture(_s, m_font, m_col, m_ren);
	int w, h;
	SDL_QueryTexture(m_tex, NULL, NULL, &w, &h);
	m_rect.w = w;
	m_rect.h = h;
}

void Text_Object::set_x(int _x) {m_rect.x = _x;}
void Text_Object::set_y(int _y) {m_rect.y = _y;}
void Text_Object::set_xy(int _x, int _y) {m_rect.x = _x; m_rect.y = _y;}
