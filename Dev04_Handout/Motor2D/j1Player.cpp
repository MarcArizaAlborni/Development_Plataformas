#include "j1Module.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Point.h"


j1Player::j1Player(){

	
	

}


 j1Player::~j1Player() {



}



 bool j1Player::Init() {



	 return 0;
 }


 bool j1Player::Awake(pugi::xml_node config) {


	 return 0;
 }



 bool j1Player::Start() {




	 return 0;
 }

 bool j1Player::PreUpdate() {

	 PlayerInput.A_active = App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT;
	 PlayerInput.D_active = App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT;
	 PlayerInput.Space_active = App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT;

	PlayerState = idle;


	 if (PlayerInput.A_active) {

		PlayerState = move_left;
	 }

	 if (PlayerInput.D_active) {
		 PlayerState = move_right;
	 }

	 if (PlayerInput.Space_active) {

		 PlayerState = jump;

	 }
	

	 return 0;
 }


 bool j1Player::Update() {

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


	 return 0;
 }

 bool j1Player::PostUpdate() {



	 return 0;

 }



 bool j1Player::CleanUp() {


	 return 0;

 }