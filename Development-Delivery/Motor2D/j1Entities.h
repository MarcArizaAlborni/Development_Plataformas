#ifndef _J1_ENTITIES_
#define _J1_ENTITIES_

#include "p2SString.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Collision.h"
#include "SDL_mixer/include/SDL_mixer.h"


enum EntityStates {
	
	UNKNOWN_STATE,
};
class j1EntityManager;
struct SDL_Texture;
struct SDL_Rect;

enum class ENTITY_TYPE;

class j1Entities {


public:

	//j1Entites(iPoint pos, ENTITY_TYPE Type);

	//~j1Entiies() {}

	virtual bool Start() { return true; }

	virtual bool Update(float dt) { return true; }
	virtual bool PostUpdate() { return true; }
	virtual bool CleanUp();

	virtual bool Load(pugi::xml_node&) { return true; }
	virtual bool Save(pugi::xml_node&) { return true; }

	
	virtual void OnCollision(Collider*, Collider*) {}

public:
	
	//Entity Variables
	iPoint		position;
	iPoint EntityVel;
	ENTITY_TYPE type;
	Collider*	collider = nullptr;

	
	
	
	

	

	






	

};

#endif