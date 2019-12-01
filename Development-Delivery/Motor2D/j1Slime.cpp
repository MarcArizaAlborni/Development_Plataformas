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
	idle.PushBack({ 0,0,35,37 });
	idle.PushBack({ 35,0,35,37 });
	idle.PushBack({ 70,0,35,37 });
	idle.PushBack({ 105,0,35,37 });
	idle.PushBack({ 140,0,35,37 });
	idle.PushBack({ 175,0,35,37 });
	idle.speed = 0.3f;

	walking.PushBack({ 0,37,35,34 });
	walking.PushBack({ 35,37,35,34 });
	walking.PushBack({ 70,37,35,34 });
	walking.PushBack({ 105,37,35,34 });
	walking.PushBack({ 140,37,35,34 });
	walking.PushBack({ 175,37,35,34 });
	walking.PushBack({ 210,37,35,34 });
	walking.PushBack({ 245,37,35,34 });
	walking.speed = 0.2f;

	attack.PushBack({0,71,49,34});
	attack.PushBack({49,71,49,34});
	attack.PushBack({98,71,49,34});
	attack.PushBack({147,71,49,34});
	attack.PushBack({196,71,49,34});
	attack.PushBack({245,71,49,34});
	attack.PushBack({294,71,49,34});
	attack.PushBack({343,71,49,34});
	attack.speed = 0.2f;
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

	SLMrect = { position.x, position.y, SLMwith, SLMheight };
	collider = App->collision->AddCollider(SLMrect, ObjectType::Slime, App->entityManager);

	return true;
}