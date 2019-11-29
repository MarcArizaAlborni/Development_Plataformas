#include <iostream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Entities.h"
#include "Brofiler/Brofiler.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	want_to_save = want_to_load = false;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	collision = new j1Collision();
	fade = new j1FadeToBlack();
	pathfinding = new j1PathFinding();
	entityManager = new j1EntityManager();
	
	


	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(entityManager);
	
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(scene);	
	AddModule(collision);
	AddModule(fade);
	

	// render last to swap buffer
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
	
	save_game = "save_file";
	load_game = "save_file";

	frame_cap = 30; 

	bool ret = false;
		
	config = LoadConfig(config_file);


	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());
		frame_cap = config.child("app").attribute("framerate_cap").as_uint();
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
{//
	startup_timer.Start();

	bool ret = true;
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

	BROFILER_CATEGORY("App Update();", Profiler::Color::Aqua)


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
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	BROFILER_CATEGORY("App FinishUpdate();", Profiler::Color::LawnGreen)


	if(want_to_save == true)
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();




	if (last_second_timer.ReadMs() > 1000)
	{
		last_second_timer.Start();
		prev_sec_frames = frames_last_second;
		frames_last_second = 0;
	}

	float avg_fps = frame_count / startup_timer.ReadSec();		//Equals seconds to the returning value of the ReadSec() method, which returns the amount of time passed in seconds. Use timer->ReadSec() to have no decimals (as its a low resolution timer)
	float seconds_since_startup = startup_timer.ReadSec();		//Gets the average frames per second by dividing the actual number of frames with the amount of seconds that have passed.
	uint32 last_frame_ms = frame_timer.Read();					//As it is the end of the update, the frame's ms can be calculated.
	uint32 frames_on_last_update = prev_sec_frames;				//Keeps track of how many frames were processed the last second.

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
		avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);

	App->win->SetTitle(title);

	float frame_cap_ms = 1000 / frame_cap;						//Calculates the frame_cap in ms (how fast a frame is processed / how fast an image is refreshed). Done for readability.
	float current_frame_ms = frame_timer.Read();				//Calculates the current frame's time spent processing. Need to declare it here so the time is consistent through the whole check. Could be below or above the cap.

	if (current_frame_ms < frame_cap_ms)						//If the current frame processing time is lower than the specified frame_cap. Timer instead of PerfTimer was used because SDL_Delay is inaccurate.
	{
		true_delay_timer.Start();

		SDL_Delay(frame_cap_ms - current_frame_ms);				//SDL_Delay delays processing for a specified time. In this case, it delays for the difference in ms between the frame cap (30fps so 33,3ms per frame) and the current frame.

		int intended_delay = frame_cap_ms - current_frame_ms;	//Done for readability. Set as the value of the intended delay.

		//LOG("Waiting for  %d milliseconds starting in  %f", intended_delay, true_delay_timer.ReadMs());
	}


}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	BROFILER_CATEGORY("App PreUpdate);", Profiler::Color::DarkOrange)


	frame_count++;									//Adds +1 to the frame count before each update loop. This variable will keep track of how many frames have been processed through all runtime.
	frames_last_second++;							//Used to keep track of how many frames there were in the last second.

	dt = frame_timer.ReadSec();						//Keeps track of the amount of time that has passed since last frame in seconds (processing time of a frame: Frame 1: 0.033secs, ...).
	frame_timer.Start();							//Starts the frame timer. Used to calculate ms per frame.

	//LOG("Time between frames: %f", dt);


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

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	BROFILER_CATEGORY("App PreUpdate);", Profiler::Color::DeepPink)
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
	BROFILER_CATEGORY("App CleanUp();", Profiler::Color::Orange)
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
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void j1App::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
	save_game.create(file);
}

bool j1App::LoadGameNow()
{
	bool ret = false;

	load_game.create("save_game.xml");

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.GetString());

	if(result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.GetString());

		root = data.child("game_state");

		p2List_item<j1Module*>* item = modules.start;
		ret = true;

		while(item != NULL && ret == true)
		{
			ret = item->data->Load(root.child(item->data->name.GetString()));
			item = item->next;
		}

		data.reset();
		if(ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.GetString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	p2List_item<j1Module*>* item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Save(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if (ret == true)
	{
		data.save_file(save_game.GetString());
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}
