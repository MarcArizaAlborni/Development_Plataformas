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

	if (App->map->Load("SimpleLevel1.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/rosa.png");

	App->audio->PlayMusic(App->map->data.MusicAudio_Files.GetString());

	for (iPoint i = { 50, 288 }; i.x <= 50; i.x += 40)
	{
		App->entityManager->AddEnemies(i, SKELETON);
		//App->entityManager->AddEnemies(i, SKULL);
		//App->entityManager->AddEnemies(i, SLIME);
		App->entityManager->AddEnemies({ 300,100 }, BEE);
	}

	App->entityManager->CreateEntity(PLAYER);


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	iPoint Player_pos = App->map->WorldToMap(0,0);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = Player_pos;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");
		//App->SaveGame();
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->fade->FadeToBlack("SimpleLevel1.tmx");

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->fade->FadeToBlack("SimpleLevel2.tmx");

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
	//iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	//p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		/*App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);
*/
	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}



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

	return true;
}




bool j1Scene::Save(pugi::xml_node& data)const {

	pugi::xml_node mapname = data.append_child("");


	return true;
}