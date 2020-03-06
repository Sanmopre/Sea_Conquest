#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

struct timed_var
{
	float iterations;
	float counter = 0;
};

class j1Timer
{
public:

	j1Timer();

	void Start();
	void Stop();
	void Resume();

	float ReadSec() const;
	uint Read() const;

private:

	bool	running;
	uint	started_at;
	uint	stopped_at;
};

#endif //__j1TIMER_H__