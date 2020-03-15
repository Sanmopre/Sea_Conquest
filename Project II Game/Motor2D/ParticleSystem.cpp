#include "ParticleSystem.h"
#include "j1ParticleManager.h"
#include "j1App.h"

#include "random.h"

ParticleSystem::ParticleSystem(PARTICLE_TYPES _type, p2Point<float> location, int index)
{
	systemProps.Location = location;
	systemType = _type;
	loadSystem();
	activateSystem(index);
}

ParticleSystem::~ParticleSystem()
{
	deactivateAllParticles();
}

void ParticleSystem::loadSystem() //this must depend on system type; right now, though, they are just hardcoded values. 
{
	systemProps.lifetime = 255;
	systemProps.Acceleration = { 0, 0.01 };
	systemProps.rect = { 0, 0, 10, 10 };
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
			App->pmanager->updateIndex();
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