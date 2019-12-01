#include "p2Point.h"
#include "j1Entities.h"
#include "p2DynArray.h"

struct Animation;
enum EntitiesType;



#define DETECTION_RANGE 100
#define NEGATIVE_DETECTION_RANGE -100

class j1Skeleton : public j1Entities
{
public:
	j1Skeleton(iPoint Position, EntitiesType type);

	virtual ~j1Skeleton();

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

	SDL_Rect SKrect;
	int SKwith;
	int SKheight;

	//Animation
	Animation		idle;
	Animation		walking;
	Animation		death;
	Animation       attack;


	//PATHING
	p2DynArray<iPoint>* path;
	bool Dead;
	float gravity;
	float jumpforce_xml;
	iPoint Pathing;
	
	bool GoLeft;
	bool GoRight;
	int JumpTicks;
	iPoint LastPosition;
	bool TouchingCollider;
	//PATHING FUNCTION
//	void LookForPath();
	void j1Skeleton::Move(p2DynArray<iPoint>& path, float dt);

	void j1Skeleton::ComparePositions();
	void j1Skeleton::GroundJump();
	void j1Skeleton::Movement(bool CurrentState);
};