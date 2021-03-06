#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1LogoScene.h"
#include "j1Pathfinding.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1EntityManager.h"
#include "j1ParticleManager.h"
#include "j1GUI.h"
#include "j1Fonts.h"
#include "j1InGameUI.h"
#include "j1TransitionManager.h"
#include "j1SceneManager.h"
#include "j1MainMenuUI.h"
#include "j1Minimap.h"
#include "j1Font.h"
#include "j1QuestManager.h"
#include "j1Map.h"
#include "j1DialogSystem.h"
#include "j1AnimationManager.h"
#include "j1Explanation.h"
#include "j1Fog.h"
#include "j1AssetManager.h"

#include <thread>


#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	want_to_save = want_to_load = false;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	map = new j1Map();
	scene = new j1Scene();
	scene2 = new j1Scene2();
	scene3 = new j1LogoScene();
	InGameUI = new j1InGameUI();
	pathfinding = new j1PathFinding();
	player = new j1Player();
	entitymanager = new j1EntityManager();
	pmanager = new j1ParticleManager();
	gui = new j1GUI();
	fonts = new j1Fonts();
	transitions = new j1TransitionManager();
	scenemanager = new j1SceneManager();
	mainmenu = new j1MainMenuUI();
	minimap = new j1Minimap();
	font = new j1Font();
	quest = new j1QuestManager();
	anim = new j1AnimationManager();
	dialog = new j1DialogSystem();
	expl = new j1Explanation();
	fog = new j1Fog();
	assets = new j1AssetManager();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	
	AddModule(assets);
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(anim);
	AddModule(fonts);
	AddModule(font);
	AddModule(player);
	AddModule(minimap);
	AddModule(map);
	AddModule(fog);
	AddModule(scene);
	AddModule(scene2);
	AddModule(scene3);

	AddModule(InGameUI);
	AddModule(mainmenu);
	AddModule(audio);
	AddModule(entitymanager);

	AddModule(gui);
	AddModule(expl);
	AddModule(transitions);
	AddModule(quest);
	AddModule(pathfinding);
	AddModule(dialog);
	AddModule(pmanager);
	AddModule(scenemanager);

	//Render (last to swap buffer)
	AddModule(render);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());
	}

	if(ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	////
	App->scenemanager->ChangeScene(3);
	App->audio->PlayFxIntro(App->audio->logo_audio,0,100);

	font_name = App->fonts->Load("textures/ui/font.png", "ABCDEFGHIJKLMNOPQRSTUWYZ0123456789-= ", 1);
	////
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();


	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	//LOG("DT: %f", dt);
	frame_time.Start();
}

void logsomething(char* something)
{
	LOG(something);
}
// ---------------------------------------------
void j1App::FinishUpdate()
{
	if(want_to_save == true)
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	char* cap = "OFF";
	char* vsync = "OFF";

	if (fpscap)
		cap = "ON";
	else
		cap = "OFF";

	if ((App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) && framerate_cap > 10)
		framerate_cap -= 10;
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		framerate_cap += 10;
	//if (App->render->vsync)
	//	vsync = "ON";
	//else
	//	vsync = "OFF";
	////////char* thread1 = "thread1";								   Thread optimization testing, do not touch
	////////char* thread2 = "thread2";								   Thread optimization testing, do not touch
	////////char* thread3 = "thread3";								   Thread optimization testing, do not touch
	////////char* thread4 = "thread4";								   Thread optimization testing, do not touch
	////////														   Thread optimization testing, do not touch
	////////std::thread t1(logsomething, thread1);					   Thread optimization testing, do not touch
	////////std::thread t2(logsomething, thread2);					   Thread optimization testing, do not touch
	////////std::thread t3(logsomething, thread3);					   Thread optimization testing, do not touch
	////////std::thread t4(logsomething, thread4);					   Thread optimization testing, do not touch
	////////t1.join();												   Thread optimization testing, do not touch
	////////t2.join();												   Thread optimization testing, do not touch
	////////t3.join();												   Thread optimization testing, do not touch
	////////t4.join();												   Thread optimization testing, do not touch
	sprintf_s(title, 256, "Sea Conquest || FPS: %02u / EstFPS: %02u/ Av.FPS: %.2f / Last Frame Ms: %02u / Cap: %s / dt: %f / Camera.x: %d / Camera.y: %d",
		frames_on_last_update, framerate_cap, avg_fps, last_frame_ms, cap, dt,App->render->camera.x, App->render->camera.y);

	App->win->SetTitle(title);

	//LOG("Last frame ms %d", last_frame_ms);
	//App->win->SetTitle(App->input->GetText().GetString());
	if ((framerate_cap > 0) && fpscap)
	{
		if ((last_frame_ms < (1000 / framerate_cap))) {
			SDL_Delay((1000 / framerate_cap) - last_frame_ms);
		}
	}

	if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		SaveGame("save_game.xml");

	if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		godmode = !godmode;

	if (App->godmode)
	{
		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			show_pathnodes = !show_pathnodes;
		if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
			ignore_fog = !ignore_fog;
		if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
			numerate_entities = !numerate_entities;
		if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			show_chasing_range = !show_chasing_range;
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		show_pathnodes = false;
		ignore_fog = false;
		numerate_entities = false;
	}

	clicking_ui = false;
}



// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		if (game_pause == true)
			dt = 0.0f;

		ret = item->data->Update(dt);
	}

	return ret;
}

bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

 		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
float j1App::GetDT() const
{
	return 0.0f;
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

void j1App::LoadGame()
{
	LoadGameNow();
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	want_to_save = true;
	save_game.create(file);
}

bool j1App::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document save_doc;
	pugi::xml_parse_result result = save_doc.load_file("save_game.xml");

	pugi::xml_node node = save_doc.child("game");

	App->render->Load(node);
	App->entitymanager->Load(node);

	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;
	
	pugi::xml_document save_doc;
	pugi::xml_parse_result result = save_doc.load("save_game.xml");
	save_doc.reset();
	pugi::xml_node node = save_doc.append_child("game");

	App->render->Save(node);
	App->entitymanager->Save(node);
	
	save_doc.save_file("save_game.xml");
	want_to_save = false;
	
	return ret;
}