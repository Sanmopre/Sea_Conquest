#include "ParticleSystem.h"
#include "j1ParticleManager.h"
#include "j1App.h"

#include "random.h"

ParticleSystem::ParticleSystem(PARTICLE_TYPES _type, p2Point<float> location, int index, float _timer)
{
	systemProps.Location = location;
	systemType = _type;
	timer = _timer;
	toDelete = false;

	if (_timer == 0)
		timeActive = false;
	else
		timeActive = true;

	if (systemType == PARTICLE_TYPES::CLOUD)
	{
		numberOfParticles = 3;
	}
	else if (systemType == PARTICLE_TYPES::SMOKE || systemType == PARTICLE_TYPES::FIRE)
	{
		numberOfParticles = 6;
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

	if (systemProps.type == PARTICLE_TYPES::CLOUD)
	{
		systemProps.Velocity = { -0.7, 0 };
		systemProps.lifetime = 40;
		systemProps.Acceleration = { 0, 0 };
		systemProps.rect = { 0, 0, 200, 200 };
		systemProps.lifetimeSubstraction = 0;
	}
	else if (systemProps.type == PARTICLE_TYPES::SMOKE)
	{
		systemProps.lifetime = 255;
		systemProps.Acceleration = { 0, (-0.01f) };
		systemProps.rect = { 0, 0, 5, 5 };
		systemProps.lifetimeSubstraction = 1.4;
	}
	else if (systemProps.type == PARTICLE_TYPES::FIRE)
	{
		systemProps.lifetime = 255;
		systemProps.Acceleration = { 0, (-0.01f) };
		systemProps.rect = { 0, 0, 5, 5 };
		systemProps.lifetimeSubstraction = 2;
	}
	else
	{
		systemProps.lifetime = 255;
		systemProps.Acceleration = { 0, 0.01 };
		systemProps.rect = { 0, 0, 7, 7 };
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

void ParticleSystem::Update(float dt)
{
	if (timeActive == true)
	{
		timer -= dt;

		if (timer <= 0)
		{
			toDelete = true;
		}
	}
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

void ParticleSystem::changePosition(fPoint location)
{
	Particle* particle = nullptr;
	for (int i = 0; i < numberOfParticles; i++)
	{
		particle = *(referencesArray + i);
		particle->Props.Location = location;
	}
}