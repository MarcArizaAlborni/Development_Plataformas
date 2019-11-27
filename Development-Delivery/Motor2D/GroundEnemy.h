#include "j1Entities.h"
#include "p2Point.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "j1Pathfinding.h"

#define START_ATTACK_RANGE 150

struct SDL_Texture;
struct Collider;



enum EntityType;


class j1GroundEnemy :public j1Entity {

public:

	j1GroundEnemy(iPoint FEnemyPos, EntityType type);

	virtual ~j1GroundEnemy();


	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* A, Collider* B);

	void Moving(p2DynArray<iPoint>& path, float dt);


public:


	Animation IdleA;
	Animation MoveA;
	Animation AttackA;
	Animation DieA;

	p2DynArray<iPoint>* path;
};