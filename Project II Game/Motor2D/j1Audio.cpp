#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"
#include <math.h>
#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.create("audio");
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}
	Mix_AllocateChannels(360);

	boat_attack = App->audio->LoadFx("audio/fx/boat_attack.wav");
	boat_destroy = App->audio->LoadFx("audio/fx/boat_destroy.wav");
	boat_spawn = App->audio->LoadFx("audio/fx/boat_spawn.wav");
	harvester_destroy = App->audio->LoadFx("audio/fx/harvester_destroy.wav");
	harvester_spawn = App->audio->LoadFx("audio/fx/harvester_spawn.wav");
	harvester_work = App->audio->LoadFx("audio/fx/harvester_work.wav");
	structure_build = App->audio->LoadFx("audio/fx/structure_build.wav");
	structure_destroy = App->audio->LoadFx("audio/fx/structure_destroy.wav");
	ui_open = App->audio->LoadFx("audio/fx/ui_open.wav");
	ui_purchase = App->audio->LoadFx("audio/fx/ui_purchase.wav");
	ui_wood_hit = App->audio->LoadFx("audio/fx/uiclick.wav");
	logo_audio = App->audio->LoadFx("audio/fx/logo_intro.wav");
	welcome_voice = App->audio->LoadFx("audio/fx/welcome.wav");
	start = App->audio->LoadFx("audio/fx/start_game.wav");
	you_are_not_prepared = App->audio->LoadFx("audio/fx/you_are_not_prepared.wav");
	this_will_be_fun = App->audio->LoadFx("audio/fx/this_will_be_fun.wav");	
	such_arrogance = LoadFx("audio/fx/such_arrogance.wav");
	ha = LoadFx("audio/fx/ha.wav");
	no = LoadFx("audio/fx/no.wav");
	//mainmenu_music = Mix_LoadMUS("audio/music/Motorista_Reciclista_Shop1_start.wav");
	//ingame_chill_music = Mix_LoadMUS("audio/music/Sea_conquest_chill_cutre.wav");
	mainmenu_music = LoadMusic("audio/music/tavern_song.wav");
	ingame_chill_music = LoadMusic("audio/music/song.wav");

	
	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	std::list<Mix_Chunk*>::iterator it;
	for(it = fx.begin(); it != fx.end(); it++)
		Mix_FreeChunk(*it);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
/*
bool j1Audio::PlayMusic(const char* path, float fade_time, Mix_Music* loadedmusic)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	if (loadedmusic == nullptr)
		music = Mix_LoadMUS(path);
	//_Mix_Music* loadedmus = loadedmusic;
	music = loadedmusic;
	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}
*/

uint j1Audio::LoadMusic(const char* path) // Loads the audio on the Mix_Music* 
{
	unsigned int ret = 0;

	if (!active)
		return 0;

	//TODO 5 load the audio path given into a Mix_Music variable
	Mix_Music* music_chunk = Mix_LoadMUS(path);

	if (music_chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		//TODO 5.1 Add the previous audio into the list
		musics.push_back(music_chunk);
		ret = musics.size();
	}

	return ret;
}

bool j1Audio::PlayMusic(unsigned int id, int volume, float fade_time)
{
	bool ret = true;
	int vol = 0;
	if (App->mainmenu != nullptr)
		if (App->mainmenu->GetMenu().music != nullptr)
			vol = App->mainmenu->GetMenu().music->Value;
	if (volume > 0)
		vol = volume;

	if (!active)
		return false;

	if (id > 0 && id <= musics.size())
	{
		//TODO 6 Iterate all the music audios stored in the list
		std::list <Mix_Music*>::const_iterator it;
		it = std::next(musics.begin(), id - 1);
		Mix_VolumeMusic(vol);
		//TODO 7 Given the fade_time implement a fade in and fade out using Mix_Fade(Out/In)Music
		if (*it != NULL)
		{
			if (fade_time > 0.0f)
			{
				Mix_FadeOutMusic(int(fade_time * 1000.0f));
			}
			else
			{
				Mix_HaltMusic();
			}
		}
		if (fade_time > 0.0f)
		{
			Mix_FadeInMusic(*it, -1, (int)(fade_time * 1000.0f));
			//Mix_PlayMusic(*it, -1);
		}
		else
		{
			Mix_PlayMusic(*it, -1);
		}

	}

	return ret;
}

