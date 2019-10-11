#pragma once

#include "j1Module.h"

class j1Collision : public j1Module
{
public:

	j1Collision();

	// Destructor
	virtual ~j1Collision();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update();

	// Called before quitting
	bool CleanUp();

};