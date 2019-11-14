#ifndef _J1_ENTITIES_
#define _J1_ENTITIES_

#include "p2SString.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Collision.h"
#include "p2Animation.h"



enum EntityState {
	UNKNOWN
};
class j1EntityManager;
struct SDL_Texture;
struct SDL_Rect;

enum class ENTITY_TYPE;

class j1Entity {


public:

	

	j1Entity(iPoint pos, ENTITY_TYPE Type);

	~j1Entity() {}

	virtual bool Start() { return true; }

	virtual bool Update(float dt) { return true; }
	virtual bool PostUpdate() { return true; }
	virtual bool CleanUp();

	virtual bool Load(pugi::xml_node&) { return true; }
	virtual bool Save(pugi::xml_node&) { return true; }

	//virtual void OnCollision(Collider* c1, Collider* c2) {}
	virtual void OnCollision(Collider*, Collider*) {}

public:

	//INITIALIZE VARIABLES 

	iPoint	position;
	ENTITY_TYPE type;
	Collider*	collider = nullptr;

	

	
	//TEXUTRES INIT
	SDL_Texture* texture = nullptr;
	

};

#endif