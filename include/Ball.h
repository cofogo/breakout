#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Object.h"

class Ball: public Object
{
public:
	Ball(SDL_Rect _rect, short _spd, short _angle);
	~Ball();
	
	void move(short _x_max, short _y_max);
	void bounce(short _angle);
	void calc_speed(short _angle);
	
private:
	short m_total_speed;
	short m_vector_angle;
	short m_x_speed, m_y_speed;
};

#endif //BALL_H
