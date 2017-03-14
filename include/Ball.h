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
	     SDL_Rect* _obst, SDL_Rect* _paddle0_r,
	     int& _score0);
	void change_dir(short _angle);
	void coll_react(int _cen_x, int _cen_y, SDL_Rect* _obst);
	void set_xy(double _x, double _y);

private:
	short m_total_speed;
	short m_vector_angle;
	double m_dx, m_dy;
	double m_real_x, m_real_y;
	int m_score;
	double m_combo;
};

#endif //BALL_H
