#include "j1App.h"
#include <vector>
#include "Particle.h"
#include "j1Render.h"
#include "random.h"

using namespace std;

Particle::Particle()
{
	active			= false;
}

void Particle::Update()
{
	if (active)
	{
		pVelocity	+= pAcceleration;

		pLocation	+= pVelocity;

		pRect.x		= pLocation.x;
		pRect.y		= pLocation.y;

		Draw();

		remainingLifetime -= 2;

		if (remainingLifetime < 0)
		{
			loadProperties(Props);
			remainingLifetime = lifespan;
		}
	}
}

void Particle::loadProperties(ParticleProps properties)
{	
	pLocation			= properties.Location;
	pVelocity			= {((float)(Random::Randomize() - 0.5)), ((float)(Random::Randomize()) * (-1))};
	pAcceleration		= properties.Acceleration;
	lifespan			= properties.lifetime;
	pRect				= properties.rect; 
	remainingLifetime	= lifespan;
	Props				= properties;
}

void Particle::switchParticleState()
{
	if (active)
	{
		active = false;
	}
	else
	{
		active = true;
	}
}

bool Particle::Draw()
{
	App->render->AddBlitEvent(3, nullptr, pLocation.x, pLocation.y, pRect, false, 0.0f, 255, 0, 0, remainingLifetime);
	
	return true;
} 