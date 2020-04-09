#include "j1ParticleManager.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"

j1ParticleManager::j1ParticleManager()
{
	particlePool.resize(1500);	// Allocate dynamicaly a lot of particles for later use
	Index = 0;
}

j1ParticleManager::~j1ParticleManager()
{
	deleteAllParticles();
	deleteAllSystems();
}

bool j1ParticleManager::Start()
{
	return true;
}

bool j1ParticleManager::Update(float dt)
{
	int counter = 0;

	while (counter != particlePool.size())
	{
		vector<Particle>::iterator particle = particlePool.begin();

		particle += counter;
		for (; particle != particlePool.end(); particle++)
		{
			if (particle->timeFinished == true)
				particle->active = false;

			if (particle->active)
				particle->Update(dt);

			counter++;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		iPoint pos = { 1, 1 };
		App->render->ScreenToWorld(pos.x, pos.y);

		fPoint fpos = { (float)pos.x,  (float)pos.y };
			App->pmanager->createSystem(PARTICLE_TYPES::EXPLOSION, fpos, 2);
	}
	return true;
}

bool j1ParticleManager::CleanUp()
{
	particlePool.clear();
	return true;
}

void j1ParticleManager::deleteAllParticles()
{
	while (particlePool.size() != 0)
	{
		particlePool.erase(particlePool.begin(), particlePool.begin() + 1);
		particlePool.shrink_to_fit();
	}
}
///////////////////ParticleSystems methods

ParticleSystem* j1ParticleManager::createSystem(PARTICLE_TYPES type, p2Point<float> location, float timer)
{
	ParticleSystem* newSystem = new ParticleSystem(type, location, Index, timer);
	systems.push_back(newSystem);
	return newSystem;
}

void j1ParticleManager::deleteSystem(ParticleSystem* system_)
{
	if (systems.size() != 0)
		for (auto system = systems.begin(); system != systems.end(); system++)
		{
			if ((*system) == system_)
			{
				delete (*system);
				systems.erase(system, system + 1);
				systems.shrink_to_fit();
				break;
			}
		}
}

void j1ParticleManager::deleteAllSystems()
{
	while (systems.size() != 0)
	{
		delete (*systems.begin());
		systems.erase(systems.begin(), systems.begin() + 1);
		systems.shrink_to_fit();
	}
}

////////////////////Index Methods
void j1ParticleManager::updateIndex()
{
	int counter = 0;

	for (int newIndex = Index; particlePool[newIndex].active != false; newIndex++)
	{
		counter++;

		if (newIndex == 1499) //in case we arrive to the particle number 1499(the last one), we go back to the beggining to check if there are any particles free.
			newIndex = 0;

		if (counter = 1500)
			LOG("The unexpected happened. We ran out of particles");
	}

	Index += counter;
}

int j1ParticleManager::getIndex()
{
	return Index;
}

void j1ParticleManager::changeIndex(int newIndex)
{
	Index = newIndex;
}



