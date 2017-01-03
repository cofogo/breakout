#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Paddle
{
public:
	Paddle(int _x, int _y);
	~Paddle();
	
	void assign_texture(SDL_Texture* _tex);
	SDL_Texture* get_texture();
	void render(SDL_Renderer* _ren);
	void move_l(const int _amt);
	void move_r(const int _amt);
	
private:
	SDL_Texture* m_texture;
	SDL_Rect m_rect;
};

#endif //PADDLE_H