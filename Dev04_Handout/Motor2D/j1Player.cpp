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

	 PlayerInput.A_active = App->input->GetKey[SDL_SCANCODE_A] == KEY_REPEAT;
	 PlayerInput.D_active = App->input->GetKey[SDL_SCANCODE_D] == KEY_REPEAT;
	 PlayerInput.Space_active = App->input->GetKey[SDL_SCANCODE_SPACE] == KEY_REPEAT;



	 return 0;


 }


 bool j1Player::Update() {

	 /*switch (CurrentState)
	 {
	 case idle:
	 

		 break;


	 default:
		 break;
	 }*/


	 return 0;
 }

 bool j1Player::PostUpdate() {



	 return 0;

 }



 bool j1Player::CleanUp() {


	 return 0;

 }