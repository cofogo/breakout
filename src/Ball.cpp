#include "Ball.h"

Ball::Ball(SDL_Rect _rect, short _spd, short _angle)
: Object(_rect)
, m_total_speed(_spd)
, m_vector_angle(_angle)
, m_dx(0)
, m_dy(0)
, m_real_x(_rect.x)
, m_real_y(_rect.y)
, m_score(0)
, m_combo(0.0d)
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
		m_dx = (double)(local_angle) / 90;
		m_dy = -(double)(90 - local_angle) / 90;
	}
	else if(_angle >= 90 && _angle < 180) {
		short local_angle = _angle - 90;
		m_dx = (double)(90 - local_angle) / 90;
		m_dy = (double)(local_angle) / 90;
	}
	else if(_angle >= 180 && _angle < 270) {
		short local_angle = _angle - 180;
		m_dx = -(double)(local_angle) / 90;
		m_dy = (double)(90 - local_angle) / 90;
	}
	else if(_angle >= 270 && _angle < 360) {
		short local_angle = _angle - 270;
		m_dx = -(double)(90 - local_angle) / 90;
		m_dy = -(double)(local_angle) / 90;
	}
	
	cerr << "ball total speed: " << m_total_speed << endl;
	cerr << "ball_x_spd: " << m_dx << endl;
	cerr << "ball_y_spd: " << m_dy << endl;
	cerr << "ball_x: " << m_real_x << endl;
	cerr << "ball_y: " << m_real_y << endl;
}

//TODO move collision, score, etc logic separatly. 'Cause spaghetti
void Ball::update(short _x_max, short _y_max,
     vector<shared_ptr<Brick>>* _obsts, SDL_Rect* _paddle0_r,
	 int& _score0)
{
	int left = m_rect.x;
	int right = m_rect.x + m_rect.w;
	int top = m_rect.y;
	int bot = m_rect.y + m_rect.h;
	int cen_x = m_rect.x + (m_rect.w / 2);
	int cen_y = m_rect.y + (m_rect.h / 2);

	//in the future this flag will change depending on which player the ball
	//belongs to
	bool bott_death = true;	
	
	// checking bounds
	if(right + m_dx > _x_max || left + m_dx < 0) {
		m_dx = -m_dx;
	}
	if(bott_death) {
		if(top + m_dy < 0) {
			m_dy = -m_dy;
		}
	}
	else {
		if(bot + m_dy > _y_max) {
			m_dy = -m_dy;
		}
	}

	//check for and handle collisions
	for(unsigned i = 0; i < _obsts->size(); ++i) {
		int obst_left = (*_obsts)[i]->get_rect()->x;
		int obst_right = (*_obsts)[i]->get_rect()->x
		              + (*_obsts)[i]->get_rect()->w;
		int obst_top = (*_obsts)[i]->get_rect()->y;
		int obst_bot = (*_obsts)[i]->get_rect()->y
		             + (*_obsts)[i]->get_rect()->h;

		if(left > obst_right
		|| right < obst_left
		|| top > obst_bot
		|| bot < obst_top
		) { // no collision
		}
		else {
			//collision - have to find direction change
			cerr << "Collision!\n";
			//TODO move brick destruction to where it belongs
			//destroy brick
			(*_obsts)[i] = _obsts->back();
			(*_obsts).pop_back();
			
			//TODO - this logic is simplified, sometimes misbehaves
			//polish laer
			if(cen_y > obst_bot || cen_y < obst_top) {
				m_dy *= -1;
			}
			else {
				m_dx *= -1;
			}

			//updating combo and score value
			m_combo += 0.1d;
			m_score += 100;

			break;
		}
	}

	//handle collision with the paddle
	if(left > _paddle0_r->x + _paddle0_r->w
	|| right < _paddle0_r->x
	|| top > _paddle0_r->y + _paddle0_r->h
	|| bot < _paddle0_r->y
	) { // no collision
	}
	else {
		//collision - have to find direction change
		cerr << "Paddle collision!\n";
		if(cen_y > _paddle0_r->y + _paddle0_r->h || cen_y < _paddle0_r->y) {
			m_dy *= -1;
			short pad0_cen_x = (_paddle0_r->x + (_paddle0_r->w / 2));
			double new_dx = (double)(cen_x - pad0_cen_x) / (_paddle0_r->w / 2);
			cerr << "new_dx" << fabs(new_dx) << endl;
			
			//limit change range
			if(new_dx > 0.9d) {new_dx = 0.9d;}
			else if(new_dx < -0.9d) {new_dx = -0.9d;}

			m_dx = new_dx;
			m_dy = (m_dy > 0)? (1.0d - fabs(new_dx)) : (-1.0d + fabs(new_dx));
			cerr << "dx: " << m_dx << endl;
			cerr << "dy: " << m_dy << endl;
		}
		else {
			m_dx *= -1;
		}

		//give score
		_score0 += m_score * (0.9d + m_combo);
		m_combo = 0.0d;
		m_score = 0;
		//not reseting the m_score would result in an interesting mechanic
		//the longer the ball lives, the more valuable it becomes
	}

	//moving the ball
	//NOTE a bit inefficient
	m_rect.x = m_real_x += m_dx * m_total_speed;
	m_rect.y = m_real_y += m_dy * m_total_speed;
}
