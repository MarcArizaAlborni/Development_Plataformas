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
	bool U_active;
	bool I_active;
	


};
enum CurrentState 
{

	IdleState,
	JumpState,
	JumpStateLeft, 
	JumpStateRight,
	LeftState,
	RightState,
	LeftSlimeState,
	RightSlimeState,
	DoubleJumpState,
	DoubleJumpStateLeft,
	DoubleJumpStateRight,
	DashStateLeft,
	DashStateRight

};

class j1Player : public j1Module 
{
private:


	iPoint Inipos;
	float Character_vel; 
	
	
public:
	
	float Gravity;
	j1Player();

	// Destructor
	virtual ~j1Player();

	bool Awake(pugi::xml_node& conf);

	bool Start();
	float floor; //NOMES PER TESTING
	bool PreUpdate();	
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();
	//INPUT COSES
	Input_player		PlayerInput;
	CurrentState		PlayerState;
	SDL_Rect			Player_Rect;
	//ACTUALIZING POSITION 
	iPoint				CurrentPosition;
	iPoint              StartPosition;

	SDL_Texture*		Graphics = nullptr;
	//For printing the animation in each moment
	Animation*			CurrentAnimation;

	// ANIMATION
	Animation			idle;



	//JUMP

	float JumpSpeed = 2;
	bool jumping = false;
	float gravity2 = 0.2f;


	
};
