#include "Paddle.h"

Paddle::Paddle(int _x, int _y)
{
	m_rect.x = _x;
	m_rect.y = _y;
	m_rect.w = 0;
	m_rect.h = 0;
}

Paddle::~Paddle()
{
	SDL_DestroyTexture(m_texture);
	m_texture = NULL;
}

void Paddle::assign_texture(SDL_Texture* _tex)
{
	m_texture = _tex;
	
	int tex_w, tex_h;
	SDL_QueryTexture(m_texture, NULL, NULL, &tex_w, &tex_h);
	
	m_rect.w = tex_w;
	m_rect.h = tex_h;
}

SDL_Texture* Paddle::get_texture()
{
	return m_texture;
}

void Paddle::render(SDL_Renderer* _ren)
{
	SDL_RenderCopy(_ren, m_texture, NULL, &m_rect);
}