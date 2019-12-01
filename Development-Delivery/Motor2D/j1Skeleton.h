#include "p2Point.h"
#include "j1Entities.h"
#include "p2DynArray.h"

struct Animation;
enum EntitiesType;



#define DETECTION_RANGE 150
#define NEGATIVE_DETECTION_RANGE -150


class j1Skeleton : public j1Entities
{
public:
	j1Skeleton(iPoint Position, EntitiesType type);

	virtual ~j1Skeleton();

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

	//Initial properties
	bool flip = false;
	SDL_Rect SKrect;
	int SKwidth;
	int SKheight;

	//Animation
	Animation		idle;
	Animation		walking;
	Animation		death;
	Animation       attack;
	Animation       hit;
	Animation       RIP;
	bool DieOnce = 0;
	//PATHING
	p2DynArray<iPoint>* path;
	bool Dead;
	float jumpforce_xml;
	iPoint Pathing;

	bool GoLeft;
	bool GoRight;
	iPoint LastPosition;
	bool TouchingColliderLeft; //PER L' ESQUERRA TOCA COLLIDER
	bool TouchingColliderRight;//PERLA DRETA TOCA COLLIDER

	bool AttackLeft;
	bool AttackRight;

	void ComparePositions();
	void Movement();
};