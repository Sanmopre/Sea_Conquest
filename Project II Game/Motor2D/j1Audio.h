#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "j1MainMenuUI.h"
#include "j1GUIElements.h"
#include <list>
#include <iterator>
#define DEFAULT_MUSIC_FADE_TIME 2.0f
#define RAD_TO_DEG 57.32f				// The result of 180 / 3.14 for pass radiants to degrees
#define MAX_DISTANCE 255				// The maximum distance where you can listen
struct _Mix_Music;
struct Mix_Chunk;
typedef unsigned int uint;
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
	bool PlaySpatialFx(uint id, uint channel_angle = 1, uint distance = 1, int repeat = 0);
	uint GetAngle(iPoint player_pos, iPoint enemy_pos);
	uint GetDistance(iPoint player_pos, iPoint enemy_pos);

	uint boat_attack;
	uint boat_destroy;
	uint boat_spawn;
	uint harvester_destroy;
	uint harvester_spawn;
	uint harvester_work;
	uint structure_build;
	uint structure_destroy;
	uint ui_open;
	uint ui_purchase;
	uint ui_wood_hit;
	uint logo_audio;
	uint welcome_voice;
	uint start;

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