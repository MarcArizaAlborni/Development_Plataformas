#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entities.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Skeleton.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Pathfinding.h"


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

	gravity = 0;
}

j1Skeleton::~j1Skeleton()
{
}

bool j1Skeleton::Awake(pugi::xml_node& node)
{
	//state = IdleState;
	
	
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
	position.y += gravity;
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	return true;
}

bool j1Skeleton::Update(float dt)
{
	
	if (state != (JumpState || FallState)) {
		ComparePositions();
	}
	
	switch (state)
	{
	case IdleState:

		TouchingColliderRight = false;
		TouchingColliderLeft = false;
		flip = true;
		animation = &idle;
		//ComparePositions();
		LOG("SKELETON IDLE");
		break;

	case DeadState:
		animation = &death;
		break;

	case LeftState:
		if (GoLeft) {
			Movement(1);//1 FOR LEFT 0 FOR RIGHT
			LOG("SKELETON LEFT");
			animation = &walking;
			TouchingColliderRight = false;
			TouchingColliderLeft = false;
		}
		if (position.y +50 < App->entityManager->player->position.y &&App->entityManager->player->position.x < 
position.x && (App->entityManager->player->position.x - position.x) <= DETECTION_RANGE) {

			state=JumpState;
		}
		
		break;


	case RightState:
		if (GoRight) {
			LOG("SKELETON RIGHT");
			Movement(0);//1 FOR LEFT 0 FOR RIGHT
			animation = &walking;
			TouchingColliderRight = false;
			TouchingColliderLeft = false;
		}
		if (position.y + 50 < App->entityManager->player->position.y && (App->entityManager->player->position.x - position.x) <= DETECTION_RANGE) {


 			state = FallState;
		}
		
		break;

	case JumpState:
		animation = &hit;
		break;

	case FallState:
		animation = &hit;
		if (position.y < App->entityManager->player->position.y) {//NECESSITO QUE L'ESQUELET SIGUI DE LA MATEIXA ALTURA QUE LA DEL PLAYER O TROBAR LA 
		//DIFFERENCIA D'ALTURA PERQUE NO ES QUEDI FLOTANT
			position.y += 5;
		}
		if (position.y == App->entityManager->player->position.y) {
			state = IdleState;
		}
		break;
	}
	

	return true;
}

bool j1Skeleton::PostUpdate()
{
	

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
		collider->SetPos(position.x + 8, position.y);
	}
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
	
	if (A->type == ObjectType::Skeleton) {

		

		if (B->type == ObjectType::LateralPlatform) {


			//LEFT TO RIGHT
			if (A->rect.x + A->rect.w >= B->rect.x && A->rect.x <= B->rect.x)  //EL BO
			{
				
				TouchingColliderRight = true;
				if (App->entityManager->player->position.x > position.x) {
					
					state = JumpState;
					GroundJump();
				}
				LOG("SKELETON INTO WALL FROM THE LEFT");

			}
			//Right to Left
			else if (A->rect.x <= B->rect.x + B->rect.w && A->rect.x + A->rect.w >= B->rect.x + B->rect.w) {

				TouchingColliderLeft = true;
				if (App->entityManager->player->position.x < position.x) {
					GroundJump();
				}
				LOG("SKELETON INTO WALL FROM THE RIGHT");
			}

		}
	}
}

bool j1Skeleton::InitEntity()
{

	SKwith = 21;
	SKheight = 35;

	state = IdleState;

	animation = &walking;
	SKrect = { position.x, position.y, SKwith, SKheight };
	collider = App->collision->AddCollider(SKrect, ObjectType::Skeleton, App->entityManager);

	return true;
}

void j1Skeleton::ComparePositions()
{
	if (Dead != true) {
		if (App->entityManager->player != nullptr) {
			//LEFT
			if (((App->entityManager->player->position.x - position.x) >= DETECTION_RANGE)
				|| (App->entityManager->player->position.x - position.x) <= NEGATIVE_DETECTION_RANGE) {

				state = IdleState;
			}
			else
			{
				if (App->entityManager->player->position.x < position.x && (App->entityManager->player->position.x - position.x) <= DETECTION_RANGE) {
					state = LeftState;
					GoLeft = true;
					GoRight = false;
					LOG("GOING LEFT");
				}
				else if (App->entityManager->player->position.x > position.x && (App->entityManager->player->position.x - position.x) >= -DETECTION_RANGE) {
					GoRight = true;
					GoLeft = false;
					state = RightState;
					LOG("GOING RIGHT");
				}
				else
				{
					GoLeft = false;
					GoRight = false;
				}
			}
		}
	}
}

void j1Skeleton::GroundJump()
{
	if (state == JumpState) {
		if (position.y > App->entityManager->player->position.y) {
			animation = &hit;
			position.y -= 5;
		}
		if (position.y < App->entityManager->player->position.y) {//NECESSITO QUE L'ESQUELET SIGUI DE LA MATEIXA ALTURA QUE LA DEL PLAYER O TROBAR LA 
		//DIFFERENCIA D'ALTURA PERQUE NO ES QUEDI FLOTANT
			state = FallState;
		}
	}
	
	
		
}

void j1Skeleton::Movement(bool CurrentState)
{
	//DRETA
	if (CurrentState == 0 && TouchingColliderRight==false) {
		position.x += 2;
	}
	//ESQUERRA
	else if(CurrentState==1 && TouchingColliderLeft==false) {
		position.x -= 2;
	}
	
}

//void j1Skeleton::LookForPath()
//{
//	iPoint origin = { App->map->WorldToMap((int)position.x + Pathing.x, (int)position.y + Pathing.y) };
//	iPoint destination;
//
//	if (position.x < App->entityManager->player->position.x){
//		destination = { App->map->WorldToMap((int)(App->entityManager->player->position.x), (int)(App->entityManager->player->position.y))
//		};
//	}
//	
//
//	if (App->pathfinding->IsWalkable(destination) && App->pathfinding->IsWalkable(origin))
//	{
//		path = App->pathfinding->CreatePath(origin, destination);
//
//		Move(*path, dt);
//		path_created = true;
//	}
//}


//void j1Skeleton::Move(p2DynArray<iPoint>& path, float dt)
//{
//	
//	
//		direction = App->pathfinding->CheckDirectionGround(path);
//
//		if (direction == PATH_MOVEMENT::LEFT)
//		{
//			animation = &runAnim;
//			position.x -= speed.x * dt;
//			flip = true;
//		}
//		else if (direction == PATH_MOVEMENT::RIGHT)
//		{
//			animation = &runAnim;
//			position.x += speed.x * dt;
//			flip = false;
//		}
//	
//}