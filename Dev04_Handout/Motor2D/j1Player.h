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
	
	enum Tranformation {
		human,
		slime
	};

	iPoint Inipos;
	float Character_vel;  
	float Gravity;
	
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	bool j1Player::Awake(pugi::xml_node& conf);

	bool Start();
	
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

	SDL_Texture*		Graphics = nullptr;
	//For printing the animation in each moment
	Animation*			CurrentAnimation;

	// ANIMATION
	Animation			idle;
};
