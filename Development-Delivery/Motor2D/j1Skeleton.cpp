#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entities.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Skeleton.h"
#include "j1Player.h"
#include "j1Map.h"


j1Skeleton::j1Skeleton(iPoint pos, EntitiesType type) : j1Entities(pos, EntitiesType::SKELETON)
{
	idle.PushBack({ 0,0,24,32 });
	idle.PushBack({ 0,32,24,32 });
	idle.PushBack({ 0,64,24,32 });
	idle.PushBack({ 0,96,24,32 });
	idle.PushBack({ 0,128,24,32 });
	idle.PushBack({ 0,160,24,32 });
	idle.PushBack({ 0,192,24,32 });
	idle.PushBack({ 0,224,24,32 });
	idle.PushBack({ 0,256,24,32 });
	idle.PushBack({ 0,288,24,32 });
	idle.speed = 0.5;

	walking.PushBack({24,0,22,32});
	walking.PushBack({24,32,22,32});
	walking.PushBack({24,64,22,32});
	walking.PushBack({24,96,22,33});
	walking.PushBack({24,129,22,33});
	walking.PushBack({24,162,22,33});
	walking.PushBack({24,195,22,32});
	walking.PushBack({24,227,22,32});
	walking.PushBack({24,259,22,32});
	walking.PushBack({24,291,22,32});
	walking.PushBack({24,323,22,32});
	walking.PushBack({24,355,22,32});
	walking.PushBack({24,387,22,32});
	walking.speed = 0.5;

	death.PushBack({ 46,0,30,32 });
	death.PushBack({ 46,32,30,32 });
	death.PushBack({ 46,64,30,32 });
	death.PushBack({ 46,96,30,32 });
	death.PushBack({ 46,128,30,32 });
	death.PushBack({ 46,160,30,32 });
	death.PushBack({ 46,192,30,32 });
	death.PushBack({ 46,224,30,32 });
	death.PushBack({ 46,256,30,32 });
	death.PushBack({ 46,288,30,32 });
	death.PushBack({ 46,320,30,32 });
	death.PushBack({ 46,352,30,32 });
	death.PushBack({ 46,384,30,32 });
	death.speed = 0.3;
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
	return true;
}

bool j1Skeleton::Update(float dt)
{
	position = IniPos;

	switch (state)
	{
	case IdleState:
		flip = true;
		position.x = 100*dt;
		animation = &idle;
		break;

	case DeadState:
		animation = &death;
		break;

	}
	
	SKrect.x = position.x;
	SKrect.y = position.y;

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
		collider->SetPos(position.x+8, position.y);
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
	IniPos.x = 6;
	IniPos.y = 270;
	SKwith = 21;
	SKheight = 35;

	state = IdleState;

	animation = &idle;
	SKrect = { position.x, position.y, SKwith, SKheight };
	collider = App->collision->AddCollider(SKrect, ObjectType::Skeleton, App->entityManager);

	return true;
}