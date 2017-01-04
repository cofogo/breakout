#include "Paddle.h"

// Paddle::Paddle(const int _x, const int _y, const int _s)
Paddle::Paddle(SDL_Rect _rect, const int _s)
: Object(_rect), m_speed(_s)
{
}

Paddle::~Paddle()
{
}

void Paddle::move_l()
{
	m_rect.x -= m_speed;
}

void Paddle::move_r()
{
	m_rect.x += m_speed;
}

void Paddle::set_speed(const int _amt)
{
	m_speed = _amt;
}
