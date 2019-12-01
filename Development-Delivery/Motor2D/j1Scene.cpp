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

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

	debug_path = false;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	debug_tex = App->tex->Load("maps/rosa.png");

	App->audio->PlayMusic(App->map->data.MusicAudio_Files.GetString());

	if (Level1 == true)
	{
		LoadLevel1();
	}

	if( Level2 == true)
	{
		LoadLevel2();
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");
		
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		Level1 = true;
		Level2 = false;
		App->fade->FadeToBlack("SimpleLevel1.tmx");
	}
		
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		Level1 = false;
		Level2 = true;

		App->fade->FadeToBlack("SimpleLevel2.tmx");
	}

	App->map->Draw();

	//p2SString title("Project Ceta");

	//App->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
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
	LOG("Freeing scene");

	App->map->CleanUp();
	App->collision->CleanUp();
	if (App->entityManager->player != nullptr)
		App->entityManager->player->CleanUp();
	App->entityManager->CleanUp();

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	Level1 = data.child("Level1").attribute("active").as_bool();
	Level2 = data.child("Level2").attribute("active").as_bool();

	LoadNewLevel();

	return true;
}

bool j1Scene::Save(pugi::xml_node& data)const {
	
	pugi::xml_node tutorial = data.append_child("Level1");
	tutorial.append_attribute("active") = Level1;

	pugi::xml_node level1 = data.append_child("Level2");
	level1.append_attribute("active") = Level2;

	return true;
}

void j1Scene::LoadNewLevel()
{
	CleanUp();

	Start();
	App->collision->Start();
	App->entityManager->Start();

}

void j1Scene::LoadLevel1()
{
	App->map->Load("SimpleLevel1.tmx");

	SpawnLevel1Entities();
}

void j1Scene::SpawnLevel1Entities()
{
	App->entityManager->AddEnemies({ 100, 254 }, SLIME);
	App->entityManager->AddEnemies({ 500, 254 }, BEE);
	App->entityManager->CreateEntity(PLAYER);
}

void j1Scene::LoadLevel2()
{
	App->map->Load("SimpleLevel2.tmx");

	SpawnLevel2Entities();
}

void j1Scene::SpawnLevel2Entities()
{
	App->entityManager->AddEnemies({ 100, 254 }, SKELETON);
	App->entityManager->AddEnemies({ 300, 254 }, SKULL);
	App->entityManager->CreateEntity(PLAYER);
}

void j1Scene::ReSpawnEntities()
{
	if (Level1 == true)
		SpawnLevel1Entities();

	else if (Level2)
		SpawnLevel2Entities();
}
