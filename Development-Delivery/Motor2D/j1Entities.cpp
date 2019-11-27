#include "j1Entities.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Map.h"
#include "p2Animation.h"
#include "j1Player.h"
#include "p2Log.h"



j1Entity::j1Entity(iPoint position, EntityType type) : position(position), type(type), texture(nullptr), collider(nullptr), animation(nullptr)	
{
	return;
}



bool j1Entity::Update(float dt, bool DoLogic)
{
	return true;
}


bool j1Entity::CleanUp() {

	if (collider != nullptr)
		collider->to_delete = true;

	return true;
}


