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
	Graphics = NULL;
	CurrentAnimation = NULL;
	idle.PushBack({ 0,0,17,26 });
	idle.PushBack({ 0,0,17,26 });

}


 j1Player::~j1Player()
 {
 
 }


 bool j1Player::Awake(pugi::xml_node config)
 {

	 return true;
 }



 bool j1Player::Start() 
 {

	 CurrentPosition = { 100, 0};

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

	 if (PlayerInput.A_active) 
	 {
		PlayerState = LeftState;
	 }

	 if (PlayerInput.D_active) 
	 {
		 PlayerState = RightState;
	 }

	 if (PlayerInput.Space_active)
	 {
		 PlayerState = JumpState;
	 }

	 return true;
 }


 bool j1Player::Update()
 {

	 //APLICACIO DELS DIFFERENTS PLAYER STATES
	 switch (PlayerState)
	 {
	 case IdleState:
		 LOG("IDLE");
		 CurrentAnimation = &idle;
		 break;

	 case LeftState:

		 break;


	 case RightState:

		 break;


	 case JumpState:

		 break;



	 default:
		 break;
	 }

	 SDL_Rect r = CurrentAnimation->GetCurrentFrame();

	 App->render->Blit(Graphics, CurrentPosition.x, CurrentPosition.y , &r, 1.0f, true);

 
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