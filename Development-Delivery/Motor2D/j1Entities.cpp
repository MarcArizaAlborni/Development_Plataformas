#include "j1Entities.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Map.h"
#include "p2Animation.h"
#include "j1Player.h"
#include "p2Log.h"
#include "Brofiler/Brofiler.h"



j1Entities::j1Entities(iPoint pos, EntitiesType type)	
{
	position.x = pos.x;
	position.y = pos.y;
	this->type = type;
}

bool j1Entities::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1Entities::Start()
{
	return true;
}

bool j1Entities::PreUpdate()
{
	return true;
}

bool j1Entities::Update(float dt)
{
	return true;
}

bool j1Entities::PostUpdate()
{
	return true;
}

bool j1Entities::CleanUp() 
{
	return true;
}

bool j1Entities::Load(pugi::xml_node &)
{
	return true;
}

bool j1Entities::Save(pugi::xml_node &)const
{
	return true;
}

void j1Entities::BlitEntities(SDL_Rect r, bool flip, float x, float y)
{
	BROFILER_CATEGORY("BlitEntity", Profiler::Color::Thistle)

		if (flip == false)
			App->render->Blit(texture,  x,  y, &r, flip);
		else
			App->render->Blit(texture,  x,  y, &r, flip);
}

bool j1Entities::InitEntity()
{
	return true;
}
