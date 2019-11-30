#ifndef _J1ENTITIES_
#define _J1ENTITIES_

#include "p2SString.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Collision.h"
#include "p2Animation.h"
#include "j1EntityManager.h"


struct SDL_Texture;
struct SDL_Rect;
struct Collider;

enum EntitiesType;
enum EntitiesState;

class j1Entities
{
public:

	j1Entities(iPoint Position, EntitiesType type);

	~j1Entities() {}

	virtual bool Awake(pugi::xml_node& config);
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

	virtual void BlitEntities(SDL_Rect r, bool flip = false, float x = 0, float y = 0);

	virtual void OnCollision(Collider* c1, Collider* c2) {};

	virtual bool InitEntity();

public:

	iPoint			position;				//Entity Position
	EntitiesType	type;					//Entity Type
	EntitiesState	state;					//Entity State

	Collider*		collider = nullptr;		//Entity Collider
	SDL_Texture*	texture = nullptr;		//Entity Texture
	Animation*		animation = nullptr;	//Entity Animation
};

#endif _J1ENTITIES_