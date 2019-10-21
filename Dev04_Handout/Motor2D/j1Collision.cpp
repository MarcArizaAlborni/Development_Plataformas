
#include "j1Collision.h"

j1Collision::j1Collision()
{
	name.create("collision");
}

j1Collision::~j1Collision()
{

}

bool j1Collision::Awake(pugi::xml_node config)
{

	return true;
}

bool j1Collision::Start()
{
	
	return true;
}

bool j1Collision::PreUpdate()
{
	return true;
}

bool j1Collision::Update()
{
	return true;
}

bool j1Collision::CleanUp()
{
	return true;
}
