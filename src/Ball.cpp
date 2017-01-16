#include "Ball.h"

Ball::Ball(SDL_Rect _rect, short _spd, short _angle)
: Object(_rect)
, m_total_speed(_spd)
, m_vector_angle(_angle)
, m_dx(0)
, m_dy(0)
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
		m_dx = (double)(m_total_speed) * x_speed_margin;
		m_dy = (double)(m_total_speed) * y_speed_margin;
	}
	else if(_angle >= 90 && _angle < 180) {
		short local_angle = _angle - 90;
		double x_speed_margin = (double)(90 - local_angle) / 90;
		double y_speed_margin = (double)(local_angle) / 90;
		m_dx = (double)(m_total_speed) * x_speed_margin;
		m_dy = (double)(m_total_speed) * y_speed_margin;
	}
	else if(_angle >= 180 && _angle < 270) {
		short local_angle = _angle - 180;
		double x_speed_margin = -(double)(local_angle) / 90;
		double y_speed_margin = (double)(90 - local_angle) / 90;
		m_dx = (double)(m_total_speed) * x_speed_margin;
		m_dy = (double)(m_total_speed) * y_speed_margin;
	}
	else if(_angle >= 270 && _angle < 360) {
		short local_angle = _angle - 270;
		double x_speed_margin = -(double)(90 - local_angle) / 90;
		double y_speed_margin = -(double)(local_angle) / 90;
		m_dx = (double)(m_total_speed) * x_speed_margin;
		m_dy = (double)(m_total_speed) * y_speed_margin;
	}
	
	cerr << "ball total speed: " << m_total_speed << endl;
	cerr << "ball_x_spd: " << m_dx << endl;
	cerr << "ball_y_spd: " << m_dy << endl;
	cerr << "ball_x: " << m_real_x << endl;
	cerr << "ball_y: " << m_real_y << endl;
}

void Ball::move(short _x_max, short _y_max, vector<shared_ptr<Brick>>* _obsts)
{
	int left = m_rect.x + m_dx;
	int right = m_rect.x + m_rect.w + m_dx;
	int top = m_rect.y + m_dy;
	int bot = m_rect.y + m_rect.h  + m_dy;
	int cen_x = m_rect.x + (m_rect.w / 2);
	int cen_y = m_rect.y + (m_rect.h / 2);
	
	// checking bounds
	if(right + m_dx > _x_max || left + m_dx < 0) {
		m_dx = -m_dx;
	}
	if(bot + m_dy > _y_max || top + m_dy < 0) {
		m_dy = -m_dy;
	}
	
	//check for and handle collisions
	for(unsigned i = 0; i < _obsts->size(); ++i) {
		int obst_left = (*_obsts)[i]->get_rect()->x;
		int obst_right = (*_obsts)[i]->get_rect()->x
		              + (*_obsts)[i]->get_rect()->w;
		int obst_top = (*_obsts)[i]->get_rect()->y;
		int obst_bot = (*_obsts)[i]->get_rect()->y
		             + (*_obsts)[i]->get_rect()->h;
		/*int obst_cen_x = (*_obsts)[i]->get_rect()->x
					   + ((*_obsts)[i]->get_rect()->w / 2);
		int obst_cen_y = (*_obsts)[i]->get_rect()->y
					   + ((*_obsts)[i]->get_rect()->h / 2);*/
		if(left > obst_right
		|| right < obst_left
		|| top > obst_bot
		|| bot < obst_top
		) { // no collision
		}
		else {
			//collision - have to find direction change
			cerr << "Collision!\n";
			//destroy brick
			(*_obsts)[i] = _obsts->back();
			(*_obsts).pop_back();
			
			if(cen_y > obst_bot || cen_y < obst_top) {
				m_dy *= -1;
			}
			else {
				m_dx *= -1;
			}
			//FIXME - solution looks fun, but does not work as intended xD
			/*if(abs(top - obst_top) < abs(left - obst_left)) {
				m_dy = -m_dy;
			}
			else if(abs(top - obst_top) > abs(left - obst_left)) {
				m_dx = -m_dx;
			}
			else {
				m_dx = -m_dx;
				m_dy = -m_dy;
			}
			break;*/
		}
	}

	//moving the ball
	//NOTE a bit inefficient
	m_rect.x = m_real_x += m_dx;
	m_rect.y = m_real_y += m_dy;
}
