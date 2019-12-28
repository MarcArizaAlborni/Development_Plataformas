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
#include "j1SceneUI.h"

#include "Brofiler/Brofiler.h"

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

bool j1Skull::Start()
{
	BROFILER_CATEGORY("Skull Start();", Profiler::Color::Lavender)
	texture = App->tex->Load("Sprites/Skull.png");
	InitEntity();
	return true;
}

bool j1Skull::PreUpdate()
{
	BROFILER_CATEGORY("Skull PreUpdate();", Profiler::Color::NavajoWhite)
		if (App->scene_ui->ResetEntitiesPositionNewGame == true) {
			position.x = SkullResetPosition.x;
			position.y = SkullResetPosition.y;
			state = IdleState;
		}
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	return true;
}

bool j1Skull::Update(float dt)
{
	BROFILER_CATEGORY("Skull Update();", Profiler::Color::LightCyan)
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
			position.x -= 1.5;
			if (GoDown) {
				if (TouchingColliderPlatformOver != true) {
					position.y += 1.5;
					LOG("GOING DOWN LEFT");
				}
			}
			else if (GoUp) {
				if (TouchingColliderPlatformUnder != true) {
					position.y -= 1.5;
					LOG("GOING UP LEFT");
				}
			}
		}

		break;

	case RightState:
		TouchingColliderPlatformOver, TouchingColliderPlatformUnder = false;
		if (GoRight) {
			position.x += 1.5;
			if (GoDown) {
				if (TouchingColliderPlatformOver != true) {
					position.y += 1.5;
					LOG("GOING DOWN RIGHT");
				}

			}
			else if (GoUp) {
				if (TouchingColliderPlatformUnder != true) {
					position.y -= 1.5;
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
	BROFILER_CATEGORY("Skull PostUpdate();", Profiler::Color::YellowGreen)
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
	BROFILER_CATEGORY("Skull CleanUp();", Profiler::Color::SaddleBrown)
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
	BROFILER_CATEGORY("Skull OnCollision();", Profiler::Color::LightSalmon)
	if (A->type == ObjectType::Skull) {

		if (B->type == ObjectType::Player) {

			if (((position.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((position.y + A->rect.h) > B->rect.y)) {
				
				if (App->scene_ui->PlayerInvencibility == true) {
					App->scene_ui->HealthChanged = true;
					App->scene_ui->ResetEntitiesPositionNewGame = true;
					App->entityManager->player->position.x = 90.0f;
					App->entityManager->player->position.y = 250.0f;
					if (App->scene_ui->LifeAt4 == true) {
						LOG("4 TO 3");
						App->scene_ui->LifeAt3 = true;
						App->scene_ui->LifeAt4 = false;
						App->scene_ui->PlayerInvencibility = false;
						//App->scene_ui->HealthChanged = false;
					}
					else if (App->scene_ui->LifeAt3 == true) {
						LOG("3 TO 2");
						App->scene_ui->LifeAt2 = true;
						App->scene_ui->LifeAt3 = false;
						App->scene_ui->PlayerInvencibility = false;
						//App->scene_ui->HealthChanged = false;
					}
					else if (App->scene_ui->LifeAt2 == true) {
						LOG("2 TO 1");
						App->scene_ui->LifeAt1 = true;
						App->scene_ui->LifeAt2 = false;
						App->scene_ui->PlayerInvencibility = false;
						//App->scene_ui->HealthChanged = false;
					}
					else {
						LOG("1 TO 0");
						App->scene_ui->LifeAt0 = true;
						App->scene_ui->LifeAt1 = false;
						//App->scene_ui->HealthChanged = false;

					}
				}
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
	pugi::xml_document config;
	config.load_file("config.xml");

	pugi::xml_node skull = config.child("config").child("entities").child("skull");

	SLLwidth = skull.attribute("width").as_int();
	SLLheight = skull.attribute("height").as_int();

	animation = &idle;
	state = IdleState;

	SLLrect = { position.x, position.y, SLLwidth, SLLheight };
	SkullResetPosition.x = position.x;
	SkullResetPosition.y = position.y;
	collider = App->collision->AddCollider(SLLrect, ObjectType::Skull, App->entityManager);

	return true;
}

void j1Skull::ComparePositions()
{
	BROFILER_CATEGORY("Skull Path();", Profiler::Color::DodgerBlue)
	if (Dead != true) {
		if (App->entityManager->player != nullptr) {

			if (((App->entityManager->player->position.x - position.x) >= DETECTION_RANGE)
				|| (App->entityManager->player->position.x - position.x) <= NEGATIVE_DETECTION_RANGE) {
				
				/*if (App->audio->EnemyMusic == true) {
					Mix_HaltMusic();
					App->audio->EnemyMusic = false;
					App->audio->Area2Level1 = true;
					App->audio->Area1Level1 = false;
					App->audio->PlayMusic("audio/Music/L1_A2.ogg");
				}*/
				state = IdleState;
			}
			else {
				//TO THE RIGHT OF THE PLAYER
				/*if (App->audio->EnemyMusic != true) {
					Mix_HaltMusic();
					App->audio->EnemyMusic = true;
					App->audio->Area2Level1 = false;
					App->audio->Area1Level1 = false;
					App->audio->PlayMusic("audio/Music/EnemyMusic.ogg");
				}*/
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
