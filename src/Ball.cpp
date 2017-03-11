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
//TODO unify collision handling to all box objects and fix the madness
void Ball::update(short _x_max, short _y_max,
     SDL_Rect* _obst, SDL_Rect* _paddle0_r,
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

	//react to collision
	if(_obst != NULL) {
		//collision - have to find direction change
		cerr << "Collision!\n";

		//TODO the below clarity assignments are probs redundant now
		int obst_left = _obst->x;
		int obst_right = _obst->x
					   + _obst->w;
		int obst_top = _obst->y;
		int obst_bot = _obst->y
					 + _obst->h;
		int obst_w = _obst->w;
		int obst_h = _obst->h;
		int obst_cen_x = _obst->x
					   + (_obst->w / 2);
		cerr << "ocx: " << obst_cen_x << endl;
		int obst_cen_y = _obst->y
					   + (_obst->h / 2);

		//updating combo and score values
		m_combo += 0.1d;
		m_score += 100;

		double prev_cen_x = (double)cen_x - (m_dx * m_total_speed);
		double prev_cen_y = (double)cen_y - (m_dy * m_total_speed);
		//calculate distance between walls on x and y axes
		double dist_x = fabs(prev_cen_x - obst_cen_x)
					  - (obst_w/2) - (m_rect.w/2);
		double dist_y = fabs(prev_cen_y - obst_cen_y)
					  - (obst_h/2) - (m_rect.h/2);

		//for debug
		cerr << "prev cen x/y: " << prev_cen_x << "/" << prev_cen_y << endl;
		cerr << "curr o_c x/y: " << obst_cen_x << "/" << obst_cen_y << endl;
		cerr << "curr dst x/y: " << dist_x << "/" << dist_y << endl;

		//if dist is negative, collision must have not happened on that axis
		if(dist_x > 0 && dist_y > 0) {
			/* if relatively closer to x, collision happened on x
			 * and vice versa. If both are equal, both directions flip. */
			double relative_dist_x = dist_x / fabs(m_dx * m_total_speed);
			double relative_dist_y = dist_y / fabs(m_dy * m_total_speed);

			if(relative_dist_x <= relative_dist_y) {
				//vertical collision
				m_dx *= -1;
				cerr << "X flip ";
			}
			if(relative_dist_y <= relative_dist_x) {
				//horizontal collision
				m_dy *= -1;
				cerr << "Y flip ";
			}
			cerr << "rel_d_x/rel_d_y: " << relative_dist_x
				 << "/" << relative_dist_y << endl;
			cerr << "qdx/qdy: " << m_dx << "/" << m_dy << endl;

			//moving the ball to colission point
			double trans_rem = m_total_speed;
			if(dist_x > 0) {
				trans_rem -= dist_x;
				if(m_dy > 0) {
					m_rect.x = m_real_x += dist_x;
				}
				else {
					m_rect.x = m_real_x -= dist_x;
				}
			}
			if(dist_y > 0) {
				trans_rem -= dist_y;
				if(m_dy > 0) {
					m_rect.y = m_real_y += dist_y;
				}
				else {
					m_rect.y = m_real_y -= dist_y;
				}
			}
			
			//moving the ball with energy remaining after impact
			m_rect.x = m_real_x += m_dx * trans_rem;
			m_rect.y = m_real_y += m_dy * trans_rem;

			return;
		}
		else {
			if(dist_x < 0) {m_dy *= -1;}
			if(dist_y < 0) {m_dx *= -1;}
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
			double q_limit = 0.8d;
			if(new_dx > q_limit) {new_dx = q_limit;}
			else if(new_dx < -q_limit) {new_dx = -q_limit;}

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

void Ball::set_xy(double _x, double _y) {m_real_x = _x; m_real_y = _y;}
