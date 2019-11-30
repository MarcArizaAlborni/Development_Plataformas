#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Skeleton.h"
#include "j1Entities.h"

j1Skeleton::j1Skeleton(iPoint pos, EntitiesType type) : j1Entities(pos, EntitiesType::SKELETON)
{
	idle.PushBack({ 0,0,22,32 });
	idle.PushBack({ 0,0,22,32 });
	idle.PushBack({ 0,0,22,32 });
	idle.PushBack({ 0,0,22,32 });
	idle.PushBack({ 0,0,22,32 });
	idle.PushBack({ 0,0,22,32 });
	idle.speed = 0.5;

	walking.PushBack({0,65,0,32});

}

j1Skeleton::~j1Skeleton()
{
}

bool j1Skeleton::Awake(pugi::xml_node& node)
{
	return true;
}

bool j1Skeleton::Start()
{
	texture = App->tex->Load("Sprites/Skeleton.png");
	InitEntity();
	return true;
}

bool j1Skeleton::PreUpdate()
{
	if (state == IdleState)
	{
		
	}

	return true;
}

bool j1Skeleton::Update(float dt)
{
	position = IniPos;

	switch (state)
	{
	case IdleState:
		animation = &idle;
		break;
	}
	
	SDL_Rect r = animation->GetCurrentFrame();

	if (flip == false)
	{
		BlitEntities(r, flip, position.x, position.y);
	}
	else
	{
		BlitEntities(r, flip, position.x, position.y);
	}
	return true;
}

bool j1Skeleton::PostUpdate()
{
	return true;
}

bool j1Skeleton::CleanUp()
{
	App->tex->UnLoad(texture);

	return true;
}

bool j1Skeleton::Load(pugi::xml_node &node)
{
	return true;
}

bool j1Skeleton::Save(pugi::xml_node &node) const
{
	return true;
}

void j1Skeleton::OnCollision(Collider* A, Collider* B)
{

}

bool j1Skeleton::InitEntity()
{
	IniPos.x = 0;
	IniPos.y = 0;

	state = IdleState;
	animation = &idle;

	return true;
}