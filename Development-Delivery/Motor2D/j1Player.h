#include "p2List.h"
#include "p2Point.h"
#include "p2Log.h"
#include "j1Entities.h"
#include "j1Audio.h"
#include "j1Input.h"


struct SDL_Rect;
struct Animation;

enum EntitiesType;

struct Input_player
{
	//GAMEPLAY
	bool A_active; //LEFT
	bool D_active;//RIGHT
	bool Space_active;// JUMP
	bool U_active;//DASH ABILITY 2
	bool I_active;//TELEPORT
	
	

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


class j1Player : public j1Entities
{
public:
	
	j1Player(iPoint Position, EntitiesType type);

	// Destructor
	virtual ~j1Player();

	bool Start();
	bool PreUpdate();	
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node &node); //Load 
	bool Save(pugi::xml_node &node) const;//Save
	// Collision handling -----
	void OnCollision(Collider* A, Collider* B);


	iPoint Inipos;

	//CONTROLLS F
	bool GOD_MODE = false;
	bool flip = false;

	float floor;

	//AUDIO THINGS 

	//INPUT COSES
	Input_player		PlayerInput;
	SDL_Rect			Player_Rect;
	int 				Player_Width;
	int					Player_Height;

	//ACTUALIZING POSITION 
	iPoint              StartPosition;
	iPoint              LastPosition;
	iPoint              Character_vel;
	
	// ANIMATION
	Animation			idle;
	Animation			jump;
	Animation			run;
	Animation			dash;
	
	bool InitEntity();

	//TELEPORT

	void PlayerTP(int posx,int posy);
	//MOVEMENT

	void Movement();


	//JUMP
	float TempVelY;
	float FallingVel = 0;
	bool On_Ground;
	float Gravity; //10
	float GravitySave;
	bool Jump_Ready;
	bool MidAirUP;
	bool Falling;
	bool EndJump;
	bool CanJump;
	bool TouchingCollider;
	bool JumpTicks;
	bool FallLeft;
	bool FallRight;
	
	void On_The_Air() {

		CanJump = false;
		if (On_Ground == true) {
			Jump_Ready = false;
		}
		else if(On_Ground==false){
			Jump_Ready = true;
		}

	}

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

	void Jumping();
	
	//DASH THINGS
	float DashDist;
	bool DashActiveLeft;
	bool DashActiveRight;
	bool CanDash;
	bool DashDone;
	

	void DashFunction();

};

