#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Skeleton.h"
#include "p2Log.h"

#include "Brofiler/Brofiler.h"


j1EntityManager::j1EntityManager()
{
	name.create("entities");
	//Load XML FILE FOR ALL VARIABLES HERE
}

j1EntityManager::~j1EntityManager() 
{	
}

bool j1EntityManager::Awake(pugi::xml_node& config) 
{
	for (p2List_item<j1Entities*>* EntitySelect = entityList.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->Awake(config);
	}
	return true;
}

bool j1EntityManager::Start() 
{
	BROFILER_CATEGORY("EntityManager_Start", Profiler::Color::Gainsboro)

	for (p2List_item<j1Entities*>* EntitySelect = entityList.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->Start();
	}

	return true;
}

bool j1EntityManager::PreUpdate()
{
	//BROFILER_CATEGORY
	for (p2List_item<j1Entities*>* EntitySelect = entityList.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->PreUpdate();
	}

	// i < NUMBER OF MAXIM ENTITIES
	for (int i = 0; i < 200; ++i)
	{
		if (entities[i].type != EntitiesType::NOTHING)
		{
			if (entities[i].type == EntitiesType::SKELETON)
				SpawnEnemies(entities[i]);

			else
				SpawnEnemies(entities[i]);

			entities[i].type = EntitiesType::NOTHING;
		}
	}

	LOG("PREUPDATE ENTITIY MANAGER");
	return true;
}

bool j1EntityManager::Update(float dt) 
{
	//BROFILER_CATEGORY

	for (p2List_item<j1Entities*>* EntitySelect = entityList.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->Update(dt);
	}

	//LOG("UPDATE ENTITIY MANAGER");

	return true;
}

bool j1EntityManager::PostUpdate() 
{
	//BROFILER_CATEGORY

	for (p2List_item<j1Entities*>* EntitySelect = entityList.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->PostUpdate();
	}

	//LOG("POSTUPDATE ENTITIY MANAGER");


	return true;
}

bool j1EntityManager::CleanUp() {

	//LOG("CLEAN UP ENTITIY MANAGER");

	for (p2List_item<j1Entities*>* EntitySelect = entityList.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->CleanUp();
	}

	entityList.clear();	//Clear List								
	player = nullptr;
	
	return true;
}

bool j1EntityManager::Load(pugi::xml_node &)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node &) const
{
	return true;
}


//FOR CREATING NEW ENTITIES
j1Entities* j1EntityManager::CreateEntities(EntitiesType type, iPoint pos)
{
	j1Entities* ret = nullptr;

	switch (type)
	{
	case EntitiesType::PLAYER:
		ret = new j1Player(pos, type);

		if (ret != nullptr)
		{
			entityList.add(ret);
		}
		break;

	case EntitiesType::SKELETON:
		ret = new j1Skeleton(pos, type);
		if (ret != nullptr)
		{
			entityList.add(ret);
		}
		break;	
	}

	return ret;
}

void j1EntityManager::CreateEntity(iPoint pos, EntitiesType type)
{
	switch (type)
	{
	case EntitiesType::PLAYER:
		player = (j1Player*)CreateEntities(EntitiesType::PLAYER, pos);
		break;

	case EntitiesType::SKELETON:
		skeleton = (j1Skeleton*)CreateEntities(EntitiesType::SKELETON, pos);
		break;
	}
}

void j1EntityManager::AddEnemies(iPoint pos, EntitiesType type)
{
	for (int i = 0; i < 200; ++i)
	{
		if (entities[i].type == EntitiesType::NOTHING)
		{
			entities[i].type = type;
			entities[i].position.x = pos.x;
			entities[i].position.y = pos.y;
			break;
		}
	}
}

void j1EntityManager::SpawnEnemies(const EntitiesInfo& info)
{
	for (uint i = 0; i < 200; ++i)  //NUMERO HARCODED HA DE POSSARSE AL XML
	{
		if (entities[i].type != EntitiesType::NOTHING)
		{
			j1Entities* ret = nullptr;

			switch (info.type) {

			case EntitiesType::SKELETON:

				if (ret != nullptr)
					entityList.add(ret);
				break;
			}

			ret->Start();

			break;
		}
	}
}

//FOR DESTROYING A SINGLE ENTITY
void j1EntityManager::DeleteEntities(j1Entities* entity) {


}



//CLEANUP ALL ENTITIES (CLEANUP STATE)
void j1EntityManager::CleanEntities()
{
	for (p2List_item<j1Entities*>* EntitySelect = entityList.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		if (EntitySelect->data != player)
		{
			EntitySelect->data->CleanUp();
			entityList.del(EntitySelect);
			RELEASE(EntitySelect->data);
		}
	}

}

void j1EntityManager::OnCollision(Collider * c1, Collider * c2)
{
	for (p2List_item<j1Entities*>* EntitySelect = entityList.start; EntitySelect != nullptr; EntitySelect = EntitySelect->next)
	{
		if (EntitySelect->data->collider == c1)
		{
			EntitySelect->data->OnCollision(c1, c2);
			EntitySelect->data->OnCollision(c2, c1);
			break;
		}
	}
}

bool j1EntityManager::InitEntity()
{

	for (p2List_item<j1Entities*>* EntitySelect = entityList.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->InitEntity();
	}

	return true;
}