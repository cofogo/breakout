// implementation of tools defined in tools.h

#include <utils.h> //header for this implementarion

SDL_Texture* utils::load_txt_texture(
             string _s, TTF_Font* _font, SDL_Colour _col,
             SDL_Renderer* _ren)
{
	SDL_Surface* txt_surf = TTF_RenderText_Solid(_font, _s.c_str(), _col);
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
