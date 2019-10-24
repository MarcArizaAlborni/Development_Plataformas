#include "j1App.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Map.h"

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
	DebugDraw();
	return true;
}

bool j1Collision::CleanUp()
{
	return true;
}


void j1Collision::DebugDraw()
{
	if (debug == false)
		return;

	//Uint8 alpha = 100;
	//for (uint i = 0; i < ; ++i)
	//{
	//	if (collider[i] == nullptr)
	//		continue;

	//	switch ()
	//	{
	//	case ObjectType::Platform: // red
	//		App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
	//		break;
	//	case ObjectType::Player: // yellow
	//		App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
	//		break;
	//	}
	//}



}