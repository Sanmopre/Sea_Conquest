#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#define MAX_PARTICLE_LIFESPAN 255

#include "j1App.h"
#include "SDL/include/SDL.h"
#include <vector>
#include "p2Point.h"

using namespace std;

struct ParticleProps
{
	p2Point<float>	Location		= {0,0};
	p2Point<float>	Velocity		= {0,0};
	p2Point<float>	Acceleration	= {0,0};
	SDL_Rect		rect			= {0,0,20,20};
	int				lifetime		= 255;
};

class Particle
{
public:

	Particle();
	
	void			Update();
	bool			Draw();
	void			switchParticleState();
	void			loadProperties(ParticleProps properties);

	SDL_Rect		pRect;
	p2Point<float>	pLocation;
	p2Point<float>	pVelocity;
	p2Point<float>	pAcceleration;

	int				lifespan;
	int				remainingLifetime;
	bool			active;
	ParticleProps	Props;	
};

#endif 