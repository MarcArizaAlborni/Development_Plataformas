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
	 Inipos.y = node.attribute("inipos_y").as_float();
	 Character_vel = node.attribute("velocity").as_float();
	 Gravity = node.attribute("gravity").as_float();
	 
	 CurrentPosition = { Inipos.x, Inipos.y };

	/* for (pugi::xml_node animations = node.child("animation"); animations && ret; animations.next_sibling("animation"))
	 {
		 
		 idle.loop = animations.attribute("loop").as_bool();
		 idle.speed = animations.attribute("speed").as_float();

		 for (pugi::xml_node frame = node.child("frame"); frame && ret; frame.next_sibling("frame"))
		 {
			 idle.PushBack({ frame.attribute("x").as_int(), frame.attribute("y").as_int(), frame.attribute("w").as_int(), frame.attribute("h").as_int() });
		 }
		
	 }*/
	 return ret;
 }


 bool  j1Player::Start() 
 {
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
	 PlayerInput.Space_active = App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN;
	 PlayerInput.U_active = App->input->keyboard[SDL_SCANCODE_U] == KEY_REPEAT;
	 PlayerInput.U_active = App->input->keyboard[SDL_SCANCODE_I] == KEY_REPEAT;
	

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

		 if (PlayerInput.U_active) {

			 PlayerState = DashStateLeft;
			 LOG("LEFT TO DASH LEFT");

		 }
		 
		 if (PlayerInput.Space_active) {

			 PlayerState = JumpStateLeft;
			 LOG("LEFT TO JUMP LEFT");

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

			 PlayerState = DashStateRight;
			 LOG("RIGHT TO DASH RIGHT");

		 }
		 
		 if (PlayerInput.Space_active) {

			 PlayerState = JumpStateRight;
			 LOG("RIGHT TO JUMP RIGHT");
		 }

		
	 }
	 
	 if (PlayerState == JumpStateLeft) 
	 {
		 
		 //PER QUAN ESTIGUIN LES ANIMACIONS FEM HO DE anim_finished-> PlayerState=idle; o  player.x pos == alguna cosa playerstate = idlestate
		
		 if (PlayerInput.Space_active && PlayerInput.A_active) {//AQUEST D'AQUI CREC QUE EL TREUREM I MIRARE COM FER EL SALT MILLOR 

			 PlayerState = DoubleJumpStateLeft;
			 LOG("JUMP LEFT TO DOUBLE JUMP LEFT");
		 }
		 
		 if (PlayerInput.Space_active && PlayerInput.D_active) {//AQUEST D'AQUI CREC QUE EL TREUREM I MIRARE COM FER EL SALT MILLOR 

			 PlayerState = DoubleJumpStateRight;
			 LOG("JUMP LEFT TO DOUBLE JUMP RIGHT");
		 }

		 if (PlayerInput.Space_active) {

			 PlayerState = DoubleJumpState;
			 LOG("JUMP LEFT TO DOUBLE JUMP");
		 }

		 if (PlayerInput.U_active && PlayerInput.A_active) {

			 PlayerState = DashStateLeft;
		 }

		 if (PlayerInput.U_active && PlayerInput.D_active) {

			 PlayerState = DashStateRight;
		 }
	 }

	 if (PlayerState == JumpStateRight)
	 {
		
		 //PER QUAN ESTIGUIN LES ANIMACIONS FEM HO DE anim_finished-> PlayerState=idle; o  player.x pos == alguna cosa playerstate = idlestate

		 if (PlayerInput.Space_active && PlayerInput.A_active) {//AQUEST D'AQUI CREC QUE EL TREUREM I MIRARE COM FER EL SALT MILLOR 

			 PlayerState = DoubleJumpStateLeft;
			 LOG("JUMP RIGHT TO DOUBLE JUMP LEFT");
		 }

		 if (PlayerInput.Space_active && PlayerInput.D_active) {//AQUEST D'AQUI CREC QUE EL TREUREM I MIRARE COM FER EL SALT MILLOR 

			 PlayerState = DoubleJumpStateRight;
			 LOG("JUMP RIGHT TO DOUBLE JUMP RIGHT");
		 }

		 if (PlayerInput.Space_active) {

			 PlayerState = DoubleJumpState;
			 LOG("JUMP RIGHT TO DOUBLE JUMP");

		 }

		 if (PlayerInput.U_active && PlayerInput.D_active) {

			 PlayerState = DashStateLeft;
			 LOG("DOUBLE JUMP RIGHT TO DASH LEFT");
		 }

		 if (PlayerInput.U_active && PlayerInput.A_active) {

			 PlayerState = DashStateRight;
			 LOG("DOUBLE JUMP RIGHT TO DASH RIGHT");
		 }
	 }
	 

	 if (PlayerState == DoubleJumpStateLeft) {


		 //PER QUAN ESTIGUIN LES ANIMACIONS FEM HO DE anim_finished-> PlayerState=idle; o  player.x pos == alguna cosa playerstate = idlestate

		 if (PlayerInput.U_active && PlayerInput.A_active) {

			 PlayerState = DashStateLeft;
			 LOG("DOUBLE JUMP RIGHT TO DASH LEFT");
		 }

		 if (PlayerInput.U_active && PlayerInput.D_active) {

			 PlayerState = DashStateRight;
			 LOG("DOUBLE JUMP RIGHT TO DASH RIGHT");
		 }
	 }

	 if (PlayerState == DoubleJumpStateRight) {

		 //PER QUAN ESTIGUIN LES ANIMACIONS FEM HO DE anim_finished-> PlayerState=idle; o  player.x pos == alguna cosa playerstate = idlestate

		 if (PlayerInput.U_active && PlayerInput.D_active) {

			 PlayerState = DashStateRight;
			 LOG("DOUBLE JUMP RIGHT TO DASH RIGHT");
		 }

		 if (PlayerInput.U_active && PlayerInput.A_active) {

			 PlayerState = DashStateLeft;
			 LOG("DOUBLE JUMP RIGHT TO DASH LEFT");
		 }
	 }

	 if (PlayerState == DashStateLeft) {

		 //PER QUAN ESTIGUIN LES ANIMACIONS FALTA UN ON PUGIS SALTAR DESPRES DEL DASH SI ENCARA NO HO HAS FET
		 //PER QUAN ESTIGUIN LES ANIMACIONS FEM HO DE anim_finished-> PlayerState=idle; o  player.x pos == alguna cosa playerstate = idlestate
	 }

	 if (PlayerState == DashStateRight) {

		 //PER QUAN ESTIGUIN LES ANIMACIONS FALTA UN ON PUGIS SALTAR DESPRES DEL DASH SI ENCARA NO HO HAS FET
		 //PER QUAN ESTIGUIN LES ANIMACIONS FEM HO DE anim_finished-> PlayerState=idle; o  player.x pos == alguna cosa playerstate = idlestate
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
	

	case JumpStateLeft:
		LOG("JUMPING LEFT");
		break;

	case JumpStateRight:
		LOG("JUMPING RIGHT");
		break;


	case DoubleJumpState:
		LOG("DOUBLE JUMPING ");
		break;


	case DoubleJumpStateLeft:
		LOG("DOUBLE JUMPING LEFT");
		break;

	case DoubleJumpStateRight:
		LOG("DOUBLE JUMPING RIGHT");
		break;


	case DashStateLeft:
		LOG("DASH LEFT");
		break;

	case DashStateRight:
		LOG("DASH RIGHT");
		break;


	}


	//SDL_Rect r = CurrentAnimation->GetCurrentFrame();
	
	//App->render->Blit(Graphics, CurrentPosition.x, CurrentPosition.y, &r, 1.0f, true);


	Player_Rect = { CurrentPosition.x, CurrentPosition.y, 50, 70 };

	App->render->DrawQuad(Player_Rect, 255, 255, 0);

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

