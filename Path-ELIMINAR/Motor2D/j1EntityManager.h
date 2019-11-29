#ifndef _J1_ENTITY_MANAGER
#define _J1_ENTITY_MANAGER

#include "j1Module.h"
#include "j1Entities.h"
#include "p2List.h"




class j1EntityManager : public j1Module
{
public:

	j1EntityManager();

	~j1EntityManager();


	//FUNCTION ROTATION IN ENTITY MANAGER
	bool Awake(pugi::xml_node&);
	bool Start();				//Load textures for each entity
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	
	p2List<j1Entity*> entities_list;

	//FUNCTIONS RELATED TO ENTITIES
	j1Entity* CreateEntity(EntityType type, int x = 0, int y = 0);
	void DestroyAllEntities();
	void DestroyEntity(j1Entity *Entity);

	j1Entity* getPlayer() const;

	//ENTITIES
	j1Player* player; //Player (Entity)

	pugi::xml_node	config;
	bool DoLogic;			//Bool to see if logic from an entity has to be done
	float Accumulated_dt;	    //dt accomulated amount 
	float Cycle_Pause_Time;	//Time between cycle 


	void CleanEntities();

public:

	

	

	//XML DOCUMENT OPEN
	//CREATE LOAD TEXTURES FOR RENTITIES
	
	
};

#endif