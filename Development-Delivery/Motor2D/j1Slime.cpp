#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entities.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Slime.h"
#include "j1Player.h"
#include "j1Map.h"


j1Slime::j1Slime(iPoint pos, EntitiesType type) : j1Entities(pos, EntitiesType::SLIME)
{
	idle.PushBack({ 0,0,32,32 });
	idle.PushBack({ 0,32,23,32 });

	walking.PushBack({ 24,0,22,32 });
	walking.PushBack({ 24,32,22,32 });

	death.PushBack({ 46,0,30,32 });
	death.PushBack({ 46,32,30,32 });

}

j1Slime::~j1Slime()
{
}

bool j1Slime::Awake(pugi::xml_node& node)
{
	return true;
}

bool j1Slime::Start()
{
	texture = App->tex->Load("Sprites/Slime.png");
	InitEntity();
	return true;
}

bool j1Slime::PreUpdate()
{
	return true;
}

bool j1Slime::Update(float dt)
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

	SLMrect.x = position.x;
	SLMrect.y = position.y;

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

bool j1Slime::PostUpdate()
{
	return true;
}

bool j1Slime::CleanUp()
{
	App->tex->UnLoad(texture);

	return true;
}

bool j1Slime::Load(pugi::xml_node &node)
{
	return true;
}

bool j1Slime::Save(pugi::xml_node &node) const
{
	return true;
}

void j1Slime::OnCollision(Collider* A, Collider* B)
{

}

bool j1Slime::InitEntity()
{

	SLMwith = 32;
	SLMheight = 32;

	state = IdleState;

	animation = &idle;
	SLMrect = { position.x, position.y, SLMwith, SLMheight };
	collider = App->collision->AddCollider(SLMrect, ObjectType::Slime, App->entityManager);

	return true;
}