#include "p2Log.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Map.h"

j1Player::j1Player(iPoint pos, EntitiesType type) : j1Entities(pos, EntitiesType::PLAYER)
{

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

	//dash.PushBack({ 0,140,21,35 });
	//dash.PushBack({ 21,140,21,35 });
	//dash.PushBack({ 42,140,21,35 });
	//dash.PushBack({ 63,140,21,35 });
	//dash.PushBack({ 84,140,21,35 });
	/*dash.PushBack({ 172,131,24,43 });
	dash.PushBack({ 172,131,24,43 });*/
	dash.PushBack({ 172,131,24,43 });
	dash.PushBack({ 208,131,31,43 });
	/*dash.PushBack({ 208,131,31,43 });
	dash.PushBack({ 208,131,31,43 });*/
	//dash.PushBack({ 105,140,21,35 });
	dash.speed = 0.01;
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
	 Character_vel.x = node.attribute("velocity_X").as_float();
	 GravitySave = Gravity = node.attribute("gravity").as_float();
	 DashDist = node.attribute("DashDistance").as_float();
     TempVelY=Character_vel.y = node.attribute("velocity_Y").as_float();
	 Player_Width = node.attribute("width").as_int();
	 Player_Height = node.attribute("height").as_int();
	 CanDash = true;
	 JumpTicks = true;
	

	 PlayerState = IdleState;
	 CurrentAnimation = &idle;
	 CanJump = false;

	 return ret;
 }

 bool  j1Player::Start() 
 {
	 LOG("Loading player textures");
	 
	 Graphics = App->tex->Load("Sprites/DudeMOD.png");

	 InitPlayer();
	
	 return true;
 }

 bool j1Player::PreUpdate()
 {  
	
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
		PlayerInput.Space_active = App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN;
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
			
			if (PlayerInput.Space_active && CanJump == true && JumpTicks == true) {
				JumpTicks = false;
				CanJump = false;
				
				PlayerState = JumpState;
 				LOG("IDLE TO JUMP");
				

				if (On_Ground == false) {
					PlayerState = FallState;
					LOG("JUMP NOT AVAILABLE");
				}

			}

			if (PlayerInput.Space_active && CanJump == true ) {
				
				On_Ground = false;
				CanJump = false;
				Character_vel.y = 50;
				PlayerState = JumpState;
 				


				

			}
			
		}

		if (PlayerState == LeftState)
		{
			
			if (!PlayerInput.A_active)
			{

				PlayerState = IdleState;
				LOG("LEFT TO IDLE");
			}

			if (PlayerInput.U_active && CanDash == true) {
				DashActiveLeft = true;
				StartPosition.x = CurrentPosition.x;
				PlayerState = DashState;
				LOG("LEFT TO DASH LEFT");
			}

			if (PlayerInput.Space_active && CanJump==true ) {
				
				On_Ground = false;
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

			if (PlayerInput.U_active && CanDash==true) {
				DashActiveRight = true;
				StartPosition.x = CurrentPosition.x;
				PlayerState = DashState;
				LOG("LEFT TO DASH RIGHT");
			}

			if (PlayerInput.Space_active && CanJump == true) {
				//Character_vel.y = 50;
				PlayerState = JumpState;
				LOG("LEFT TO JUMP");
				

			}

		}
		if (PlayerState == FallState) {

			
			LOG("FALL STATE");
			Movement();

			if (PlayerInput.U_active && PlayerInput.D_active) {
				DashActiveRight = true;
				StartPosition.x = CurrentPosition.x;
				PlayerState = DashState;
			}
			else if (PlayerInput.U_active && PlayerInput.A_active) {
				DashActiveLeft = true;
				StartPosition.x = CurrentPosition.x;
				PlayerState = DashState;
			}

			//PlayerState = IdleState;
			if (CanJump == true) {
				LOG("22");
				TouchingCollider = true;
				PlayerState = IdleState;
			}
			
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
		TouchingCollider = false;
		//CurrentPosition.y -= Gravity;
		if (EndJump == true) {
			CurrentAnimation = &idle;

		}
		else {
			CurrentAnimation = &jump;
		}
		CanDash = true;
		JumpTicks = true;
		LOG("IDLE STATE");
		break;

	case LeftState:
		LOG("MOVING LEFT");
		flip = true;
		Movement();
		CurrentAnimation = &run; 
		break;

	case RightState:
		LOG("MOVING RIGHT");
		flip = false;
		Movement();
		CurrentAnimation = &run;
		break;

	case JumpState:
		
		EndJump = false;
		Gravity = GravitySave;
		CurrentAnimation = &jump;
		On_The_Ground();
		
		if (Jump_Ready == true) {
			MidAirUP = true;
			
			Jumping();
		}
		if (EndJump == true) {
			PlayerState = FallState;
			CurrentAnimation = &jump;
	    }

		
		LOG("JUMP STATE ACTIVE");
		break;

	case FallState:

		TouchingCollider = false;
		CurrentAnimation = &jump;
		
		break;

	case DoubleJumpState:

		LOG("DOUBLE JUMP STATE");
		EndJump = false;
		Gravity = GravitySave;
		CurrentAnimation = &jump;
		//On_The_Ground();
		
		if (Jump_Ready == true) {
			MidAirUP = true;
			LOG("GOING TO DOUBLE JUMP");
			Jumping();
		}
		if (EndJump == true) {
			PlayerState = IdleState;
			CurrentAnimation = &idle;
			LOG("DOUBLE JUMP TO IDLE");
		}
		

		break;

	case DashState:
		
		CanDash = false;
		CurrentAnimation = &dash;
		DashFunction();
		LOG("DASH");
		break;
	}


	Player_Rect.x = CurrentPosition.x;
	Player_Rect.y = CurrentPosition.y;

	Player_Collider->SetPos(CurrentPosition.x, CurrentPosition.y);

	SDL_Rect r = CurrentAnimation->GetCurrentFrame();

	App->render->Blit(Graphics, CurrentPosition.x, CurrentPosition.y, &r, flip);

	if (flip == false)
	{
		Player_Collider->SetPos(CurrentPosition.x, CurrentPosition.y); //Makes the collider follow the player.
	}
	else
	{
		Player_Collider->SetPos(CurrentPosition.x, CurrentPosition.y); //Makes the collider follow the player.
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
	
	 if (collider != nullptr)
	 {
		 collider->to_delete = true;
		 collider = nullptr;
	 }

	 return true;
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

			 if ((A->rect.x + A->rect.w - 5) <= (B->rect.x ))  //EL BO
			 { //Left to right
				 TouchingCollider = true;
				 Movement();
				 CurrentPosition.x = LastPosition.x - 1; 
				 LOG("PLAYER INTO WALL FROM THE LEFT");
				 
			 }

			 

			 //else if (B->rect.x - B->rect.w >= A->rect.x - 20)
			 //{ //Right to left
				// TouchingCollider = true;
				// Movement();
				// CurrentPosition.x = LastPosition.x;
				//LOG("PLAYER INTO WALL FROM THE RIGHT");
			 //}
		 }

		 //from above
		 if ((CurrentPosition.y + A->rect.h) <= B->rect.y + 20 ) // from above
		 {
			 if ((A->rect.x + A->rect.w > B->rect.x) || (A->rect.x + A->rect.w < B->rect.x + B->rect.w)) {

				 On_Ground = true;
				 CanJump = true;
				 CurrentPosition.y = LastPosition.y;
				 CurrentPosition.y = B->rect.y - A->rect.h + 1;
				 float Gravity2 = Gravity;

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

	 if (A->type == ObjectType::Player && B->type == ObjectType::Victory)
	 {
		 if (((CurrentPosition.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((CurrentPosition.y + A->rect.h) > B->rect.y))
		 {
			 App->fade->FadeToBlack("SimpleLevel2.tmx"); 
		 }
	 }
 }
 
 bool j1Player::InitPlayer()
 {

	 floor = CurrentPosition.y;
	 
	 CurrentPosition = { Inipos.x, Inipos.y };

	 Player_Rect = { CurrentPosition.x, CurrentPosition.y, Player_Width, Player_Height };

	 Player_Collider = App->collision->AddCollider(Player_Rect, ObjectType::Player /*this*/);

	 // On_Ground = true;
	

	 return true;
 }



 void j1Player::DashFunction(){

	 
		 MidAirUP = false;
		 Gravity = 0;
		 if (StartPosition.x - DashDist < CurrentPosition.x && DashActiveLeft == true && TouchingCollider == false) {

			 CurrentPosition.x -= Character_vel.x * 2;
			 //CurrentPosition.y -= 1; //MAGIC NUMBER UNA VEGADA ELS COLIDERS FUNCIONIN PERFECTAMENT NO FARA FALTA
			 LOG("DASH FUNCTION LEFT");

		 }
		 else if (StartPosition.x + DashDist > CurrentPosition.x && DashActiveRight == true && TouchingCollider == false) {

			 CurrentPosition.x += Character_vel.x * 2;
			 //CurrentPosition.y -= 1; //MAGIC NUMBER UNA VEGADA ELS COLIDERS FUNCIONIN PERFECTAMENT NO FARA FALTA

			 LOG("DASH FUNCTION RIGHT");

		 }
		 else {
			 DashActiveLeft = false;
			 DashActiveRight = false;
			 //MidAirUP = false;
			 Gravity = GravitySave;
			 PlayerState = FallState;

			 LOG("DASH  TO JUMP");
		 }
	 


 }


 void j1Player::Jumping() {


	 CanJump = false;

	 if (MidAirUP == true) {
		 LOG("MID AIR TRUE");
		 Character_vel.y -= Gravity * 0.75;



		 /*if (PlayerInput.U_active&&PlayerInput.A_active) {

			 DashActiveRight;
			 DashFunction();

		 }*/


		 if (PlayerInput.A_active) {
			 CurrentPosition.x -= 1.5*Character_vel.x;


		 }



		 if (PlayerInput.D_active) {
			 CurrentPosition.x += 1.5*Character_vel.x;

			 LOG("GOING RIGHT INSIDE JUMP");
		 }

		 /*if (PlayerInput.U_active && PlayerInput.D_active) {
			 LOG("GOING RIGHT AND DASHING RIGHT IN JUMP");

			 DashActiveLeft;
			 DashFunction();

		 }*/



		 if (Character_vel.y <= 0) {
			 float altura = CurrentPosition.y;
			 LOG("VELOCITY REACHED 0   %f", altura);
			 MidAirUP = false;
		 }

		 CurrentPosition.y -= Character_vel.y / 2;

	 }

	 if (MidAirUP == false) {
		 LOG("MID AIR UP == FALSE");
		 //Character_vel.y += Gravity;

		 //CurrentPosition.y += 1.5*Character_vel.y;

		 if (PlayerInput.D_active) {
			 FallLeft = true;
			 FallRight = false;

		 }
		 if (PlayerInput.A_active) {
			 FallRight = true;
			 FallLeft = false;
		 }


		 /*if (PlayerInput.U_active && PlayerInput.D_active) {

			 LOG("100");
			 DashActiveRight = true;
			 StartPosition.x = CurrentPosition.x;
			 PlayerState = DashState;
			 On_Ground = false;


		 }

		 else if (PlayerInput.U_active && PlayerInput.A_active) {
			 LOG("100");
			 DashActiveLeft = true;
			 StartPosition.x = CurrentPosition.x;
			 PlayerState = DashState;
			 On_Ground = false;
			 LOG("LEFT TO DASH LEFT");

		 }*/

		 if (On_Ground == true) {
			 LOG("TO IDLE FROM JUMP");


			 EndJump = true;
			 Character_vel.y = TempVelY;

		 }

		 else {

			 LOG("FALLING");
			 CurrentPosition.y += Character_vel.y;

			 ++FallingVel;
			 LOG("TIMES = %d", FallingVel);
		 }

	 }
 }