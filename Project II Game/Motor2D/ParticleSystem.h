#pragma once
#include "p2Point.h"
#include "Particle.h"

enum class PARTICLE_TYPES
{ 
	NONE,
	TEST,
	SMOKE,
	FIRE,
	EXPLOSION,
	WIND,
	CLOUD,
	WAVE
};
class ParticleSystem
{
public:
	ParticleSystem(PARTICLE_TYPES type, p2Point<float>, int index);
	~ParticleSystem();

	//void update(float dt);

	void loadSystem(); //maybe we should load each effect properties from an xml
	bool activateSystem(int index);
	void deactivateParticle(Particle* particle);
	void deactivateAllParticles();
	void changePosition(iPoint location);

	ParticleProps systemProps;

private:
	PARTICLE_TYPES systemType;
	Particle* referencesArray[20];
	const int numberOfParticles = 20;
};