#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entities.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Bee.h"
#include "j1Player.h"
#include "j1Map.h"


j1Bee::j1Bee(iPoint pos, EntitiesType type) : j1Entities(pos, EntitiesType::BEE)
{
	idle.PushBack({ 0,0,32,32 });
	idle.PushBack({ 0,32,23,32 });

	walking.PushBack({ 24,0,22,32 });
	walking.PushBack({ 24,32,22,32 });

	death.PushBack({ 46,0,30,32 });
	death.PushBack({ 46,32,30,32 });

}

j1Bee::~j1Bee()
{
}

bool j1Bee::Awake(pugi::xml_node& node)
{
	return true;
}

bool j1Bee::Start()
{
	texture = App->tex->Load("Sprites/Bee.png");
	InitEntity();
	return true;
}

bool j1Bee::PreUpdate()
{
	return true;
}

bool j1Bee::Update(float dt)
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

	Beerect.x = position.x;
	Beerect.y = position.y;

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

bool j1Bee::PostUpdate()
{
	return true;
}

bool j1Bee::CleanUp()
{
	App->tex->UnLoad(texture);

	return true;
}

bool j1Bee::Load(pugi::xml_node &node)
{
	return true;
}

bool j1Bee::Save(pugi::xml_node &node) const
{
	return true;
}

void j1Bee::OnCollision(Collider* A, Collider* B)
{

}

bool j1Bee::InitEntity()
{

	Beewith = 32;
	Beeheight = 32;

	state = IdleState;

	animation = &idle;
	Beerect = { position.x, position.y, Beewith, Beeheight };
	collider = App->collision->AddCollider(Beerect, ObjectType::Bee, App->entityManager);

	return true;
}