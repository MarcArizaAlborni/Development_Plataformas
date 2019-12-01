#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Load Map
	bool Load(pugi::xml_node& data);

	// Save Current Map
	bool Save(pugi::xml_node& data) const;

private:

	SDL_Texture* debug_tex;

	bool debug_path;

public:

	//Load, Respawn Enemies and Change maps

	void LoadNewLevel();

	void LoadLevel1();
	void LoadLevel2();

	void SpawnLevel1Entities();
	void SpawnLevel2Entities();

	void ReSpawnEntities();

	bool Level1 = true;
	bool Level2 = false;

};

#endif // __j1SCENE_H__