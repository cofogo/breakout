#include "Ball.h"

Ball::Ball(SDL_Rect _rect, short _spd, short _angle)
: Object(_rect)
, m_total_speed(_spd)
, m_vector_angle(_angle)
, m_x_speed(0)
, m_y_speed(0)
, m_real_x(_rect.x)
, m_real_y(_rect.y)
{
	change_dir(m_vector_angle);
}

Ball::~Ball() {}

void Ball::change_dir(short _angle)
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
	
	/* TODO current direcrional speed calculation algorithm seems very clumsy
	 * maybe could be done by calculating sin() and cos()? */
	//calculating speed depending on which 90deg quarter the angle degree falls into
	if(_angle >= 0 && _angle < 90) {
		short local_angle = _angle;
		double x_speed_margin = (double)(local_angle) / 90;
		double y_speed_margin = -(double)(90 - local_angle) / 90;
		m_x_speed = (double)(m_total_speed) * x_speed_margin;
		m_y_speed = (double)(m_total_speed) * y_speed_margin;
	}
	else if(_angle >= 90 && _angle < 180) {
		short local_angle = _angle - 90;
		double x_speed_margin = (double)(90 - local_angle) / 90;
		double y_speed_margin = (double)(local_angle) / 90;
		m_x_speed = (double)(m_total_speed) * x_speed_margin;
		m_y_speed = (double)(m_total_speed) * y_speed_margin;
	}
	else if(_angle >= 180 && _angle < 270) {
		short local_angle = _angle - 180;
		double x_speed_margin = -(double)(local_angle) / 90;
		double y_speed_margin = (double)(90 - local_angle) / 90;
		m_x_speed = (double)(m_total_speed) * x_speed_margin;
		m_y_speed = (double)(m_total_speed) * y_speed_margin;
	}
	else if(_angle >= 270 && _angle < 360) {
		short local_angle = _angle - 270;
		double x_speed_margin = -(double)(90 - local_angle) / 90;
		double y_speed_margin = -(double)(local_angle) / 90;
		m_x_speed = (double)(m_total_speed) * x_speed_margin;
		m_y_speed = (double)(m_total_speed) * y_speed_margin;
	}
	
	cerr << "ball total speed: " << m_total_speed << endl;
	cerr << "ball_x_spd: " << m_x_speed << endl;
	cerr << "ball_y_spd: " << m_y_speed << endl;
	cerr << "ball_x: " << m_real_x << endl;
	cerr << "ball_y: " << m_real_y << endl;
}

void Ball::move(short _x_max, short _y_max, vector<unique_ptr<Brick>>* _obsts)
{
	// checking bounds
	if(m_rect.x + m_rect.w + m_x_speed > _x_max
	|| m_rect.x + m_x_speed < 0) {
		m_x_speed = -m_x_speed;
	}
	if(m_rect.y + m_rect.h + m_y_speed > _y_max || m_rect.y + m_y_speed < 0) {
		m_y_speed = -m_y_speed;
	}
	
	//
	for(unsigned i = 0; i < _obsts->size(); ++i) {
		if(m_rect.x > (*_obsts)[i]->get_rect()->x + (*_obsts)[i]->get_rect()->w
		|| m_rect.x + m_rect.w < (*_obsts)[i]->get_rect()->x
		|| m_rect.y > (*_obsts)[i]->get_rect()->y + (*_obsts)[i]->get_rect()->h
		|| m_rect.y + m_rect.h < (*_obsts)[i]->get_rect()->y
		) { // no collision
		}
		else {
			//collision - have to find direction change
			cerr << "Collision!\n";
			break;
		}
	}
	
	//NOTE a bit inefficient
	m_rect.x = m_real_x += m_x_speed;
	m_rect.y = m_real_y += m_y_speed;
}