#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Entities.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1App.h"



j1EntityManager::j1EntityManager():player(nullptr)
{
	name.create("Entities");
	//Load XML FILE FOR ALL VARIABLES HERE
}

j1EntityManager::~j1EntityManager() {

	delete player;
}

bool j1EntityManager::Awake(pugi::xml_node& config) {

	this->config = config;

	for (p2List_item<j1Entity*>* EntitySelect = entities_list.start; EntitySelect; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->Awake(config.child(EntitySelect->data->name.GetString()));
	}

	//LOG("AWAKE ENTITIY MANAGER");

	return true;
}

bool j1EntityManager::Start() {

	
	//LOG("START ENTITIY MANAGER");
	
	//Initialize all Entities in the entity list 
	
	for (p2List_item<j1Entity*>* EntitySelect = entities_list.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->Start();
	}

	return true;
}

bool j1EntityManager::PreUpdate() {

	for (p2List_item<j1Entity*>* EntitySelect = entities_list.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->PreUpdate();
	}

	//LOG("PREUPDATE ENTITIY MANAGER");
	return true;
}

bool j1EntityManager::Update(float dt) {


	Accumulated_dt += dt;

	if (Accumulated_dt >= Cycle_Pause_Time) 
	{
		DoLogic = true;
	}

	for (p2List_item<j1Entity*>* EntitySelect = entities_list.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->Update(dt, DoLogic);
	}

	if (DoLogic == true)				
	{
		DoLogic = false;
		Accumulated_dt = 0;
	}

	//LOG("UPDATE ENTITIY MANAGER");

	return true;
}

bool j1EntityManager::PostUpdate() {


	for (p2List_item<j1Entity*>* EntitySelect = entities_list.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->PostUpdate();
	}

	//LOG("POSTUPDATE ENTITIY MANAGER");


	return true;
}

bool j1EntityManager::CleanUp() {

	//LOG("CLEAN UP ENTITIY MANAGER");

	
	for (p2List_item<j1Entity*>* EntitySelect = entities_list.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		EntitySelect->data->CleanUp();
	}

	entities_list.clear();	//Clear List								

	player = NULL;
	
	
	return true;
}

//FOR CREATING NEW ENTITIES
j1Entity* j1EntityManager::CreateEntity(EntityType type, int x, int y)
{
	//static_assert?

	j1Entity* ret = nullptr;

	switch (type)
	{
	case EntityType::PLAYER:							
				
		//ret = new j1Player(x, y, type);

		
		break;

	case EntityType::GROUND_ENEMY:							
		
		break;
	case EntityType::FLYING_ENEMY:							
		
		break;
	}
	//ret->type = type;

	if (ret != nullptr)								
	{
		entities_list.add(ret);								
	}

	return ret;
}



//FOR DESTROYING A SINGLE ENTITY
void j1EntityManager::DestroyEntity(j1Entity *Entity) {

	for (p2List_item<j1Entity*>* EntitySelect = entities_list.start; EntitySelect != NULL; EntitySelect = EntitySelect->next)
	{
		if (EntitySelect->data == Entity)
		{
			if (EntitySelect->data == getPlayer())		
			{
				getPlayer()->CleanUp();
			}

			entities_list.del(EntitySelect);
			RELEASE(EntitySelect->data);
			break;
		}
	}
}



//CLEANUP ALL ENTITIES (CLEANUP STATE)
void j1EntityManager::CleanEntities()
{
	p2List_item<j1Entity*>* item;
	for (item = entities_list.start; item != nullptr; item = item->next)
	{
		if (item->data->collider != nullptr)
			item->data->collider->to_delete = true;

		item->data->CleanUp();
	}

}



j1Entity* j1EntityManager::getPlayer() const
{
	j1Entity* ret = nullptr;

	for (p2List_item<j1Entity*>* entity = entities_list.start; entity; entity = entity->next)
	{
		if (entity->data->type == EntityType::PLAYER)
		{
			ret = entity->data;
			break;
		}
	}

	return ret;
}