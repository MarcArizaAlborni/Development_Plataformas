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

bool j1Entities::Start()
{
	BROFILER_CATEGORY("Entity Start()", Profiler::Color::MistyRose)
	return true;
}

bool j1Entities::PreUpdate()
{
	BROFILER_CATEGORY("Entity Start()", Profiler::Color::MediumSpringGreen)
	return true;
}

bool j1Entities::Update(float dt)
{
	BROFILER_CATEGORY("Entity Start()", Profiler::Color::Turquoise)
	return true;
}

bool j1Entities::PostUpdate()
{
	BROFILER_CATEGORY("Entity Start()", Profiler::Color::DarkKhaki)
	return true;
}

bool j1Entities::CleanUp() 
{
	BROFILER_CATEGORY("Entity Start()", Profiler::Color::Gray)
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
