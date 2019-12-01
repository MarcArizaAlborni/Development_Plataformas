#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entities.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Slime.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Pathfinding.h"


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

	attack.PushBack({ 0,71,49,34 });
	attack.PushBack({ 49,71,49,34 });
	attack.PushBack({ 98,71,49,34 });
	attack.PushBack({ 147,71,49,34 });
	attack.PushBack({ 196,71,49,34 });
	attack.PushBack({ 245,71,49,34 });
	attack.PushBack({ 294,71,49,34 });
	attack.PushBack({ 343,71,49,34 });
	attack.speed = 0.2f;

}

j1Slime::~j1Slime()
{
}

bool j1Slime::Start()
{
	texture = App->tex->Load("Sprites/Slime.png");
	InitEntity();
	return true;
}

bool j1Slime::PreUpdate()
{
	//position.y += gravity;
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	return true;
}

bool j1Slime::Update(float dt)
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
			SLMwidth = SLMwidth - 20;
			LOG("ATTACKED LEFT");
		}
		else if (AttackRight == true) {
			SLMwidth = SLMwidth + 20;
			LOG("ATTACKED RIGHT");
		}
		state = IdleState;
		break;

	case AttackReadyState:

		//ATTACK ANIMATION LEFT
		if (AttackLeft) {
			flip = true;
			animation = &attack;
			//if (animation->FinishedAnimation()) {

			state = AttackState;
			//}
		}
		//ATACK ANIMATION RIGHT
		else if (AttackRight) {
			animation = &attack;
			//if (animation->FinishedAnimation()) {
			state = AttackState;
			//}
		}
		break;

	}


	return true;
}

bool j1Slime::PostUpdate()
{


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

	if (A->type == ObjectType::Slime) {

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

bool j1Slime::InitEntity()
{

	SLMwidth = 21;
	SLMheight = 35;

	state = IdleState;

	animation = &walking;
	SLMrect = { position.x, position.y, SLMwidth, SLMheight };
	collider = App->collision->AddCollider(SLMrect, ObjectType::Slime, App->entityManager);

	return true;
}

void j1Slime::ComparePositions()
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

void j1Slime::Movement()
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

