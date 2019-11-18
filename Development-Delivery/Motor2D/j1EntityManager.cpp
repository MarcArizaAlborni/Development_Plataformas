#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Entities.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1App.h"



j1EntityManager::j1EntityManager()
{
	name.create("Entities");
	//Load XML FILE FOR ALL VARIABLES HERE
}

j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Awake(pugi::xml_node& config) {

	this->config = config;

	for (p2List_item<j1Entity*>* Entity = entities_list.start; Entity; Entity = Entity->next)
	{
		Entity->data->Awake(config.child(Entity->data->name.GetString()));
	}

	LOG("AWAKE ENTITIY MANAGER");

	return true;
}

bool j1EntityManager::Start() {

	
	LOG("START ENTITIY MANAGER");
	//Load Entity textures
	//Initialize all Entities in the entity list 
	
	return true;
}

bool j1EntityManager::PreUpdate() {

	LOG("PREUPDATE ENTITIY MANAGER");
	return true;
}

bool j1EntityManager::Update(float dt) {

	LOG("UPDATE ENTITIY MANAGER");

	return true;
}

bool j1EntityManager::PostUpdate() {

	LOG("POSTUPDATE ENTITIY MANAGER");


	return true;
}

bool j1EntityManager::CleanUp() {

	LOG("CLEAN UP ENTITIY MANAGER");

	

	
	
	return true;
}

//FOR CREATING NEW ENTITIES
j1Entity *j1EntityManager::CreateEntity(int x, int y, EntityType eType) {

	j1Entity* ret = nullptr;

	switch (eType)
	{
	case GROUND_ENEMY:
		//
		break;
	case FLYING_ENEMY:
		//
		break;
	}
	ret->type = eType;

	entities_list.add(ret);

	return ret;
}



//FOR DESTROYING A SINGLE ENTITY
void j1EntityManager::DestroyEntity(j1Entity *Entity) {

	/*p2List_item<j1Entity*>* finder = entities_list.start;
	while (finder != NULL)
	{
		if (finder->data == Entity)
		{
			if (finder->data == getPlayer())
				getPlayer()->CleanUp();
			entities_list.del(finder);
			RELEASE(finder->data);
			break;
		}
		finder = finder->next;
	}*/
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