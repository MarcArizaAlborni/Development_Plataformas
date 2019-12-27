#include "p2Point.h"
#include "j1Entities.h"


#define DETECTION_RANGE 300
#define NEGATIVE_DETECTION_RANGE -300


struct Animation;
enum EntitiesType;

class j1Skull : public j1Entities
{
public:
	j1Skull(iPoint Position, EntitiesType type);

	virtual ~j1Skull();
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
	iPoint SkullResetPosition;
	//Initial properties
	bool flip = false;

	SDL_Rect SLLrect;
	int SLLwidth;
	int SLLheight;

	//Animation
	Animation		idle;
	Animation		walking;
	Animation		death;

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
	void ComparePositions();
	void Movement();
};