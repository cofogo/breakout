#ifndef BALL_H
#define BALL_H

#include <cmath>
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils.h"
using utils::vec2;
using utils::coll_data;
#include "Object.h"
#include "Brick.h"

class Ball: public Object
{
public:
	Ball(SDL_Rect _rect, short _spd, const vec2& _dir);
	~Ball();
	
	void update(short _x_max, short _y_max,
	     SDL_Rect* _paddle0_r,
	     int& _score0);
	void set_dir(const vec2& _dir);
	void coll_react(coll_data* _coll);
	void set_xy(double _x, double _y);
	short get_speed();
	vec2 get_dir();

private:
	short m_speed;
	short m_vector_angle;
	vec2 m_dir;
	vec2 m_pos;
	int m_score;
	double m_combo;
};

#endif //BALL_H
