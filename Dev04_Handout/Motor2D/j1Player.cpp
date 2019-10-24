#include "p2Point.h"
#include "p2Animation.h"
#include "p2Log.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Audio.h"



j1Player::j1Player() 
{
	name.create("player");
}


 j1Player::~j1Player()
 {
 }

 bool j1Player::Awake(pugi::xml_node& node)
 {
	 CurrentPosition.y = floor;
	


	 bool ret = true;
	
	 pugi::xml_node player = node.child("player");

	 Inipos.x = node.attribute("inipos_x").as_float();
	 Inipos.y = node.attribute("inipos_y").as_float();
	 Character_vel = node.attribute("velocity").as_float();
	 Gravity = node.attribute("gravity").as_float();
	 DashDist = node.attribute("DashDistance").as_float();
	 
	 CurrentPosition = { Inipos.x, Inipos.y };

	 return ret;
 }


 bool  j1Player::Start() 
 {
	 LOG("Loading player textures");
	 
	 
	 Graphics = App->tex->Load("Sprites/Dude.png");
	 floor = CurrentPosition.y;
	 PlayerState = IdleState;
	 SlimeForm = false;


	
	
	
	 return true;
 }

bool j1Player::PreUpdate() 
 {

	PlayerInput.F10_active = App->input->keyboard[SDL_SCANCODE_F10] == KEY_DOWN;
	PlayerInput.F3_active = App->input->keyboard[SDL_SCANCODE_F3] == KEY_DOWN;
	PlayerInput.F11_active= App->input->keyboard[SDL_SCANCODE_F11] == KEY_DOWN;

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
		//PlayerInput.W_active = App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN;
		PlayerInput.U_active = App->input->keyboard[SDL_SCANCODE_U] == KEY_DOWN;
		PlayerInput.I_active = App->input->keyboard[SDL_SCANCODE_I] == KEY_DOWN;
		PlayerInput.Space_active = App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN;
		
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
	if (PlayerInput.F11_active) { //NI IDEA DE PERQUE NO VA

		App->render->camera.x = CurrentPosition.x;
		App->render->camera.y = CurrentPosition.y;
		float camx = App->render->camera.x;
		float camy = App->render->camera.y;
		LOG("CAM.X = %f   CAM.Y == %f", camx, camy);
	}

	//ROTACIO DELS PLAYER STATES 
	if (GOD_MODE == false) {
		if (PlayerState == IdleState)
		{
			if (SlimeForm == false) {
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



				if (PlayerInput.I_active) {

					PlayerState = SlimeState;
					LOG("IDLE (HUMAN) TO SLIME");
				}

				if (PlayerInput.Space_active) {

					On_The_Ground();


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
			else {

				if (PlayerInput.A_active) {

					PlayerState = LeftState;
					LOG("IDLE TO LEFT (SLIME)");

				}

				if (PlayerInput.D_active) {

					PlayerState = RightState;
					LOG("IDLE TO RIGHT (SLIME)");
				}

				if (PlayerInput.I_active) {


					PlayerState = SlimeState;
					LOG("IDLE (SLIME TO SLIME)");

				}
			}
		}


		if (PlayerState == LeftState)
		{
			if (SlimeForm == false) {
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

				

			}

			else {

				if (!PlayerInput.A_active)
				{

					PlayerState = IdleState;
					LOG("LEFT(SLIME) TO IDLE (SLIME)");
				}


			}


			if (PlayerInput.Space_active && SlimeForm == false) {

				On_The_Ground();


				if (On_Ground == true) {

					StartPosition.y = CurrentPosition.y;
					PlayerState = JumpState;
					LOG("LEFT TO JUMP");
				}

				if (On_Ground == false) {

					LOG("JUMP NOT AVAILABLE");
				}


			}

		}
		if (PlayerState == RightState)
		{
			if (SlimeForm == false) {
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

				
			}

			else {

				if (!PlayerInput.D_active)
				{
					PlayerState = IdleState;
					LOG("RIGHT (SLIME) TO IDLE (SLIME)");
				}

			}

			if (PlayerInput.Space_active && SlimeForm == false) {

				On_The_Ground();


				if (On_Ground == true) {

					StartPosition.y = CurrentPosition.y;
					PlayerState = JumpState;
					LOG("LEFT TO JUMP");
				}

				if (On_Ground == false) {

					LOG("JUMP NOT AVAILABLE");
				}


			}



		}

		if (PlayerState == SlimeState) {
			SlimeForm;
			if (SlimeForm == false) { //HUMAN TO SLIME
				//CANVIAR SPRITE AL DE SLIME 
				//ANIMACIO DE TRANSFORMACIO
				SlimeForm = true;
				//if(animation  to slime transformation finished) -> playerstate= Idle 
				LOG("TRANSFORMING INTO SLIME");
				PlayerState = IdleState; // Aixo no funciona be per ara perque com que no hi han animacions canvia al instant

			}

			else if (SlimeForm == true) { // SLIME TO HUMAN

				//CANVIAR SPRITE AL NORMAL
				//ANIMACIO TRANSFORMACIO
				SlimeForm = false;
				//if(animation to human transformation finished) -> playerstate= Idle 
				LOG("TRANSFORMING INTO HUMAN");
				PlayerState = IdleState;
			}

		}
	}

	else {


		if (PlayerInput.AG_active) {
			CurrentPosition.x -= Character_vel*2;

		}

		if (PlayerInput.DG_active) {
			CurrentPosition.x += Character_vel*2;

		}

		if (PlayerInput.WG_active) {
			CurrentPosition.y -= Character_vel*2;

		}

		if (PlayerInput.SG_active) {
			CurrentPosition.y += Character_vel*2;

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
		//LOG("IDLE");
		CurrentAnimation = &idle;
		break;

	case LeftState:
		LOG("MOVING LEFT");
		CurrentPosition.x -= Character_vel;
		CurrentPosition.y;
		break;

	case RightState:
		LOG("MOVING RIGHT");
		CurrentPosition.x += Character_vel;
		CurrentPosition.y;
		break;
	case JumpState:
		
		Jumping();
		LOG("JUMP STATE ACTIVE");
		break;
	case DashState:
		
		//Mix_PlayChannel(-1,Jump_Sound , 0);
		DashFunction();
		LOG("DASH");
		break;
	}

	

	//SDL_Rect r = CurrentAnimation->GetCurrentFrame();
	
	//App->render->Blit(Graphics, CurrentPosition.x, CurrentPosition.y, &r, 1.0f, true);


	Player_Rect = { CurrentPosition.x, CurrentPosition.y, 50, 60 };

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
	 pos.append_attribute("y") =CurrentPosition.y;
	 return true;
 }