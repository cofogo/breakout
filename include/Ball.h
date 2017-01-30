#ifndef BALL_H
#define BALL_H

#include <cmath>
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Object.h"
#include "Brick.h"

class Ball: public Object
{
public:
	Ball(SDL_Rect _rect, short _spd, short _angle);
	~Ball();
	
	void update(short _x_max, short _y_max,
	     vector<shared_ptr<Brick>>* _obsts, SDL_Rect* _paddle0_r);
	void change_dir(short _angle);
private:
	short m_total_speed;
	short m_vector_angle;
	double m_dx, m_dy;
	double m_real_x, m_real_y;
};

#endif //BALL_H
