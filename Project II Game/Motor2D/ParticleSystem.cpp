#include "ParticleSystem.h"
#include "j1ParticleManager.h"
#include "j1App.h"

#include "random.h"

ParticleSystem::ParticleSystem(PARTICLE_TYPES _type, p2Point<float> location, int index, float _timer)
{
	systemProps.Location = location;
	systemType = _type;
	timer = _timer;

	if (systemType == PARTICLE_TYPES::CLOUD)
	{
		numberOfParticles = 3;
	}
	else
	{
		numberOfParticles = 20;
	}

	loadSystem();
	activateSystem(index);
}

ParticleSystem::~ParticleSystem()
{
	deactivateAllParticles();
}


void ParticleSystem::loadSystem()
{
	systemProps.type = systemType;
	systemProps.timer = timer;

	if (systemProps.type == PARTICLE_TYPES::CLOUD)
	{
		systemProps.Velocity = { -1, 0 };
		systemProps.lifetime = 40;
		systemProps.Acceleration = { 0, 0 };
		systemProps.rect = { 0, 0, 100, 100 };
		systemProps.lifetimeSubstraction = 0;
	}
	else
	{
		systemProps.lifetime = 255;
		systemProps.Acceleration = { 0, 0.01 };
		systemProps.rect = { 0, 0, 10, 10 };
		systemProps.lifetimeSubstraction = 3;
	}
}

bool ParticleSystem::activateSystem(int index)
{
	Particle* pReference = nullptr;
	int counter = 0;
	int newIndex = index;

	while (counter < numberOfParticles)
	{
		if (App->pmanager->particlePool[newIndex].active == true)
		{
			App->pmanager->updateIndex(); //make it return false if we ran out of particles
			newIndex = App->pmanager->getIndex();
		}

		pReference = &(App->pmanager->particlePool[newIndex]);
		pReference->loadProperties(systemProps);
		pReference->switchParticleState();

		*(referencesArray + counter) = pReference;

		newIndex++;
		counter++;
	}

	App->pmanager->changeIndex(index + counter);

	return true;
}

void ParticleSystem::deactivateParticle(Particle* particle)
{
	particle->switchParticleState();
}

void ParticleSystem::deactivateAllParticles()
{
	Particle* particle = nullptr;
	for (int i = 0; i < numberOfParticles; i++)
	{
		particle = *(referencesArray + i);
		particle->switchParticleState();
	}
}

void ParticleSystem::changePosition(iPoint location)
{
	Particle* particle = nullptr;
	for (int i = 0; i < numberOfParticles; i++)
	{
		particle = *(referencesArray + i);
		particle->Props.Location.x = (float)location.x;
		particle->Props.Location.y = (float)location.y;
	}
}