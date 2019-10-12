#include "p2Point.h"
#include "p2Animation.h"
#include "p2Log.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Player.h"



j1Player::j1Player() 
{
	name.create("player");
}


 j1Player::~j1Player()
 {
 }

 bool j1Player::Awake(pugi::xml_node& node)
 {
	 bool ret = true;

	 pugi::xml_node player = node.child("player");

	 Inipos.x = node.attribute("inipos_x").as_float();
	 Inipos.y = node.attribute("inipos.y").as_float();
	 Character_vel = node.attribute("velocity").as_float();
	 Gravity = node.attribute("gravity").as_float();

	 
	
	 return ret;
 }


 bool  j1Player::Start() 
 {
	
	 CurrentPosition = { 0, 400};

	 LOG("Loading player textures");

	 Graphics = App->tex->Load("Sprites/Dude.png");

	 PlayerState = IdleState;

	 return true;
 }

bool j1Player::PreUpdate() 
 {

	 //AGAFAR INPUTS I TAL, HE MIRAT I CREC QUE FERHO AIXI ES MILLOR QUE TOT JUNT 
	 PlayerInput.A_active = App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT;
	 PlayerInput.D_active = App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT;
	 PlayerInput.Space_active = App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT;

	//ROTACIO DELS PLAYER STATES 

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

		 if (PlayerInput.Space_active)
		 {
			 PlayerState = JumpState;

			 LOG("IDLE TO JUMP");
		 }
	 }
	 

	 if (PlayerState == LeftState) 
	 {
		 if (!PlayerInput.A_active) 
		 {

			 PlayerState = IdleState;
			 LOG("LEFT TO IDLE");
		 }

	 }
	 
	 if (PlayerState == RightState) 
	 {
		 if (!PlayerInput.D_active) 
		 {
			 PlayerState = IdleState;
			 LOG("RIGHT TO IDLE");
		 }


	 }
	 
	 if (PlayerState == JumpState) 
	 {
		 if (!PlayerInput.Space_active)
		 {
			 PlayerState = IdleState;
			 LOG("JUMP TO IDLE");
		 }


		 
	 }
	 return true ;
}


bool j1Player::Update(float dt)
 {
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
		CurrentPosition.x -= Character_vel;
	break;
	
	case RightState:
		LOG("MOVING RIGHT");
		CurrentPosition.x += Character_vel;

	break;
	
	case JumpState:
		LOG("JUMPING");

	break;
	
	}


	SDL_Rect r = CurrentAnimation->GetCurrentFrame();
	
	App->render->Blit(Graphics, CurrentPosition.x, CurrentPosition.y, &r, 1.0f, true);

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

