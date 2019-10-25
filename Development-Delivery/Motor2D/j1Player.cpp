#include "j1App.h"
#include "j1Player.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Animation.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1Map.h"



j1Player::j1Player() 
{
	name.create("player");
}


 j1Player::~j1Player()
 {
 }

 bool j1Player::Load(pugi::xml_node& data)
 {
	 CurrentPosition.x = data.child("position1").attribute("x").as_int();
	 CurrentPosition.y = data.child("position1").attribute("y").as_int();
	 return true;
 }


 bool j1Player::Save(pugi::xml_node&  data) const
 {
	 pugi::xml_node pos = data.append_child("position1");

	 pos.append_attribute("x") = CurrentPosition.x;
	 pos.append_attribute("y") = CurrentPosition.y;
	 return true;
 }



 bool j1Player::Awake(pugi::xml_node& node)
 {

	 bool ret = true;
	
	 pugi::xml_node player = node.child("player");

	 Inipos.x = node.attribute("inipos_x").as_float();
	 Inipos.y = node.attribute("inipos_y").as_float();
	 Character_vel.x = node.attribute("velocity").as_float();
	 Gravity = node.attribute("gravity").as_float();
	 DashDist = node.attribute("DashDistance").as_float();
	 
	 

	 return ret;
 }


 bool  j1Player::Start() 
 {
	 LOG("Loading player textures");
	 
	 
	 Graphics = App->tex->Load("Sprites/Dude.png");
	 floor = CurrentPosition.y;
	 PlayerState = IdleState;

	 CurrentPosition = { Inipos.x, Inipos.y };

	 Player_Rect = { CurrentPosition.x, CurrentPosition.y, 21, 35 };

	 Player_Collider = App->collision->AddCollider(Player_Rect, ObjectType::Player, this);
	
	 On_Ground = true;

	 return true;
 }

bool j1Player::PreUpdate() 
 {  //1024
	//768
	/*App->render->camera.x = CurrentPosition.x;
	App->render->camera.y = CurrentPosition.y;*/ //768/2
	PlayerInput.F10_active = App->input->keyboard[SDL_SCANCODE_F10] == KEY_DOWN;
	PlayerInput.F3_active = App->input->keyboard[SDL_SCANCODE_F3] == KEY_DOWN;

	if(PlayerInput.F10_active && GOD_MODE==false) {
		LOG("GOD MODE ON");
		GOD_MODE = true;
	}
	else if (PlayerInput.F10_active && GOD_MODE == true) {

		LOG("GOD MODE OFF");
		GOD_MODE = false;

	}

	if (GOD_MODE == false) {


		PlayerInput.A_active = App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT;
		PlayerInput.D_active = App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT;
		PlayerInput.W_active = App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN;
		PlayerInput.U_active = App->input->keyboard[SDL_SCANCODE_U] == KEY_DOWN;
		PlayerInput.I_active = App->input->keyboard[SDL_SCANCODE_I] == KEY_DOWN;
		
		
	}

	else  {

		PlayerInput.AG_active = App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT;
		PlayerInput.DG_active = App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT;
		PlayerInput.WG_active = App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT;
		PlayerInput.SG_active = App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT;


	}
	 //AGAFAR INPUTS I TAL, HE MIRAT I CREC QUE FERHO AIXI ES MILLOR QUE TOT JUNT 
	 
	//START CURRENT LEVEL
	if (PlayerInput.F3_active) {

		CurrentPosition.x = Inipos.x;
		CurrentPosition.y =Inipos.y ;
	}


	//ROTACIO DELS PLAYER STATES 
	if (GOD_MODE == false) {
		if (PlayerState == IdleState)
		{
			
			if (PlayerInput.A_active)
			{
				PlayerState = LeftState;
				LOG("IDLE TO LEFT");
			}

			if (PlayerInput.D_active)
			{
				PlayerState = RightState;
				LOG("IDLE TO RIGHT");
			}

			if (PlayerInput.W_active) {

				if (On_Ground == true) {

					StartPosition.y = CurrentPosition.y;
					PlayerState = JumpState;
					LOG("IDLE TO JUMP");
				}

				if (On_Ground == false) {

					LOG("JUMP NOT AVAILABLE");
				}

			}
			
		}

		if (PlayerState == LeftState)
		{
			
			if (!PlayerInput.A_active)
			{

				PlayerState = IdleState;
				LOG("LEFT TO IDLE");
			}

			if (PlayerInput.U_active) {
				DashActiveLeft = true;
				StartPosition.x = CurrentPosition.x;
				PlayerState = DashState;
				LOG("LEFT TO DASH LEFT");
			}

			if (PlayerInput.W_active ) {

				if (On_Ground == true) {

					StartPosition.y = CurrentPosition.y;
					PlayerState = JumpState;
					LOG("LEFT TO JUMP");
				}
			}

		}
		if (PlayerState == RightState)
		{
			
			if (!PlayerInput.D_active)
			{
				PlayerState = IdleState;
				LOG("RIGHT TO IDLE");
			}

			if (PlayerInput.U_active) {
				DashActiveRight = true;
				StartPosition.x = CurrentPosition.x;
				PlayerState = DashState;
				LOG("LEFT TO DASH RIGHT");
			}

			if (PlayerInput.W_active) {

				if (On_Ground == true) {

					StartPosition.y = CurrentPosition.y;
					PlayerState = JumpState;
					LOG("LEFT TO JUMP");
				}

			}

		}

		if (PlayerState == JumpState)
		{
			
			if (On_Ground == true)
			{
				Jump_Ready = true;
				MidAirUP = false;
				LOG("1");


				if (PlayerState == DashState) {
					PlayerState = IdleState;
				}
			}
			else //FALLING IF NOT JUMP/DASH
			{
				LOG("2");
				if (PlayerState != DashState)
				{
					LOG("3");
					PlayerState == FallState;
				}
			}

			if (PlayerState == DashState)
			{
				//Dash Check
				//LIMIT DASH DESPRES HO FAIG
			}
			else //Player not Dashing
			{
				LOG("4");
				//Logic for when player is jumping
				if (MidAirUP)
				{
					LOG("5");
					Character_vel.x += Gravity; // Speed.y is +gravity when not grounded

				}

				CurrentPosition.y += Character_vel.y; //Update position y

				if (Hit_PLatform_From_Up)
				{
					LOG("6");
					Character_vel.y = 0;
				}
			}

			CurrentPosition.x += Character_vel.x;
			LOG("7");
			if (Hit_Platform_From_Side)
			{
				LOG("8");
				Character_vel.x = 0;
			}

		}

	}

	else {


		if (PlayerInput.AG_active) {
			CurrentPosition.x -= Character_vel.x*2;

		}

		if (PlayerInput.DG_active) {
			CurrentPosition.x += Character_vel.x*2;

		}

		if (PlayerInput.WG_active) {
			CurrentPosition.y -= Character_vel.x*2;

		}

		if (PlayerInput.SG_active) {
			CurrentPosition.y += Character_vel.x*2;

		}

	}

	 return true ;
}


