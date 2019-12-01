#include <math.h>
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "Brofiler/Brofiler.h"

j1FadeToBlack::j1FadeToBlack()
{
	name.create("fade");

}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	BROFILER_CATEGORY("FadeToBlack Start();", Profiler::Color::CornflowerBlue)

	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;

	int width = App->win->GetWidth();
	int height = App->win->GetHeight();
	int scale = App->win->GetScale();

	screen = { 0, 0, width * scale, height * scale };
}



// Update: draw background
bool j1FadeToBlack::Update(float dt)
{
	BROFILER_CATEGORY("FadeToBlack Update();", Profiler::Color::FireBrick)

	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			ChangeMap(mapname);
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{

		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 255, 255, 255, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(const char* mapname,  float time)
{
	BROFILER_CATEGORY("FadeToBlack FadeToBlack();", Profiler::Color::FloralWhite)

	bool ret = false;

	this->mapname = mapname;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool j1FadeToBlack::ChangeMap(const char* newMap)
{

	BROFILER_CATEGORY("FadeToBlack ChangeMap();", Profiler::Color::ForestGreen)

	bool ret = true;

	App->map->CleanUp();						
	
	App->collision->collider.clear();	
	App->entityManager->CleanEntities();					

	App->map->Load(newMap);					
	App->collision->LoadCollider();	
	App->entityManager->player->InitEntity();

	return ret;
}