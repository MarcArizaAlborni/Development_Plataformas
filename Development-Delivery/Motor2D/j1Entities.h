#ifndef _J1_ENTITIES_
#define _J1_ENTITIES_

#include "p2SString.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Collision.h"
#include "p2Animation.h"



enum EntityType {
	UNKNOWN,
	PLAYER,
	FLYING_ENEMY,
	GROUND_ENEMY,
};


class j1EntityManager;
struct SDL_Texture;
struct SDL_Rect;



class j1Entity:public j1Module {

public:


	j1Entity(int x, int y, EntityType type);

	~j1Entity() {}

	 bool Start() { return true; }

	 virtual bool Update(float dt, bool DoLogic);
	 bool PostUpdate() { return true; }
	 bool CleanUp();

	 bool Load(pugi::xml_node&) { return true; }
	 bool Save(pugi::xml_node&) { return true; }

	
	virtual void OnCollision(Collider*, Collider*) {}


	j1Entity* createEntity(EntityType type, int x, int y);
	void DestroyEntity(j1Entity* entity);


public:

	//INITIALIZE VARIABLES 
	p2List<j1Entity*> entities;


	iPoint	position;
	Collider*	collider = nullptr;
	EntityType type;
	

	
	//INIT TEXTURES AND ANIMATIONS 
	SDL_Texture* texture;
			
	Animation* animation;				
};

#endif