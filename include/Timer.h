#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

class Timer
{
public:
	Timer() {};
	~Timer() {};
	
	void set_start(Uint32 _t);
	void set_end(Uint32 _t);
	Uint32 get_duration();
	
private:
	Uint32 m_start, m_end;
};

#endif