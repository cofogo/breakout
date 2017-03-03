#include <Text_Object.h>

Text_Object::Text_Object(int _line_sep,
             const string& _fnt_path, int _fnt_size,
             SDL_Colour _col, SDL_Renderer* _ren,
             SDL_Rect _rect)
: m_tex(NULL)
, m_rect(_rect)
, m_col(_col)
, m_font(NULL)
, m_ren(_ren)
, m_line_sep(_line_sep)
{
	m_font = TTF_OpenFont(_fnt_path.c_str(), _fnt_size);
	if(m_font == NULL) {
		cerr << "WARNING: Could not open font.!\n";
		cerr << "TTF_Error: " << TTF_GetError() << endl;
		return;
	}
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

void Text_Object::redraw()
{
	m_tex = make_txt_tex(m_text, m_font, m_col, m_ren);
	int w, h;
	SDL_QueryTexture(m_tex, NULL, NULL, &w, &h);
	m_rect.w = w;
	m_rect.h = h;
}

void Text_Object::set_text_ln(unsigned _ln, const string& _s)
{
	while(_ln >= m_text.size()) {
		m_text.push_back(" ");
	}

	m_text[_ln] = _s;
}

void Text_Object::set_x(int _x) {m_rect.x = _x;}
void Text_Object::set_y(int _y) {m_rect.y = _y;}
void Text_Object::set_xy(int _x, int _y) {m_rect.x = _x; m_rect.y = _y;}

SDL_Texture* Text_Object::make_txt_tex(const vector<string>& _t,
             TTF_Font* _fnt,
	         SDL_Color _col, SDL_Renderer* _ren)
{
	int line_max_w = 0;
	int line_h = TTF_FontHeight(_fnt);
	vector<SDL_Texture*> texs; //TODO declare with overhead specified
	vector<SDL_Rect> rects;

	for(int i = 0; i < _t.size(); ++i) {
		SDL_Surface* txt_surf = TTF_RenderText_Solid(_fnt, _t[i].c_str(), _col);
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

		rects.push_back(SDL_Rect{0, (line_h + m_line_sep) * i
		                        , txt_surf->w, txt_surf->h});

		if(txt_surf->w > line_max_w) {line_max_w = txt_surf->w;}
		SDL_FreeSurface(txt_surf); //don't need the surface any more

		texs.push_back(txt_tex);
	}

	//TODO destroy unused textures

	int final_h = ((line_h + m_line_sep) * _t.size()) - m_line_sep;
	SDL_Texture* final_tex = SDL_CreateTexture(_ren, SDL_PIXELFORMAT_RGBA8888
	             , SDL_TEXTUREACCESS_TARGET
	             , line_max_w, final_h);
	SDL_SetTextureBlendMode(final_tex, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(_ren, final_tex	);
	SDL_SetRenderDrawColor(_ren, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(_ren);
	for(unsigned i = 0; i < texs.size(); ++i) {
		SDL_RenderCopy(_ren, texs[i], NULL, &rects[i]);
		//copied texture no longer neccessary - freeing memory
		SDL_DestroyTexture(texs[i]);
		texs[i] = NULL;
	}
	SDL_SetRenderTarget(_ren, NULL);

	return final_tex;
}
