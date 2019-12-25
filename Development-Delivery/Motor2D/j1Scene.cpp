#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1FadeToBlack.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Skeleton.h"
#include "j1SceneUI.h"

#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

	debug_path = false;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	pugi::xml_node spawn = config.child("spawn");

	Skeleton1.x = spawn.child("Skeleton1").attribute("x").as_int();
	Skeleton1.y = spawn.child("Skeleton1").attribute("y").as_int();
	Skeleton2.x = spawn.child("Skeleton2").attribute("x").as_int();
	Skeleton2.y = spawn.child("Skeleton2").attribute("y").as_int();
	Skeleton3.x = spawn.child("Skeleton3").attribute("x").as_int();
	Skeleton3.y = spawn.child("Skeleton3").attribute("y").as_int();
	Skull1.x = spawn.child("Skull1").attribute("x").as_int();
	Skull1.y = spawn.child("Skull1").attribute("y").as_int();
	Bee1.x = spawn.child("Bee1").attribute("x").as_int();
	Bee1.y = spawn.child("Bee1").attribute("y").as_int();
	MapItem1.x = spawn.child("Map1").attribute("x").as_int();
	MapItem1.y = spawn.child("Map1").attribute("y").as_int();

	MapItem2.x = spawn.child("Map2").attribute("x").as_int();
	MapItem2.y = spawn.child("Map2").attribute("y").as_int();

	MapItem3.x = spawn.child("Map3").attribute("x").as_int();
	MapItem3.y = spawn.child("Map3").attribute("y").as_int();

	MapItem4.x = spawn.child("Map4").attribute("x").as_int();
	MapItem4.y = spawn.child("Map4").attribute("y").as_int();
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	BROFILER_CATEGORY("Scene Start();", Profiler::Color::SkyBlue)

	if (App->map->Load("MainMenu.tmx") == true){
		StartMainMenu();
		MainMenuLoaded = true;
	}
	
	

	//if (App->map->Load("SimpleLevel1.tmx") == true)
	//{
	//	StartMap1();
	//	//CREEM UN BOO, QUE DETECTI QUIN NIVELL S'HA CARREGAT I DESPRES CREI ELS OBJECTES QUE SIGUIN D'AQUELL MAPA
	//}
	
	debug_tex = App->tex->Load("maps/rosa.png");

	App->audio->PlayMusic(App->map->data.MusicAudio_Files.GetString());

	
	/*if (App->map->Load("SimpleLevel2.tmx") == true) {
		StartMap2();
	}*/


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene PreUpdate();", Profiler::Color::Brown)
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	//OPEN CLOSE INGAME MENU

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {

		App->scene_ui->IngameMenu();
	}


	BROFILER_CATEGORY("Scene Update();", Profiler::Color::Thistle)
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");
		//App->SaveGame();
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		App->fade->FadeToBlack("SimpleLevel1.tmx");
		StartMap1();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		App->fade->FadeToBlack("SimpleLevel2.tmx");
		StartMap2();
	}

	App->map->Draw();

	//p2SString title("Project Ceta");

	//App->win->SetTitle(title.GetString());

	if (App->input->keyboard[SDL_SCANCODE_F9] == KEY_DOWN) {

		if (debug_path) {
			debug_path = false;
		}
		else
		{
			debug_path = true;
		}
	}
	if (debug_path == false)
		return true;

	int x, y;
	App->input->GetMousePosition(x, y);
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene PostUpdate();", Profiler::Color::DarkBlue)
	//VOLUMEN

	/*if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
	{
		App->audio->general_volume += 5;
		App->audio->SetVolumeMusic();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
	{
		App->audio->general_volume -= 5;
		App->audio->SetVolumeMusic();
	}*/




	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	BROFILER_CATEGORY("Scene Start();", Profiler::Color::PeachPuff)
	LOG("Freeing scene");

	return true;
}




bool j1Scene::Save(pugi::xml_node& data)const {

	pugi::xml_node mapname = data.append_child("");


	return true;
}

void j1Scene::StartMap1()
{
	Map1Loaded = true;
	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);


	// ENEMY SPAWNS LEVEL 1
	App->entityManager->AddEnemies(Skeleton1, SKELETON);
	App->entityManager->AddEnemies(Bee1, BEE);
	App->entityManager->AddEnemies(Skeleton2, SKELETON);
	App->entityManager->AddEnemies(Skull1, SKULL);
	App->entityManager->AddEnemies(Skeleton3, SKELETON);

	App->entityManager->CreateEntity(PLAYER);
	
	//MAP ITEM ENTITY SPAWN

	App->entityManager->AddEnemies(MapItem1, MAP);
	App->entityManager->AddEnemies(MapItem2, MAP);
	App->entityManager->AddEnemies(MapItem3, MAP);
	App->entityManager->AddEnemies(MapItem4, MAP);
}

void j1Scene::StartMap2()
{
	App->entityManager->AddEnemies(MapItem1, MAP);
	App->entityManager->AddEnemies(MapItem2, MAP);
	App->entityManager->AddEnemies(MapItem3, MAP);
	App->entityManager->AddEnemies(MapItem4, MAP);

	App->entityManager->CreateEntity(PLAYER);
}

void j1Scene::StartMainMenu()
{
}
