



#include "j1Entities.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Map.h"
#include "p2Animation.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1EntityManager.h"


j1Entity::j1Entity(int x, int y, EntityType type) : position(x, y), type(type), texture(nullptr), collider(nullptr), animation(nullptr)	
{
	name.create("entities");

	return;
}

bool j1Entity::Awake(pugi::xml_node& config)
{
	return true;
}


bool j1Entity::Start()
{
	return true;
}

bool j1Entity::PreUpdate()
{
	return true;
}

bool j1Entity::Update(float dt, bool doLogic)
{
	return true;
}

bool j1Entity::PostUpdate()
{
	return true;
}

bool j1Entity::CleanUp()
{

	return true;
}