bool j1Player::Update(float dt)
 {
	LastPosition = CurrentPosition;
	bool ret = true;
	//APLICACIO DELS DIFFERENTS PLAYER STATES
	switch (PlayerState)
	{
	case IdleState:
		LOG("IDLE");
		CurrentAnimation = &idle;
		break;

	case LeftState:
		LOG("MOVING LEFT");
		CurrentPosition.x -= Character_vel.x;
		CurrentPosition.y;
		break;

	case RightState:
		LOG("MOVING RIGHT");
		CurrentPosition.x += Character_vel.x;
		CurrentPosition.y;
		break;
	case JumpState:
		
		Character_vel.y -= Acceleration_Y; //character vel potser ha de ser CURRENTPOSITION
		MidAirUP = true;
		Jump_Ready = false;
		LOG("JUMP STATE ACTIVE");
		break;
		
	case DashState:
		
		//Mix_PlayChannel(-1,Jump_Sound , 0);
		DashFunction();
		LOG("DASH");
		break;
	}


	Player_Rect.x = CurrentPosition.x;
	Player_Rect.y = CurrentPosition.y;

	Player_Collider->SetPos(CurrentPosition.x, CurrentPosition.y);

	//OnCollision(Collider *A, collider *B);


	


	return true;
}


 bool j1Player::PostUpdate()
 {


	 return true;
 }

 bool j1Player::CleanUp()
 {

	 LOG("Unloading Player");

	 App->tex->UnLoad(Graphics);

	 return true;
 }

 void j1Player::OnCollision(Collider* A, Collider* B) {

	 if (B->type == ObjectType::Player) {
		 Collider temp = *A;
		 A = B;
		 B = &temp;
	 }
	 if (A->type != ObjectType::Player) {
		 return;
	 }

	 if (A->type == ObjectType::Player && B->type == ObjectType::Platform) {

		 //from below
		 if (LastPosition.y > (B->rect.y + B->rect.h - 1))
		 {
			 CurrentPosition.y = B->rect.y + B->rect.h;
		 }

		 //from a side
		 if (((CurrentPosition.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((CurrentPosition.y + A->rect.h)  > B->rect.y))
		 {

			 if ((A->rect.x + A->rect.w - 5) <= (B->rect.x ))
			 { //Player to the left 
				 CurrentPosition.x = LastPosition.x; 
				
				 LOG("PLAYER INTO WALL FROM THE LEFT");
			 }

			 else if (A->rect.x >= (B->rect.x + B->rect.w - 5 )) 
			 { //Player to the right
				 CurrentPosition.x = B->rect.x + B->rect.w; 
				 LOG("PLAYER INTO WALL FROM THE RIGHT");
			 }
		 }

		 //from above
		 if (((CurrentPosition.y + A->rect.h) > B->rect.y - 20) && (A->rect.x > B->rect.x + B->rect.w) && (A->rect.x + A->rect.w < B->rect.x)) { // from above

			 CurrentPosition.y = LastPosition.y;

			 On_Ground = true;
			 LOG("PLAYER INTO WALL FROM THE TOP");
		 }
	 }

	 


 }
 