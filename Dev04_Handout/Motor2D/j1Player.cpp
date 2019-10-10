#include "j1Module.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Point.h"


j1Player::j1Player()
{

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

	 CurrentPosition = {0,0 };

	 return true;
 }

 bool j1Player::PreUpdate() 
 {

	 //AGAFAR INPUTS I TAL, HE MIRAT I CREC QUE FERHO AIXI ES MILLOR QUE TOT JUNT 
	 PlayerInput.A_active = App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT;
	 PlayerInput.D_active = App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT;
	 PlayerInput.Space_active = App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT;

	

	//ROTACIO DELS PLAYER STATES 
	 PlayerState = idle;


	 if (PlayerInput.A_active) 
	 {
		PlayerState = move_left;
	 }

	 if (PlayerInput.D_active) 
	 {
		 PlayerState = move_right;
	 }

	 if (PlayerInput.Space_active)
	 {
		 PlayerState = jump;
	 }

	 return true;
 }


 bool j1Player::Update()
 {

	 //APLICACIO DELS DIFFERENTS PLAYER STATES
	 switch (PlayerState)
	 {
	 case idle:
	 

		 break;

	 case move_left:

		 break;


	 case move_right:

		 break;


	 case jump:

		 break;



	 default:
		 break;
	 }


	 return true;
 }

 bool j1Player::PostUpdate()
 {


	 return true;
 }



 bool j1Player::CleanUp()
 {


	 return true;
 }