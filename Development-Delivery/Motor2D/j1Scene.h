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
	bool Awake(pugi::xml_node&);

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

	// Save Current Map
	bool Save(pugi::xml_node& data) const;

	//Spawn Points
	iPoint Skeleton1;
	iPoint Skeleton2;
	iPoint Skeleton3;
	iPoint Skull1;
	iPoint Bee1;
	iPoint MapItem1;
	iPoint MapItem2;
	iPoint MapItem3;
	iPoint MapItem4;

	void StartMap1();
	void StartMap2();
	/*void StartMainMenu();*/

	void RestartLevelEntitiesL1();

	//FOR UI

	bool Map1Loaded;
	bool Map2Loaded;
	bool MainMenuLoaded;

	bool IngameMenuONb;
	bool IngameMenuOFFb;

	bool LoadSettingsPlease;
private:

	SDL_Texture* debug_tex;

	bool debug_path;
};

#endif // __j1SCENE_H__