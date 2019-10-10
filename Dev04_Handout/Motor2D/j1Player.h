#pragma once

#include "j1Module.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Point.h"


enum CurrentState {

	idle,
	jump, 
	move_left,
	move_right




};
class j1Player : public j1Module {
private:
	
	int floor_Level = 0; //Ni idea del valor nomes poso algo per posarho
	float speed_char = 2;  //Ni idea del valor nomes poso algo per posarho
	SDL_Rect Player_Rect;
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	bool Init();

	bool Start();

	

	bool PreUpdate();
	
	bool Update();

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


};
