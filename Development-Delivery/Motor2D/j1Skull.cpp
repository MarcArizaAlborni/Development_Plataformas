#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entities.h"
#include "j1App.h"
#include "j1Textures.h"
#include"j1Skull.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"


j1Skull::j1Skull(iPoint pos, EntitiesType type) : j1Entities(pos, EntitiesType::SKULL)
{
	idle.PushBack({ 0,0,40,40 });
	idle.PushBack({ 40,0,40,40 });
	idle.PushBack({ 80,0,40,40 });
	idle.PushBack({ 120,0,40,40 });
	idle.PushBack({ 0,40,40,40 });
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
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	return true;
}

bool j1Skull::Update(float dt)
{

	if (!Dead) {
		ComparePositions();
	}
	switch (state)
	{
	case IdleState:

		animation = &idle;

		break;

	case DeadState:
		animation = &death;
		break;

	case LeftState:
		TouchingColliderPlatformOver = false, TouchingColliderPlatformUnder = false;
		if (GoLeft) {
			position.x -= 3;
			if (GoDown) {
				if (TouchingColliderPlatformOver != true) {
					position.y += 3;
					LOG("GOING DOWN LEFT");
				}
			}
			else if (GoUp) {
				if (TouchingColliderPlatformUnder != true) {
					position.y -= 3;
					LOG("GOING UP LEFT");
				}
			}
		}

		break;

	case RightState:
		TouchingColliderPlatformOver, TouchingColliderPlatformUnder = false;
		if (GoRight) {
			position.x += 3;
			if (GoDown) {
				if (TouchingColliderPlatformOver != true) {
					position.y += 3;
					LOG("GOING DOWN RIGHT");
				}

			}
			else if (GoUp) {
				if (TouchingColliderPlatformUnder != true) {
					position.y -= 3;
					LOG("GOING UP RIGHT");
				}
			}
		}
		break;

	}


	return true;
}

bool j1Skull::PostUpdate()
{
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
	if (A->type == ObjectType::Skull) {

		if (B->type == ObjectType::Player) {

			if (((position.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((position.y + A->rect.h) > B->rect.y)) {
				App->entityManager->player->position.x = App->entityManager->player->Inipos.x;
				App->entityManager->player->position.y = App->entityManager->player->Inipos.y;
			}
		}

		if (B->type == ObjectType::Platform) {

			//FROM BELOW
			if (position.y > (B->rect.y + B->rect.h - 1))
			{
				LOG("BEE TOUCHING PLATFORM FROM BELOW");
				position.y = B->rect.y + B->rect.h;
			}
			//from above
			if ((position.y + A->rect.h) <= B->rect.y + 20) // from above
			{

				if ((A->rect.x + A->rect.w > B->rect.x) || (A->rect.x + A->rect.w < B->rect.x + B->rect.w)) {
					LOG("BEE TOUCHING PLATFORM FROM ABOVE");
					position.y = B->rect.y - A->rect.h - 1;
					TouchingColliderPlatformOver = true;

				}
			}


		}


	}

}

bool j1Skull::InitEntity()
{

	SLLwidth = 32;
	SLLheight = 32;

	animation = &idle;
	state = IdleState;

	SLLrect = { position.x, position.y, SLLwidth, SLLheight };
	collider = App->collision->AddCollider(SLLrect, ObjectType::Skull, App->entityManager);

	return true;
}

void j1Skull::ComparePositions()
{
	if (Dead != true) {
		if (App->entityManager->player != nullptr) {

			if (((App->entityManager->player->position.x - position.x) >= DETECTION_RANGE)
				|| (App->entityManager->player->position.x - position.x) <= NEGATIVE_DETECTION_RANGE) {

				state = IdleState;
			}
			else {
				//TO THE RIGHT OF THE PLAYER
				if (App->entityManager->player->position.x < position.x && (App->entityManager->player->position.x - position.x) <= DETECTION_RANGE) {
					//ON TOP OF THE PLAYER
					if (App->entityManager->player->position.y > position.y && (App->entityManager->player->position.y - position.y) <= DETECTION_RANGE) {
						//LOG("BEE TO THE RIGHT AND ON TOP OF THE PLAYER");
						GoLeft = true;
						GoDown = true;
						GoRight = false;
						GoUp = false;
						state = LeftState;
					}
					else {
						//LOG("BEE TO THE RIGHT AND UNDER  THE PLAYER");
						GoLeft = true;
						GoDown = false;
						GoRight = false;
						GoUp = true;
						state = LeftState;
					}
				}

				//TO THE LEFT OF THE PLAYER
				else if (App->entityManager->player->position.x > position.x && (App->entityManager->player->position.x - position.x) >= -DETECTION_RANGE) {

					if (App->entityManager->player->position.y > position.y && (App->entityManager->player->position.y - position.y) <= DETECTION_RANGE) {
						//LOG("BEE TO THE LEFT AND ON TOP OF THE PLAYER");
						GoLeft = false;
						GoDown = true;
						GoRight = true;
						GoUp = false;
						state = RightState;
					}
					else {
						//LOG("BEE TO THE LEFT AND UNDER  THE PLAYER");
						GoLeft = false;
						GoDown = false;
						GoRight = true;
						GoUp = true;
						state = RightState;
					}
				}
			}
		}
	}
}

void j1Skull::Movement()
{
}
