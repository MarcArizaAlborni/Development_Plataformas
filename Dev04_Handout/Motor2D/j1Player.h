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

	bool A_active; //LEFT
	bool D_active;//RIGHT
	bool Space_active;// ABILITY 1
	bool U_active;//DASH ABILITY 2
	bool I_active;//TRANSOFMRATION ABILITY 3
	bool W_active; //JUMP
	


};
enum CurrentState 
{

	IdleState,
	JumpState,
	LeftState,
	RightState,
	SlimeState,
	DashState,
	

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
	
	
	//DASH THINGS
	float DashDist;
	bool DashActiveLeft;
	bool DashActiveRight;
	
	void DashFunction(){
		

		if (StartPosition.x - DashDist < CurrentPosition.x && DashActiveLeft== true) {

			CurrentPosition.x -= Character_vel*2;
			LOG("DASH FUNCTION LEFT");
		}
		else if (StartPosition.x + DashDist > CurrentPosition.x && DashActiveRight== true) {

			CurrentPosition.x += Character_vel * 2;
			LOG("DASH FUNCTION RIGHT");
		}
		else {
			DashActiveLeft = false;
			DashActiveRight = false;
			PlayerState = IdleState;
			LOG("DASH  TO IDLE");
		}
	}

	//SLIME

	bool SlimeForm;

	
};
