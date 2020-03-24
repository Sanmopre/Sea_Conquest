#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include<vector>
#include "j1Module.h"
#include "Particle.h"
#include "ParticleSystem.h"

enum class PARTICLE_TYPES;

class j1ParticleManager : public j1Module 
{
public:

	j1ParticleManager();
	~j1ParticleManager();
	
	bool		Start();
	bool		Update(float dt);
	bool		CleanUp();

	void		updateIndex();
	void		changeIndex(int newIndex);
	int			getIndex();

	ParticleSystem* createSystem(PARTICLE_TYPES type, fPoint location);
	void		deleteSystem(ParticleSystem* system_);
	void		deleteAllSystems();

	void		deleteAllParticles();

	std::vector<Particle> particlePool;
	std::vector<ParticleSystem*> systems;

private:
	int	 Index;
};

#endif // __PARTICLESYSTEM_H__