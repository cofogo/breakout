#include "Ball.h"

Ball::Ball(SDL_Rect _rect, short _spd, short _angle)
: Object(_rect)
, m_total_speed(_spd)
, m_vector_angle(_angle)
, m_x_speed(0)
, m_y_speed(0)
{
	calc_speed(m_vector_angle);
}

Ball::~Ball() {}

/*FIXME implement calculations in floating point and/or find a way to not lose
 *move speed due to values of fractions of pixels*/
void Ball::calc_speed(short _angle)
{
	//handling input ouside of 0 - 360 range
	while(_angle >= 360) {
		_angle -= 360;
	}
	
	if(_angle < 0) {
		_angle += 180;
		while(_angle < 0) {
			_angle += 360;
		}
	}
	
	//calculating speed depending on which 90deg quarter the angle degree falls into
	if(_angle >= 0 && _angle < 90) {
		short local_angle = _angle;
		short x_speed_percent = (local_angle * 100) / 90;
		short y_speed_percent = -((90 - local_angle) * 100) / 90;
		
		m_x_speed = (m_total_speed * x_speed_percent / 100);
		m_y_speed = (m_total_speed * y_speed_percent / 100);
	}
	else if(_angle >= 90 && _angle < 180) {
		short local_angle = _angle - 90;
		short y_speed_percent = (local_angle * 100) / 90;
		short x_speed_percent = ((90 - local_angle) * 100) / 90;
		
		m_x_speed = (m_total_speed * x_speed_percent / 100);
		m_y_speed = (m_total_speed * y_speed_percent / 100);
	}
	else if(_angle >= 180 && _angle < 270) {
		short local_angle = _angle - 180;
		short y_speed_percent = ((90 - local_angle) * 100) / 90;
		short x_speed_percent = -(local_angle * 100) / 90;
		
		m_x_speed = (m_total_speed * x_speed_percent / 100);
		m_y_speed = (m_total_speed * y_speed_percent / 100);
	}
	else if(_angle >= 270 && _angle < 360) {
		short local_angle = _angle - 270;
		short x_speed_percent = -((90 - local_angle) * 100) / 90;
		short y_speed_percent = -(local_angle * 100) / 90;
		
		m_x_speed = (m_total_speed * x_speed_percent / 100);
		m_y_speed = (m_total_speed * y_speed_percent / 100);
	}
	
	cerr << "ball total speed: " << m_total_speed << endl;
	cerr << "ball_x_spd: " << m_x_speed << endl;
	cerr << "ball_y_spd: " << m_y_speed << endl;
}

void Ball::move(short _x_max, short _y_max)
{
	if(m_rect.x + m_rect.w + m_x_speed > _x_max
	|| m_rect.x + m_x_speed < 0) {
		m_x_speed = -m_x_speed;
	}
	if(m_rect.y + m_rect.h + m_y_speed > _y_max || m_rect.y + m_y_speed < 0) {
		m_y_speed = -m_y_speed;
	}
	
	m_rect.x += m_x_speed;
	m_rect.y += m_y_speed;
}