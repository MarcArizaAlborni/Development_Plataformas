#ifndef _J1_ENTITY_MANAGER
#define _J1_ENTITY_MANAGER

#include "j1Module.h"
#include "j1Entities.h"
#include "p2List.h"



enum class ENTITY_TYPE {

	ENEMY,
	PLAYER,
	WIN,
	COIN,
	UNKNOWN = 2

};


class j1EntityManager : public j1Module
{
public:

	j1EntityManager();

	~j1EntityManager();

	bool Awake();
	bool Start();				//Load textures for each entity

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	j1Entity *CreateEntity(int x, int y, ENTITY_TYPE eType);
	void DestroyEntity(j1Entity *Entity);
	p2List<j1Entity*> entities_list;
	void DestroyAllEntities();
	float accumulated_time;
	float update_ms_cycle;

	void CleanEntities();

public:

	

	

	//XML DOCUMENT OPEN
	//CREATE LOAD TEXTURES FOR RENTITIES
	SDL_Texture* player_texture;
	SDL_Texture* worm_texture;
	SDL_Texture* tribale_texture;
	SDL_Texture* debug_texture;
	SDL_Texture* Win_Texture;
	SDL_Texture* Coin_Texture;
};

#endif