#pragma once
#include "p2Animation.h"
#include "j1Module.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Point.h"
#include "j1Audio.h"
#include "p2Log.h"


struct Collider;
struct SDL_Texture;
struct p2Animation;

struct Input_player
{
	//GAMEPLAY
	bool A_active; //LEFT
	bool D_active;//RIGHT
	bool W_active;// JUMP
	bool U_active;//DASH ABILITY 2
	bool I_active;//TRANSFORMATION ABILITY 3
	

	//CONSOLE
	bool F1_active; //START LEVEL 1
	bool F2_active; //START LEVEL 2
	bool F3_active; //Start Current Level from Beginning(FET)
	bool F5_active; //Save 
	bool F6_active; //Load
	bool F9_active; //View Colliders
	bool F10_active; //God Mode (FET)
	
	//GOD MODE
	bool AG_active;//LEFT
	bool DG_active;//RIGHT
	bool SG_active;//UP
	bool WG_active;//DOWN

};
enum CurrentState 
{

	IdleState,
	JumpState,
	LeftState,
	RightState,
	DashState,
	FallState,

};


class j1Player : public j1Module 
{
private:

	iPoint Inipos;	
	
public:
	

	bool Load(pugi::xml_node &node); //Load 
	bool Save(pugi::xml_node &node) const;//Save



	j1Player();

	// Destructor
	virtual ~j1Player();
	
	bool Awake(pugi::xml_node& conf);
	bool Start();
	bool PreUpdate();	
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	// Collision handling -----
	void j1Player::OnCollision(Collider* A, Collider* B);


	//CONTROLLS F

	bool GOD_MODE = false;

	float floor = CurrentPosition.y; //NOMES PER TESTING


	//AUDIO THINGS 

	//INPUT COSES
	Input_player		PlayerInput;
	CurrentState		PlayerState;
	SDL_Rect			Player_Rect;
	Collider*			Player_Collider;

	//ACTUALIZING POSITION 
	iPoint				CurrentPosition;
	iPoint              StartPosition;
	iPoint              LastPosition;
	iPoint              Character_vel;
	SDL_Texture*		Graphics = nullptr;
	//For printing the animation in each moment
	Animation*			CurrentAnimation;

	bool flip = false;
	// ANIMATION

	Animation			idle;
	Animation			jump;
	Animation			run;
	Animation			dash;
	
	//MOVEMENT

	void Movement() {

		if (PlayerInput.A_active && TouchingCollider==false) {
			CurrentPosition.x -= Character_vel.x;
		}

		if (PlayerInput.D_active&&TouchingCollider==false) {
			CurrentPosition.x += Character_vel.x;
		}
	}





	//JUMP
	bool On_Ground;
	float Jump_Pow = 20.0f;
	float Gravity; //10
	float MaxJump= 90.0f;
	iPoint  On_Ground_Counter;
	bool Jump_Ready;
	uint temppos;
	bool Max_Reached;
	bool FallingLeft;
	bool FallingRight;
	float Acceleration_Y;
	bool MidAirUP;
	bool MidAirDOWN;
	bool Hit_Platform_From_Down = false;
	bool Hit_PLatform_From_Up = false;
	bool Hit_Platform_From_Side = false;
	bool Falling;
	bool EndJump;
	bool CanJump;
	bool TouchingCollider;
	
	void On_The_Ground() {

		//CHECK COLLISION
		CanJump = false;
		if (On_Ground == true) {

			Jump_Ready = true;

			LOG("ON GROUND TRUE");
		}
		else if (On_Ground == false) {

			Jump_Ready = false;

			LOG("ON GROUND FALSE");
		}



	}

	void Jumping() {

		CanJump = false;
		
		if (MidAirUP == true) {
			LOG("MID AIR TRUE");
			Character_vel.y -= Gravity; //CHARACTER VELOCITY = 0 AL SEGON SALT PER AIXO NO VA
			
			if (PlayerInput.A_active) {
				CurrentPosition.x -= Gravity;
			}
			if (PlayerInput.D_active) {
				CurrentPosition.x += Gravity;
			}
			if (Character_vel.y <= 0) {
				float altura = CurrentPosition.y;
				LOG("VELY REACHED 0   %f",altura);
				MidAirUP = false;
			}

			CurrentPosition.y -= Character_vel.y;

		}

		if (MidAirUP == false) {
			LOG("MID AIR UP == FALSE");
			Character_vel.y += Gravity;
		//	On_Ground = Jump_Ready;
			
			if (On_Ground == true) {
				LOG("TO IDLE FROM JUMP");
				
				EndJump = true;
				
				PlayerState = IdleState;
			}
			
			if (On_Ground == false) {
				
				LOG("FALLING");
				CurrentPosition.y += Character_vel.y;

			}




		}


	}
	
	//DASH THINGS
	float DashDist;
	bool DashActiveLeft;
	bool DashActiveRight;
	float DashTime;
	float DashPrep = 20;
	void DashFunction()
	{
		
		
		if (DashActiveLeft== true) {
			CurrentPosition.x += Character_vel.x;
			CurrentPosition.x = StartPosition.x - DashDist;
			LOG("DASH FUNCTION LEFT");
			DashActiveLeft = false;
			DashActiveRight = false;
			if (CurrentAnimation->FinishedAnimation()) {
				PlayerState = IdleState;
			}
		}
		else if( DashActiveRight== true) {
			CurrentPosition.x -= Character_vel.x;
			CurrentPosition.x = StartPosition.x + DashDist;
			LOG("DASH FUNCTION RIGHT");
			DashActiveLeft = false;
			DashActiveRight = false;
			PlayerState = IdleState;
		}
		else {
			DashActiveLeft = false;
			DashActiveRight = false;
			PlayerState = IdleState;
			LOG("DASH  TO IDLE");
		}
	}

	//AUDIO
	

};
