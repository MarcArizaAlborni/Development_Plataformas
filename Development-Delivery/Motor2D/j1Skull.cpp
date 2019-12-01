#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entities.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Skull.h"
#include "j1Player.h"
#include "j1Map.h"


j1Skull::j1Skull(iPoint pos, EntitiesType type) : j1Entities(pos, EntitiesType::SKELETON)
{
	idle.PushBack({ 0,0,40,40 });
	idle.PushBack({ 40,0,40,40 });
	idle.PushBack({ 80,0,40,40 });
	idle.PushBack({ 120,0,40,40 });
	idle.PushBack({ 0,0,40,40 });
	idle.PushBack({ 40,40,40,40 });
	idle.PushBack({ 80,40,40,40 });
	idle.PushBack({ 120,40,40,40 });
	idle.PushBack({ 0,80,40,40 });
	idle.PushBack({ 40,80,40,40 });
	idle.PushBack({ 80,80,40,40 });
	idle.PushBack({ 120,80,40,40 });
	idle.PushBack({ 0,120,40,40 });
	idle.PushBack({ 40,120,40,40 });
	idle.PushBack({ 80,120,40,40 });
	idle.PushBack({ 120,120,40,40 });
	idle.speed = 0.3f;
}

j1Skull::~j1Skull()
{
}

bool j1Skull::Awake(pugi::xml_node& node)
{
	return true;
}

bool j1Skull::Start()
{
	texture = App->tex->Load("Sprites/Skull.png");
	InitEntity();
	return true;
}

bool j1Skull::PreUpdate()
{
	return true;
}

bool j1Skull::Update(float dt)
{

	switch (state)
	{
	case IdleState:
		flip = true;
		animation = &idle;
		break;

	case DeadState:
		animation = &death;
		break;

	}

	SLLrect.x = position.x;
	SLLrect.y = position.y;

	collider->SetPos(position.x, position.y);

	SDL_Rect r = animation->GetCurrentFrame();

	if (flip == false)
	{
		BlitEntities(r, flip, position.x, position.y);
		collider->SetPos(position.x, position.y);
	}
	else
	{
		BlitEntities(r, flip, position.x, position.y);
		collider->SetPos(position.x + 8, position.y);
	}
	return true;
}

bool j1Skull::PostUpdate()
{
	return true;
}

bool j1Skull::CleanUp()
{
	App->tex->UnLoad(texture);

	return true;
}

bool j1Skull::Load(pugi::xml_node &node)
{
	return true;
}

bool j1Skull::Save(pugi::xml_node &node) const
{
	return true;
}

void j1Skull::OnCollision(Collider* A, Collider* B)
{

}

bool j1Skull::InitEntity()
{

	SLLwith = 32;
	SLLheight = 32;

	state = IdleState;

	animation = &idle;
	SLLrect = { position.x, position.y, SLLwith, SLLheight };
	collider = App->collision->AddCollider(SLLrect, ObjectType::Skull, App->entityManager);

	return true;
}