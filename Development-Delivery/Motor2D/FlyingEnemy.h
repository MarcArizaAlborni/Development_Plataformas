#include "j1Entities.h"
#include "p2Point.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "j1Pathfinding.h"


#define START_ATTACK_RANGE 200

struct SDL_Texture;
struct Collider;


enum EntityType;


class j1FlyingEnemy :public j1Entity {

public:

	j1FlyingEnemy(iPoint FEnemyPos, EntityType type);

	virtual ~j1FlyingEnemy();


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