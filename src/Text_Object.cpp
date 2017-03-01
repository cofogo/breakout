#include <Text_Object.h>

Text_Object::Text_Object(const string& _s,
             string _fnt_path, int _fnt_size,
             SDL_Colour _col, SDL_Renderer* _ren,
             SDL_Rect _rect)
: m_tex(NULL)
, m_rect(_rect)
, m_col(_col)
, m_font(NULL)
, m_ren(_ren)
{
	m_font = TTF_OpenFont(_fnt_path.c_str(), _fnt_size);
	if(m_font == NULL) {
		cerr << "WARNING: Could not open font.!\n";
		cerr << "TTF_Error: " << TTF_GetError() << endl;
		return;
	}

	m_tex = load_txt_texture(_s, m_font, _col, _ren);
	if(m_tex == NULL) {
		return;
	}

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

void Text_Object::render_stretched(SDL_Rect* _rec) //defaults to NULL
{
	SDL_RenderCopy(m_ren, m_tex, NULL, _rec);
}

void Text_Object::redraw(const string& _s)
{
	m_tex = load_txt_texture(_s, m_font, m_col, m_ren);
	int w, h;
	SDL_QueryTexture(m_tex, NULL, NULL, &w, &h);
	m_rect.w = w;
	m_rect.h = h;
}

void Text_Object::set_x(int _x) {m_rect.x = _x;}
void Text_Object::set_y(int _y) {m_rect.y = _y;}
void Text_Object::set_xy(int _x, int _y) {m_rect.x = _x; m_rect.y = _y;}

SDL_Texture* Text_Object::load_txt_texture(string _s, TTF_Font* _fnt,
	         SDL_Color _col, SDL_Renderer* _ren)
{
	SDL_Surface* txt_surf = TTF_RenderText_Solid(_fnt, _s.c_str(), _col);
	if(txt_surf == NULL) {
		cerr << "ERROR: unable to render text surface\n";
		cerr << "SDL_ttf error: " << TTF_GetError() << endl;
		return NULL;
	}

	SDL_Texture* txt_tex = SDL_CreateTextureFromSurface(_ren, txt_surf);
	if(txt_tex == NULL) {
		cerr << "ERROR: unable to create texture from rendered text\n";
		cerr << "SDL error: " << SDL_GetError() << endl;
		return NULL;
	}

	SDL_FreeSurface(txt_surf); //don't need the surface any more

	return txt_tex;
}
