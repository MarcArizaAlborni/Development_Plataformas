#pragma once
#include "p2Animation.h"
#include "j1Module.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Point.h"


struct SDL_Texture;
struct p2Animation;

struct Input_player
{

	bool A_active;
	bool D_active;
	bool Space_active;
	/*bool E_active;
	bool Q_active;
	bool R_active;*/


};
enum CurrentState 
{

	IdleState,
	JumpState, 
	LeftState,
	RightState

};

class j1Player : public j1Module 
{
private:
	
	int floor_Level = 0; //Ni idea del valor nomes poso algo per posarho
	float speed_char = 2;  //Ni idea del valor nomes poso algo per posarho
	
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();
	
	// Called each loop iteration
	bool PreUpdate();	
	bool Update();
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//INPUT COSES
	Input_player		PlayerInput;
	CurrentState		PlayerState;
	SDL_Rect*			Player_Rect;
	//ACTUALIZING POSITION 
	iPoint				CurrentPosition;

	SDL_Texture*		Graphics = nullptr;
	//For printing the animation in each moment
	Animation*			CurrentAnimation;

	// ANIMATION
	Animation			idle;
};
