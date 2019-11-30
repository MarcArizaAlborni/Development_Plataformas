#ifndef _J1ENTITYMANAGER_
#define _J1ENTITYMANAGER_

#include "j1Module.h"
#include "j1Entities.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Entities.h"

class j1Entities;
class j1Player;
class j1Skeleton;

struct SDL_Texture;

enum EntitiesState
{
	IdleState,
	JumpState,
	LeftState,
	RightState,
	DashState,
	FallState,
	DoubleJumpState,
	DeadState,
	None
};

enum EntitiesType
{
	NOTHING,
	PLAYER,
	SKELETON,
	FLYING
};

struct EntitiesInfo
{
	EntitiesType type = EntitiesType::NOTHING;
	p2Point<float> position;
};


class j1EntityManager : public j1Module
{
public:

	j1EntityManager();

	~j1EntityManager();

	//FUNCTIONS IN ENTITY MANAGER
	bool Awake(pugi::xml_node&);
	bool Start();				//Load textures for each entity
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entities* CreateEntities(EntitiesType type, iPoint pos = { 0,0 });
	void CreateEntity(EntitiesType type, iPoint pos = { 0,0 });
	void AddEnemies(iPoint pos, EntitiesType type);
	void SpawnEnemies(const EntitiesInfo& info);
	void DeleteEntities(j1Entities* entity);
	void CleanEntities();

	void OnCollision(Collider* c1, Collider* c2);

	bool InitEntity();

	//
	p2List<j1Entities*>	entityList;
	j1Player*			player = nullptr;
	j1Skeleton*			skeleton = nullptr;

private:

	EntitiesInfo	entities[200];		//NUMERO HARCODED HA DE POSSARSE AL XML
};

#endif //_J1ENTITYMANAGER_