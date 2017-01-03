#include "Timer.h"

void Timer::set_start(Uint32 _t)
{
	m_start = _t;
}

void Timer::set_end(Uint32 _t)
{
	m_end = _t;
}

Uint32 Timer::get_duration()
{
	return m_end - m_start;
}