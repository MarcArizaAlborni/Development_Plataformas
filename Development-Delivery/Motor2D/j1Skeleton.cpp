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

	walking.PushBack({ 24,0,22,32 });
	walking.PushBack({ 24,32,22,32 });
	walking.PushBack({ 24,64,22,32 });
	walking.PushBack({ 24,96,22,33 });
	walking.PushBack({ 24,129,22,33 });
	walking.PushBack({ 24,162,22,33 });
	walking.PushBack({ 24,195,22,32 });
	walking.PushBack({ 24,227,22,32 });
	walking.PushBack({ 24,259,22,32 });
	walking.PushBack({ 24,291,22,32 });
	walking.PushBack({ 24,323,22,32 });
	walking.PushBack({ 24,355,22,32 });
	walking.PushBack({ 24,387,22,32 });
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

	attack.PushBack({79,0,40,37});
	attack.PushBack({79,37,40,37});
	attack.PushBack({79,74,40,37});
	attack.PushBack({79,111,40,37});
	attack.PushBack({79,148,40,37});
	attack.PushBack({79,185,40,37});
	attack.PushBack({79,222,40,37});
	attack.PushBack({79,259,40,37});
	attack.PushBack({79,296,40,37});
	attack.PushBack({79,333,40,37});
	attack.PushBack({79,370,40,37});
	attack.PushBack({79,407,40,37});
	attack.PushBack({79,444,40,37});
	attack.PushBack({79,481,40,37});
	attack.PushBack({79,518,40,37});
	attack.PushBack({79,555,40,37});
	attack.PushBack({79,592,40,37});
	attack.PushBack({79,629,40,37});
	attack.speed = 0.3f;

}

j1Skeleton::~j1Skeleton()
{
}

bool j1Skeleton::Start()
{
	texture = App->tex->Load("Sprites/Skeleton.png");
	InitEntity();
	return true;
}

bool j1Skeleton::PreUpdate()
{
	//position.y += gravity;
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	return true;
}

bool j1Skeleton::Update(float dt)
{

	if (!Dead || state != AttackReadyState || state != AttackState) {
		ComparePositions();
	}

	switch (state)
	{
	case IdleState:
		AttackLeft, AttackRight = false;
		TouchingColliderRight = false;
		TouchingColliderLeft = false;
		flip = true;
		animation = &idle;
		//ComparePositions();
		//LOG("SKELETON IDLE");
		break;

	case DeadState:
		animation = &death;

		break;

	case LeftState:
		AttackLeft, AttackRight = false;
		if (GoLeft) {
			flip = true;
			Movement();//1 FOR LEFT 0 FOR RIGHT
			//LOG("SKELETON LEFT");
			animation = &walking;
			//TouchingColliderRight = false;
			TouchingColliderLeft = false;
		}
		break;


	case RightState:
		AttackLeft, AttackRight = false;
		
		if (GoRight) {
			flip = false;
			//LOG("SKELETON RIGHT");
			Movement();
			
			animation = &walking;
			TouchingColliderRight = false;
			//TouchingColliderLeft = false;
		}
		if (position.y + 50 < App->entityManager->player->position.y && (App->entityManager->player->position.x - position.x) <= DETECTION_RANGE) {


			state = FallState;
		}

		break;

	

	case AttackState:

		if (AttackLeft == true) {
			SKwidth = SKwidth - 20;
			LOG("ATTACKED LEFT");
		}
		else if (AttackRight == true) {
			SKwidth = SKwidth + 20;
			LOG("ATTACKED RIGHT");
		}
		state = IdleState;
		break;

	case AttackReadyState:

		//ATTACK ANIMATION LEFT
		if (AttackLeft) {
			flip=false;
			animation = &attack;
			//if (animation->FinishedAnimation()) {

			state = AttackState;
			//}
		}
		//ATACK ANIMATION RIGHT
		else if (AttackRight) {
			flip = true;
			animation = &attack;
			//if (animation->FinishedAnimation()) {
			state = AttackState;
			//}
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

		if (B->type == ObjectType::Player) {

			if (state != AttackState || state != AttackReadyState) {
				if (A->rect.x + A->rect.w >= B->rect.x && A->rect.x <= B->rect.x)
				{
					//LOG("ATTACKING LEFT");
					AttackLeft = true;
					state = AttackReadyState;
				}

				if (A->rect.x <= B->rect.x + B->rect.w && A->rect.x + A->rect.w >= B->rect.x + B->rect.w) {
					//LOG("ATTACKING RIGHT");
					AttackRight = true;
					state = AttackReadyState;
				}

			}


		}

		if (B->type == ObjectType::LateralPlatform) {

			//Right to Left

			if (A->rect.x <= B->rect.x + B->rect.w && A->rect.x + A->rect.w >= B->rect.x + B->rect.w) {

				TouchingColliderLeft = true;
				TouchingColliderRight = false;

				//LOG("WALL TO THE LEFT");
			}

		}

		if (B->type == ObjectType::LateralPlatformLeft) {

			//LEFT TO RIGHT

			if (A->rect.x + A->rect.w >= B->rect.x && A->rect.x <= B->rect.x) {

				TouchingColliderRight = true;
				TouchingColliderLeft = false;
				//LOG("WALL TO THE RIGHT");

			}
		}
	}
}

bool j1Skeleton::InitEntity()
{
	pugi::xml_document config;
	config.load_file("config.xml");

	pugi::xml_node skeleton = config.child("config").child("entities").child("skeleton");

	SKwidth = skeleton.attribute("width").as_int();
	SKheight = skeleton.attribute("height").as_int();

	state = IdleState;

	animation = &walking;
	SKrect = { position.x, position.y, SKwidth, SKheight };
	collider = App->collision->AddCollider(SKrect, ObjectType::Skeleton, App->entityManager);

	return true;
}

void j1Skeleton::ComparePositions()
{
	if (Dead != true) {
		if (App->entityManager->player != nullptr) {

			if (((App->entityManager->player->position.x - position.x) >= DETECTION_RANGE)
				|| (App->entityManager->player->position.x - position.x) <= NEGATIVE_DETECTION_RANGE) {

				state = IdleState;
			}
			else
			{
				//SKELETON TO THE RIGHT OF THE PLAYER
				if (App->entityManager->player->position.x < position.x && (App->entityManager->player->position.x - position.x) <= DETECTION_RANGE) {

					GoLeft = true;
					GoRight = false;
					state = LeftState;
					//LOG("GOING LEFT");
				}
				//SKELETON TO THE LEFT OF THE PLAYER
				else if (App->entityManager->player->position.x > position.x && (App->entityManager->player->position.x - position.x) >= -DETECTION_RANGE) {
					GoRight = true;
					GoLeft = false;
					state = RightState;
					//LOG("GOING RIGHT");
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

void j1Skeleton::Movement()
{
	//DRETA
	if (state == RightState && TouchingColliderRight == false) {
		position.x += 2;
		//LOG("MOVEMENT DRETA");
		//TouchingColliderLeft = false;
	}
	else if (state == RightState) {
		//LOG("BLOCKED DRETA");
	}
	//ESQUERRA
	if (state == LeftState && TouchingColliderLeft == false) {
		position.x -= 2;
		//LOG("MOVEMENT ESQUERRA");
		//TouchingColliderRight = false;
	}
	else if (state == LeftState) {
		// LOG("BLOCKED ESQUERRA");
	}

}

