#include "j1App.h"
#include "j1Player.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Animation.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "p2Log.h"
#include "j1Map.h"



j1Player::j1Player() 
{
	name.create("player");

	idle.PushBack({ 0,0,21,35 });
	idle.PushBack({ 21,0,21,35 });
	idle.PushBack({ 42,0,21,35 });
	idle.PushBack({ 63,0,21,35 });
	idle.PushBack({ 84,0,21,35 });
	idle.PushBack({ 105,0,21,35 });
	idle.PushBack({ 126,0,21,35 });
	idle.PushBack({ 0,35,21,35 });
	idle.PushBack({ 21,35,21,35 });
	idle.PushBack({ 42,35,21,35 });
	idle.PushBack({ 63,35,21,35 });
	idle.PushBack({ 84,35,21,35 });
	idle.speed = 0.5;


	jump.PushBack({ 0,70,21,35 });
	jump.PushBack({ 21,70,21,35 });
	jump.PushBack({ 42,70,21,35 });
	jump.PushBack({ 63,70,21,35 });
	jump.speed = 0.2;

	run.PushBack({ 0,105,21,35 });
	run.PushBack({ 21,105,21,35 });
	run.PushBack({ 42,105,21,35 });
	run.PushBack({ 63,105,21,35 });
	run.PushBack({ 84,105,21,35 });
	run.PushBack({ 105,105,21,35 });
	run.PushBack({ 126,105,21,35 });
	run.PushBack({ 147,105,21,35 });
	run.speed = 0.5;

	dash.PushBack({ 0,140,21,35 });
	dash.PushBack({ 21,140,21,35 });
	dash.PushBack({ 42,140,21,35 });
	dash.PushBack({ 63,140,21,35 });
	dash.PushBack({ 84,140,21,35 });
	dash.PushBack({ 105,140,21,35 });
	dash.speed = 0.5;
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
	 Character_vel.x = node.attribute("velocity_X").as_float();
	 Gravity = node.attribute("gravity").as_float();
	 DashDist = node.attribute("DashDistance").as_float();
	 Character_vel.y = node.attribute("velocity_Y").as_float();
	 
	 


	 return ret;
 }


 bool  j1Player::Start() 
 {
	 LOG("Loading player textures");
	 
	 
	 Graphics = App->tex->Load("Sprites/Dude.png");
	 floor = CurrentPosition.y;
	 PlayerState = IdleState;

	 CurrentPosition = { Inipos.x, Inipos.y } ;

	 Player_Rect = { CurrentPosition.x, CurrentPosition.y, 21, 35 };

	 Player_Collider = App->collision->AddCollider(Player_Rect, ObjectType::Player, this);
	
	// On_Ground = true;
	 CanJump = true;
	
	 return true;
 }

 bool j1Player::PreUpdate()
 {  //1024
	//768
	/*App->render->camera.x = CurrentPosition.x;
	App->render->camera.y = CurrentPosition.y;*/ //768/2
	
	 On_Ground;
	 if (On_Ground == false) {

		 Falling = true;
	 }

	 if (Falling == true)
	 {

		 CurrentPosition.y += Gravity;


	 }
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
		Player_Collider->to_delete = true;
		Gravity = 0;

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
			
			if (PlayerInput.W_active && CanJump == true) {
				CanJump = false;
				Character_vel.y = 50;
				PlayerState = JumpState;
				LOG("IDLE TO JUMP");
				

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

			if (PlayerInput.W_active && CanJump==true) {
				Character_vel.y = 50;
				
					PlayerState = JumpState;
					LOG("LEFT TO JUMP");
				
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

			if (PlayerInput.W_active && CanJump == true) {
				Character_vel.y = 50;
				PlayerState = JumpState;
					LOG("LEFT TO JUMP");
				

			}

		}

		
		if (PlayerState == JumpState)
		{
			
			
		}

	}

	else {


		if (PlayerInput.AG_active) {
			CurrentPosition.x -= Character_vel.x;

		}

		if (PlayerInput.DG_active) {
			CurrentPosition.x += Character_vel.x;

		}

		if (PlayerInput.WG_active) {
			CurrentPosition.y -= Character_vel.x;

		}

		if (PlayerInput.SG_active) {
			CurrentPosition.y += Character_vel.x;

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
		//LOG("IDLE");
		CurrentAnimation = &idle;
		break;

	case LeftState:
		LOG("MOVING LEFT");
		flip = true;
		CurrentPosition.x -= Character_vel.x;
		CurrentAnimation = &run;
		break;

	case RightState:
		LOG("MOVING RIGHT");
		flip = false;
		CurrentPosition.x += Character_vel.x;
		CurrentAnimation = &run;
		break;
	case JumpState:
		EndJump = false;
		CurrentAnimation = &jump;
		On_The_Ground();
		if (Jump_Ready == false) {
			PlayerState = IdleState;
			CurrentAnimation = &idle;
		}
		if (Jump_Ready == true) {
			MidAirUP = true;
			
			Jumping();
		}
		if (EndJump == true) {
			PlayerState = IdleState;
			CurrentAnimation = &idle;
		 }
		
		LOG("JUMP STATE ACTIVE");
		break;
		
	case DashState:
		
		//Mix_PlayChannel(-1,Jump_Sound , 0);
		CurrentAnimation = &dash;
		DashFunction();
		LOG("DASH");
		break;
	}


	Player_Rect.x = CurrentPosition.x;
	Player_Rect.y = CurrentPosition.y;

	Player_Collider->SetPos(CurrentPosition.x, CurrentPosition.y);

	//OnCollision(Collider *A, collider *B);

	SDL_Rect r = CurrentAnimation->GetCurrentFrame();

	App->render->Blit(Graphics, CurrentPosition.x, CurrentPosition.y, &r, flip);

	if (flip == false)
	{
		Player_Collider->SetPos(CurrentPosition.x, CurrentPosition.y); //Makes the collider follow the player.
	}
	else
	{
		Player_Collider->SetPos(CurrentPosition.x + 15, CurrentPosition.y); //Makes the collider follow the player.
	}

	


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

				 CurrentPosition.x = LastPosition.x - 1; 
				 LOG("PLAYER INTO WALL FROM THE LEFT");
				 
			 }

			 else if (B->rect.x+ B->rect.w < CurrentPosition.x)
			 { //Player to the right
				 CurrentPosition.x = B->rect.x + 5; 
				 LOG("PLAYER INTO WALL FROM THE RIGHT");
			 }
		 }

		 //from above
		 if ((CurrentPosition.y + A->rect.h) <= B->rect.y + 20 ) // from above
		 {
			 if ((A->rect.x + A->rect.w < B->rect.x) || (A->rect.x + A->rect.w < B->rect.x + B->rect.w)) { 

			 CurrentPosition.y = LastPosition.y;
			 if (GOD_MODE != true) {
				 On_Ground = true;
				 CanJump = true;
			 }
			// LOG("PLAYER INTO WALL FROM THE TOP");
			 }
		 }
	 }

	 if (GOD_MODE == false) {
		 if (A->type == ObjectType::Player && B->type == ObjectType::Water) {

			 if ((CurrentPosition.y + A->rect.h) <= B->rect.y + 20) // from above
			 {
				 CurrentPosition.x = Inipos.x;
				 CurrentPosition.y = Inipos.y;
			 }
		 }
	 }

 }
 