void j1Audio::PauseMusic(float fade_time)
{
	if (active)
	{
		if (Mix_PlayingMusic() == 1)	// Sees if there is music playing
		{
			if (Mix_PausedMusic() == 1)	// If there is resume it
			{
				Mix_ResumeMusic();
			}
			else
			{
				Mix_PauseMusic();
			}
		}
	}
}

bool j1Audio::PlayFxIntro(unsigned int id, int repeat, int volume)
{
	bool ret = false;
	int vol = 0;
	vol = volume;
	if (id > 0 && id <= fx.size())
	{
		std::list<Mix_Chunk*>::iterator it = fx.begin();
		std::advance(it, id - 1);
		Mix_Chunk* sentchunk = *it;
		Mix_VolumeChunk(sentchunk, vol);
		Mix_PlayChannel(-1, sentchunk, repeat);
	}

	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat, int volume)
{
	bool ret = false;
	int vol = 0;
	if(App->mainmenu != nullptr)
		if (App->mainmenu->GetMenu().fx != nullptr)
			vol = App->mainmenu->GetMenu().fx->Value;
	if(!active)
		return false;
	if (volume > 0)
		vol = volume;
	if(id > 0 && id <= fx.size())
	{
		std::list<Mix_Chunk*>::iterator it = fx.begin();
		std::advance(it, id - 1);
		Mix_Chunk* sentchunk = *it;
		Mix_VolumeChunk(sentchunk,vol);
		Mix_PlayChannel(-1, sentchunk, repeat);
	}

	return ret;
}

void j1Audio::StopFx(int channel)
{
	Mix_HaltChannel(channel);
	return;
}

bool j1Audio::PlaySpatialFx(uint id, uint channel_angle, uint distance, int repeat)
{
	bool ret = false;

	if (!active)
		return false;

	Mix_Chunk* chunk = NULL;

	if (id > 0 && id <= fx.size())
	{
		std::list <Mix_Chunk*>::const_iterator it;
		it = std::next(fx.begin(), id - 1);
		chunk = *it;
	}
	//LOG("volume of chunk %d", Mix_VolumeChunk(chunk, -1));
	
	if (chunk != nullptr)
	{
		while (Mix_Playing(channel_angle) == 1)	// If the channel is already playing, choose the next channel that we already allocated with Mix_AllocateChannels()
		{
			channel_angle++;

			if (channel_angle > 360)
				channel_angle = 0;
		}
		Mix_Volume(channel_angle, (int)(App->mainmenu->GetMenu().fx->Value * 1.28f));

		Mix_SetPosition(channel_angle, channel_angle, (uint)((distance * 255)/MAX_DISTANCE));

		//Mix_PlayChannel(channel_angle, chunk, repeat);	
		//LOG("volume of chunk %d", Mix_Volume(channel_angle, -1));
		ret = true;
	}

	return ret;
}

uint j1Audio::GetAngle(iPoint player_pos, iPoint enemy_pos)
{
	iPoint vector_pos = player_pos - enemy_pos;				
	iPoint vector_axis = { 0, 1 };							

	double dot_x = vector_axis.y * vector_pos.y;			
	double det_y = -(vector_axis.y * vector_pos.x);			

	float f_angle = (atan2(det_y, dot_x)) * RAD_TO_DEG;		

	if (f_angle < 0)										
		f_angle += 360;

	return uint(f_angle);
}

uint j1Audio::GetDistance(iPoint player_pos, iPoint enemy_pos)
{

	uint distance = sqrt(pow(player_pos.x - enemy_pos.x, 2) + pow(player_pos.y - enemy_pos.y, 2));	
	//LOG("Get Distance got %d distance between %s and %s", distance, "player", "enemy");								
	uint distance_scaled = (distance/* * MAX_DISTANCE*/);
	if (distance_scaled > MAX_DISTANCE)																
		distance_scaled = MAX_DISTANCE;

	return distance_scaled;
}

bool j1Audio::Update(float dt)
{
	Mix_VolumeMusic(App->mainmenu->GetMenu().music->Value);
	return true;

}

//uint j1Audio::makeLogarithmic(uint distance)
//{
//	uint ret;
//	//distance = 
//	return ret;
//}
