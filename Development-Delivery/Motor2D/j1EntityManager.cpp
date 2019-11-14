#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Entities.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Player.h"




j1EntityManager::j1EntityManager()
{
	name.create("entityManager");

	//Load XML FILE FOR ALL VARIABLES HERE
	

}

j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Awake() {

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
j1Entity *j1EntityManager::CreateEntity(int x, int y, ENTITY_TYPE eType) {

	static_assert(ENTITY_TYPE::UNKNOWN == ENTITY_TYPE(2), "UPDATE ENTITY TYPES");

	j1Entity* Entity = nullptr;

	switch (eType) {

	
	default:
		break;
	}

	entities_list.add(Entity);
	Entity->Start();
	return Entity;
}



//FOR DESTROYING A SINGLE ENTITY
void j1EntityManager::DestroyEntity(j1Entity *Entity) {

	p2List_item<j1Entity*>*item = entities_list.start;

	while (item != nullptr) {

		if (item->data == Entity) {

			entities_list.del(item);
			RELEASE(item->data);
			break;
		}

		item = item->next;
	}
}

//FOR REMOVING ALL ENTITIES
void j1EntityManager::DestroyAllEntities() {
	p2List_item<j1Entity*>* item;
	item = entities_list.start;

	while (item != nullptr) {
		if (item->data->type != ENTITY_TYPE::PLAYER) {
			RELEASE(item->data);
			item = item->next;
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