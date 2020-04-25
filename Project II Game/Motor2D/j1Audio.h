#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "j1MainMenuUI.h"
#include "j1GUIElements.h"
#include <list>
#include <iterator>
#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	//bool PlayMusic(const char* path, float fade_time = 2.0f, Mix_Music* loadedmusic = nullptr);
	//Quick bug fix (THIS AUDIO SYSTEM IS GETTING REWORKED ANYWAYS)
	bool PlayFxIntro(unsigned int fx, int repeat = 0, int volume = -1);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0, int volume = -1);
	void StopFx(int channel = -1);

	unsigned int boat_attack;
	unsigned int boat_destroy;
	unsigned int boat_spawn;
	unsigned int harvester_destroy;
	unsigned int harvester_spawn;
	unsigned int harvester_work;
	unsigned int structure_build;
	unsigned int structure_destroy;
	unsigned int ui_open;
	unsigned int ui_purchase;
	unsigned int ui_wood_hit;
	unsigned int logo_audio;

	//_Mix_Music* mainmenu_music;
	//_Mix_Music* ingame_chill_music;
	unsigned int mainmenu_music;
	unsigned int ingame_chill_music;

private:		 

	_Mix_Music*			music = NULL;
	//Mix_Music* music;
	std::list<Mix_Chunk*>	fx;
};

#endif // __j1AUDIO_H__