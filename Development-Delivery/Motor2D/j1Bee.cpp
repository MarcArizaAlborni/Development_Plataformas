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
	idle.PushBack({ 0,0,36,32  });
	idle.PushBack({ 0,32,36,32 });
	idle.PushBack({ 0,64,36,32 });
	idle.PushBack({ 0,96,36,32 });
	idle.PushBack({ 0,128,36,32 });
	idle.PushBack({ 0,160,36,32 });
	idle.speed = 0.5f;

	walking.PushBack({ 36,0,36,32 });
	walking.PushBack({ 36,32,36,32 });
	walking.PushBack({ 36,64,36,32 });
	walking.PushBack({ 36,96,36,32 });
	walking.PushBack({ 36,128,36,32 });
	walking.PushBack({ 36,160,36,32 });
	walking.PushBack({ 36,192,36,32 });
	walking.PushBack({ 36,224,36,32 });
	walking.speed = 0.5f;

	death.PushBack({ 72,0,36,32 });
	death.PushBack({ 72,32,36,32 });
	death.PushBack({ 72,64,36,32 });
	death.PushBack({ 72,96,36,32 });
	death.PushBack({ 72,128,36,32 });
	death.speed = 0.3f;

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

	Beerect = { position.x, position.y, Beewith, Beeheight };
	collider = App->collision->AddCollider(Beerect, ObjectType::Bee, App->entityManager);

	return true;
}