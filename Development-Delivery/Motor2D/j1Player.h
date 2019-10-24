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
	bool Space_active;// JUMP
	bool U_active;//DASH ABILITY 2
	bool I_active;//TRANSFORMATION ABILITY 3
	//bool W_active; JUMP


	//CONSOLE
	bool F1_active; //START LEVEL 1
	bool F2_active; //START LEVEL 2
	bool F3_active; //Start Current Level from Beginning(FET)
	bool F5_active; //Save 
	bool F6_active; //Load
	bool F9_active; //View Colliders
	bool F10_active; //God Mode (FET)
	bool F11_active; //FOCUS CAMERA ON THE  PLAYER
	
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
	SlimeState,
	DashState,
	

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
	float Character_vel;
	SDL_Texture*		Graphics = nullptr;
	//For printing the animation in each moment
	Animation*			CurrentAnimation;

	// ANIMATION
	Animation			idle;



	//JUMP

	bool On_Ground;
	float Jump_Pow = 20.0f;
	float Gravity; //10
	float character_velY;
	float MaxJump= 200.0f;
	uint On_Ground_Counter;
	bool Jump_Ready;
	uint temppos;
	bool Max_Reached;
	float FallingXSpeed = 10.0f;
	bool FallingLeft;
	bool FallingRight;



	void On_The_Ground(){ //AIXO VA ALS IF DELS STATES

		if (CurrentPosition.y == floor) {

			On_Ground = true;
			LOG("FLOOR EQUAL TO CURRENT POSITION");

		}
		else if (CurrentPosition.y<= floor) {


			On_Ground = false;
			LOG("FLOOR MES GRAN QUE CURRENT POSITION");

		}
		else if(CurrentPosition.y>=floor){ //AIXO AMB ELS COLLIDERS NO FARA FALTA PERO PER ARA EL DEIXO AQUI


			floor = CurrentPosition.y;

			On_Ground = true;

			LOG("CHARACTER UNDER THE FLOOR");
		}
	}

	void Jumping() {

		temppos = CurrentPosition.y;
		LOG("%f", temppos);

		if (StartPosition.y - MaxJump < CurrentPosition.y && Max_Reached == false ) {

			CurrentPosition.y -= Jump_Pow;

			if (StartPosition.y - MaxJump >= CurrentPosition.y) {
				Max_Reached = true;
			}
			
			

			if (PlayerInput.A_active && FallingRight != true) {


				FallingLeft = true;
				LOG("FALLING LEFT");

			}

			if (PlayerInput.D_active&&FallingLeft != true) {


				FallingRight = true;
				LOG("FALLING RIGHT");
			}

			if (FallingLeft == true) {

				CurrentPosition.x -= Character_vel;

			}

			if (FallingRight == true) {

				CurrentPosition.x += Character_vel;


			}

			LOG("JUMPING UP");
		}

		if (Max_Reached == true) {
		
			CurrentPosition.y += Gravity;
			LOG("FALLING");
			On_The_Ground();

			if (PlayerInput.A_active && FallingRight != true) {


				FallingLeft = true;
				LOG("FALLING LEFT");

			}

			if (PlayerInput.D_active&&FallingLeft != true) {


				FallingRight = true;
				LOG("FALLING RIGHT");
			}

			if (FallingLeft == true) {

				CurrentPosition.x -= Character_vel;

			}

			if (FallingRight == true) {

				CurrentPosition.x += Character_vel;


			}

			
			if (On_Ground == true) {
				
				LOG("JUMP TO IDLE");
				PlayerState = IdleState;
				Max_Reached = false;
				FallingLeft = false;
				FallingRight = false;
			}
		}
	}






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

	
	//AUDIO
	
	


};
