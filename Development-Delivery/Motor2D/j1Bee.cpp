#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entities.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Bee.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"
#include "j1SceneUI.h"

#include "Brofiler/Brofiler.h"


j1Bee::j1Bee(iPoint pos, EntitiesType type) : j1Entities(pos, EntitiesType::BEE)
{
	idle.PushBack({ 0,0,36,32 });
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

bool j1Bee::Start()
{
	BROFILER_CATEGORY("Bee Start();", Profiler::Color::Brown)
	texture = App->tex->Load("Sprites/Bee.png");
	InitEntity();
	return true;
}

bool j1Bee::PreUpdate()
{
	if (App->scene_ui->ResetEntitiesPositionNewGame == true) {
		position.x = BeeResetPosition.x;
		position.y = BeeResetPosition.y;
		state = IdleState;
	}
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	return true;
}

bool j1Bee::Update(float dt)
{
	BROFILER_CATEGORY("Bee Update();", Profiler::Color::DodgerBlue)
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
					//LOG("GOING DOWN LEFT");
				}
			}
			else if (GoUp) {
				if (TouchingColliderPlatformUnder != true) {
					position.y -= 3;
					//LOG("GOING UP LEFT");
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

bool j1Bee::PostUpdate()
{
	BROFILER_CATEGORY("Bee PostUpdate();", Profiler::Color::SaddleBrown)
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

bool j1Bee::CleanUp()
{
	BROFILER_CATEGORY("Bee CleanUp();", Profiler::Color::FireBrick)
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
	BROFILER_CATEGORY("Bee OnCollision();", Profiler::Color::CornflowerBlue)
	if (A->type == ObjectType::Bee) {

		if (B->type == ObjectType::Player) {

			if (((position.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((position.y + A->rect.h) > B->rect.y)) {
				//App->fade->FadeToBlack("SimpleLevel1.tmx");
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
					position.y = B->rect.y - A->rect.h ;
					TouchingColliderPlatformOver = true;

				}
			}


		}


	}

}

bool j1Bee::InitEntity()
{
	pugi::xml_document config;
	config.load_file("config.xml");

	pugi::xml_node bee = config.child("config").child("entities").child("bee");

	Beewidth = bee.attribute("width").as_int();
	Beeheight = bee.attribute("height").as_int();

	animation = &idle;
	state = IdleState;

	Beerect = { position.x, position.y, Beewidth, Beeheight };
	BeeResetPosition.x = position.x;
	BeeResetPosition.y = position.y;
	collider = App->collision->AddCollider(Beerect, ObjectType::Bee, App->entityManager);

	return true;
}

void j1Bee::ComparePositions()
{
	BROFILER_CATEGORY("Bee Path();", Profiler::Color::Pink)
	if (Dead != true) {
		if (App->entityManager->player != nullptr) {

			if (((App->entityManager->player->position.x - position.x) >= DETECTION_RANGE)
				|| (App->entityManager->player->position.x - position.x) <= NEGATIVE_DETECTION_RANGE) {
				
				if (App->audio->EnemyMusic == true) {
					Mix_HaltMusic();
					App->audio->EnemyMusic = false;
					App->audio->Area2Level1 = true;
					App->audio->Area1Level1 = false;
					App->audio->PlayMusic("audio/Music/L1_A2.ogg");
				}
				
				state = IdleState;
			}
			else {
				
				if (App->audio->EnemyMusic != true) {
					Mix_HaltMusic();
					App->audio->EnemyMusic = true;
					App->audio->Area2Level1 = false;
					App->audio->Area1Level1 = false;
					App->audio->PlayMusic("audio/Music/EnemyMusic.ogg");
				}
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

void j1Bee::Movement()
{
}
