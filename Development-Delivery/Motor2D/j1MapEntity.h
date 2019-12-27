#include "p2Point.h"
#include "j1Entities.h"





struct Animation;
enum EntitiesType;

class j1MapEntity : public j1Entities
{
public:
	j1MapEntity(iPoint Position, EntitiesType type);

	virtual ~j1MapEntity();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node &node); //Load 
	bool Save(pugi::xml_node &node) const;//Save
	// Collision handling -----
	void OnCollision(Collider* A, Collider* B);

	bool InitEntity();
	iPoint MapResetPosition;
	//Initial properties
	bool flip = false;

	SDL_Rect MapEntityRect;
	int MapEntitywidth;
	int MapEntityheight;

	//Animation
	Animation		idle;
	//PATHFINDING BOOLS
	bool Dead;
	bool GoLeft;
	bool GoRight;
	bool GoUp;
	bool GoDown;
	bool TouchingColliderPlatformUnder;
	bool TouchingColliderPlatformOver;
	bool TouchingColliderLeft;
	bool TouchingColliderRight;
	//PATHFINDING FUNCTIONS

	//MAP COLLECION

	bool MapCollected;
};