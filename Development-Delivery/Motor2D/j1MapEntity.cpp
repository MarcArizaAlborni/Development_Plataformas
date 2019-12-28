#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entities.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1MapEntity.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"
#include "j1SceneUI.h"

#include "Brofiler/Brofiler.h"


j1MapEntity::j1MapEntity(iPoint pos, EntitiesType type) : j1Entities(pos, EntitiesType::MAP)
{
	idle.PushBack({ 11,18,45,35 });
	idle.PushBack({ 11,18,45,35 });
	idle.speed = 0.5f;
}

j1MapEntity::~j1MapEntity()
{
}

bool j1MapEntity::Start()
{
	BROFILER_CATEGORY("Map Entity Start();", Profiler::Color::Yellow)
		texture = App->tex->Load("Sprites/Map.png");
	MapCollected = false;
	InitEntity();
	return true;
}

bool j1MapEntity::PreUpdate()
{
	BROFILER_CATEGORY("Map Entity PreUpdate();", Profiler::Color::FireBrick)
	if (App->scene_ui->ResetEntitiesPositionNewGame == true) {
		position.x = MapResetPosition.x;
		position.y = MapResetPosition.y;
		
	}
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	return true;
}

bool j1MapEntity::Update(float dt)
{

	
	
	animation = &idle;

	return true;
}

bool j1MapEntity::PostUpdate()
{
	BROFILER_CATEGORY("Map Entity PostUpdate();", Profiler::Color::SaddleBrown)
		MapEntityRect.x = position.x;
	MapEntityRect.y = position.y;

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

bool j1MapEntity::CleanUp()
{
	BROFILER_CATEGORY("Map Entity CleanUp();", Profiler::Color::AliceBlue)
		App->tex->UnLoad(texture);

	return true;
}

bool j1MapEntity::Load(pugi::xml_node &node)
{
	return true;
}

bool j1MapEntity::Save(pugi::xml_node &node) const
{
	return true;
}

void j1MapEntity::OnCollision(Collider* A, Collider* B)
{
	BROFILER_CATEGORY("Map Entity OnCollision();", Profiler::Color::CornflowerBlue)
		if (A->type == ObjectType::MapEntityCollider) {

			if (B->type == ObjectType::Player && MapCollected==false) {

				if (((position.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((position.y + A->rect.h) > B->rect.y)) {
					//+1 TO MAP COUNT
					position.x = -100;
					position.y = 100;
					App->entityManager->player->Maps_Num = App->entityManager->player->Maps_Num + 1;
					MapCollected = true;
					LOG("MAP +1");
					
				}
			}

			

			


		}

}

bool j1MapEntity::InitEntity()
{
	pugi::xml_document config;
	config.load_file("config.xml");

	pugi::xml_node MapEntity = config.child("config").child("entities").child("MapEntity");

	MapEntitywidth = MapEntity.attribute("width").as_int();
	MapEntityheight = MapEntity.attribute("height").as_int();

	animation = &idle;
	state = IdleState;

	MapEntityRect = { position.x, position.y, MapEntitywidth, MapEntityheight };
	MapResetPosition.x = position.x;
	MapResetPosition.y = position.y;
	collider = App->collision->AddCollider(MapEntityRect, ObjectType::MapEntityCollider, App->entityManager);

	return true;
}

