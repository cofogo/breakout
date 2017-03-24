#include "Ball.h"

Ball::Ball(SDL_Rect _rect, short _spd, const vec2& _dir)
: Object(_rect)
, m_speed(_spd)
, m_dir(_dir)
, m_pos(vec2{double(_rect.x), double(_rect.y)})
, m_score(0)
, m_combo(0.0d)
{
}

Ball::~Ball() {}

void Ball::set_dir(const vec2& _dir) {m_dir = _dir;}

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

	//TODO below should probs be implemented differently
	//in the future this flag will change depending on which player the ball
	//belongs to
	bool bott_death = true;	
	
	// checking bounds
	if(right + m_dir.x > _x_max || left + m_dir.x < 0) {
		m_dir.x = -m_dir.x;
	}
	if(bott_death) {
		if(top + m_dir.y < 0) {
			m_dir.y = -m_dir.y;
		}
	}
	else {
		if(bot + m_dir.y > _y_max) {
			m_dir.y = -m_dir.y;
		}
	}

	//react to collision
	if(_obst != NULL) {
		this->coll_react(cen_x, cen_y, _obst);
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
			m_dir.y *= -1;
			short pad0_cen_x = (_paddle0_r->x + (_paddle0_r->w / 2));
			double new_dx = (double)(cen_x - pad0_cen_x) / (_paddle0_r->w / 2);
			cerr << "new_dx" << fabs(new_dx) << endl;
			
			//limit change range
			double q_limit = 0.8d;
			if(new_dx > q_limit) {new_dx = q_limit;}
			else if(new_dx < -q_limit) {new_dx = -q_limit;}

			m_dir.x = new_dx;
			m_dir.y = (m_dir.y > 0)? (1.0d - fabs(new_dx)) : (-1.0d + fabs(new_dx));
			cerr << "dx: " << m_dir.x << endl;
			cerr << "dy: " << m_dir.y << endl;
		}
		else {
			m_dir.x *= -1;
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
	m_rect.x = m_pos.x += m_dir.x * m_speed;
	m_rect.y = m_pos.y += m_dir.y * m_speed;
}

void Ball::coll_react(int _cen_x, int _cen_y, SDL_Rect* _obst)
{
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

	double prev_cen_x = (double)_cen_x - (m_dir.x * m_speed);
	double prev_cen_y = (double)_cen_y - (m_dir.y * m_speed);
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
		double relative_dist_x = dist_x / fabs(m_dir.x * m_speed);
		double relative_dist_y = dist_y / fabs(m_dir.y * m_speed);

		if(relative_dist_x <= relative_dist_y) {
			//vertical collision
			m_dir.x *= -1;
			cerr << "X flip ";
		}
		if(relative_dist_y <= relative_dist_x) {
			//horizontal collision
			m_dir.y *= -1;
			cerr << "Y flip ";
		}
		cerr << "rel_d_x/rel_d_y: " << relative_dist_x
			 << "/" << relative_dist_y << endl;
		cerr << "qdx/qdy: " << m_dir.x << "/" << m_dir.y << endl;

		//moving the ball to colission point
		double trans_rem = m_speed;
		if(dist_x > 0) {
			trans_rem -= dist_x;
			if(m_dir.y > 0) {
				m_rect.x = m_pos.x += dist_x;
			}
			else {
				m_rect.x = m_pos.x -= dist_x;
			}
		}
		if(dist_y > 0) {
			trans_rem -= dist_y;
			if(m_dir.y > 0) {
				m_rect.y = m_pos.y += dist_y;
			}
			else {
				m_rect.y = m_pos.y -= dist_y;
			}
		}
		
		//moving the ball with energy remaining after impact
		m_rect.x = m_pos.x += m_dir.x * trans_rem;
		m_rect.y = m_pos.y += m_dir.y * trans_rem;

		return;
	}
	else {
		if(dist_x < 0) {m_dir.y *= -1;}
		if(dist_y < 0) {m_dir.x *= -1;}
	}
}

void Ball::coll_react_new(char _dir, vec2 _coll_point)
{
	//changing direction due to rebound
	if(_dir == 'v') {m_dir.x *= -1;}
	else if(_dir == 'h') {m_dir.y *= -1;}
	else if(_dir == 'c') {
		m_dir.x *= -1;
		m_dir.y *= -1;
	}

	//moving the ball to colission point
	vec2 dist = vec2{fabs(m_pos.x - _coll_point.x), fabs(m_pos.y - _coll_point.y)};
	double trans_rem = m_speed;
	if(dist.x > 0) {
		trans_rem -= dist.x;
		if(m_dir.y > 0) {
			m_rect.x = m_pos.x += dist.x;
		}
		else {
			m_rect.x = m_pos.x -= dist.x;
		}
	}
	if(dist.y > 0) {
		trans_rem -= dist.y;
		if(m_dir.y > 0) {
			m_rect.y = m_pos.y += dist.y;
		}
		else {
			m_rect.y = m_pos.y -= dist.y;
		}
	}
	
	//continue ball movement with energy remaining after impact
	m_rect.x = m_pos.x += m_dir.x * trans_rem;
	m_rect.y = m_pos.y += m_dir.y * trans_rem;

	return;
}

void Ball::set_xy(double _x, double _y) {m_pos.x = _x; m_pos.y = _y;}

short Ball::get_speed() {return m_speed;}
vec2 Ball::get_dir() {return m_dir;}
