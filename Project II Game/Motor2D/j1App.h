#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Timer.h"
#include "j1PerfTimer.h"


// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Scene2;
class j1LogoScene;
class j1PathFinding;
class j1Player;
class j1EntityManager;
class j1ParticleManager;
class j1GUI;
class j1Fonts;
class j1InGameUI;
class j1TransitionManager;
class j1SceneManager;
class j1MainMenuUI;
class j1Minimap;
class j1Font;
class j1QuestManager;
class j1Map;
class j1DialogSystem;
class j1AnimationManager;
class j1Explanation;


class j1App 
{
public:

	j1App(int argc, char* args[]);
	virtual ~j1App();

	bool Awake();

	bool Start();
	bool Update();
	bool CleanUp();

	void AddModule(j1Module* module);

	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	float GetDT() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

private:

	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	void PrepareUpdate();
	void FinishUpdate();

	bool PreUpdate();
	bool DoUpdate();
	bool PostUpdate();

	bool LoadGameNow();
	bool SavegameNow() const;

public:

	j1Window*				win = NULL;
	j1Input*				input = NULL;
	j1Render*				render = NULL;
	j1Textures*				tex = NULL;
	j1Audio*				audio = NULL;
	j1Scene*				scene = NULL;
	j1Scene2*				scene2 = NULL;
	j1LogoScene*			scene3 = NULL;
	j1PathFinding*			pathfinding = NULL;
	j1Player*				player = NULL;
	j1EntityManager*		entitymanager = NULL;
	j1ParticleManager*		pmanager = NULL;
	j1GUI*					gui = NULL;
	j1Fonts*				fonts = NULL;
	j1InGameUI*				InGameUI = NULL;
	j1TransitionManager*	transitions = NULL;
	j1SceneManager*			scenemanager = NULL;
	j1MainMenuUI*			mainmenu = NULL;
	j1Minimap*				minimap = NULL;
	j1Font*					font = NULL;
	j1QuestManager*			quest = NULL;
	j1Map*					map = NULL;
	j1DialogSystem*			dialog = NULL; 
	j1AnimationManager*		anim = NULL;
	j1Explanation*			expl = NULL;

private:

	p2List<j1Module*>	modules;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	p2SString			load_game;
	mutable p2SString	save_game;

	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;

public:

	uint32				framerate_cap = 144;
	bool				fpscap = true;
	uint64				frame_count = 0;
	float dt = 0.1f;
	uint32 getFpsCap() { return framerate_cap; };
	void setFpsCap(uint32 fps);

	bool				game_pause = false;
	bool				godmode = true;

};

extern j1App* App;

#endif