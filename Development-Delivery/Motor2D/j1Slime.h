#include "p2Point.h"
#include "j1Entities.h"
#include "p2DynArray.h"

#define DETECTION_RANGE 150
#define NEGATIVE_DETECTION_RANGE -150


struct Animation;
enum EntitiesType;

class j1Slime : public j1Entities
{
public:
	j1Slime(iPoint Position, EntitiesType type);

	virtual ~j1Slime();

	bool Awake(pugi::xml_node& conf);
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

	SDL_Rect SLMrect;
	int SLMwith;
	int SLMheight;

	//Animation
	Animation		idle;
	Animation		walking;
	Animation		death;
	Animation		attack;
	




	//PATHING
	//p2DynArray<iPoint>* path;
	bool Dead;
	float gravity;
	float jumpforce_xml;
	iPoint Pathing;

	bool GoLeft;
	bool GoRight;
	iPoint LastPosition;
	bool TouchingColliderLeft; //PER L' ESQUERRA TOCA COLLIDER
	bool TouchingColliderRight;//PERLA DRETA TOCA COLLIDER


	bool AttackLeft;
	bool AttackRight;

	//PATHING FUNCTION
//	void LookForPath();
	//void j1Skeleton::Move(p2DynArray<iPoint>& path, float dt);

	void j1Slime::ComparePositions();
	void j1Slime::GroundJump();
	void j1Slime::Movement();
};