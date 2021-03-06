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
#include "j1Skeleton.h"
#include "j1Skull.h"
#include "j1Bee.h"
#include "j1SceneUI.h"

#include "Brofiler/Brofiler.h"

j1Player::j1Player(iPoint pos, EntitiesType type) : j1Entities(pos, EntitiesType::PLAYER )
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

	
	dash.PushBack({ 172,131,24,43 });
	dash.PushBack({ 208,131,31,43 });
	
	dash.speed = 0.01;
}


 j1Player::~j1Player()
 {
 }

 bool  j1Player::Start() 
 {
	 BROFILER_CATEGORY("Player Start()", Profiler::Color::PapayaWhip)
	 LOG("Loading player textures");
	 
	 texture = App->tex->Load("Sprites/DudeMOD.png");
	 Life_Num = 4;
	 Invulnerability_Cap = 100;
	 Maps_Num = 0;

	 InitEntity();
	
	 return true;
 }

 bool j1Player::PreUpdate()
 {  

	 BROFILER_CATEGORY("Player PreUpdate()", Profiler::Color::Teal)

		 if ((App->scene_ui->OnMainMenu !=true) && (App->scene_ui->OnCreditsMenu != true) && (App->scene_ui->OnSettingsMenu != true)) {
			 On_Ground;
			 if (On_Ground == false) {

				 Falling = true;
			 }

			 if (Falling == true)
			 {

				 position.y += Gravity;
			 }
			 PlayerInput.F10_active = App->input->keyboard[SDL_SCANCODE_F10] == KEY_DOWN;
			 PlayerInput.F3_active = App->input->keyboard[SDL_SCANCODE_F3] == KEY_DOWN;

			 if (PlayerInput.F10_active && GOD_MODE == false) {
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

			 else {

				 PlayerInput.AG_active = App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT;
				 PlayerInput.DG_active = App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT;
				 PlayerInput.WG_active = App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT;
				 PlayerInput.SG_active = App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT;
				 collider->to_delete = true;
				 Gravity = 0;

			 }
			 //AGAFAR INPUTS I TAL, HE MIRAT I CREC QUE FERHO AIXI ES MILLOR QUE TOT JUNT 

			//START CURRENT LEVEL
			 if (PlayerInput.F3_active) {

				 position.x = Inipos.x;
				 position.y = Inipos.y;
			 }


			 //ROTACIO DELS PLAYER STATES 
			 if (GOD_MODE == false) {
				 if (state == IdleState)
				 {


					 if (PlayerInput.A_active)
					 {
						 state = LeftState;
						 //LOG("IDLE TO LEFT");
					 }

					 if (PlayerInput.D_active)
					 {
						 state = RightState;
						 //LOG("IDLE TO RIGHT");
					 }

					 if (PlayerInput.Space_active && CanJump == true && JumpTicks == true) {
						 JumpTicks = false;
						 CanJump = false;

						 state = JumpState;
						 LOG("IDLE TO JUMP");


						 if (On_Ground == false) {
							 state = FallState;
							 LOG("JUMP NOT AVAILABLE");
						 }

					 }

					 if (PlayerInput.Space_active && CanJump == true) {

						 On_Ground = false;
						 CanJump = false;
						 Character_vel.y = 50;
						 state = JumpState;

					 }

				 }

				 if (state == LeftState)
				 {

					 if (!PlayerInput.A_active)
					 {

						 state = IdleState;
						 //LOG("LEFT TO IDLE");
					 }

					 if (PlayerInput.U_active && CanDash == true && DashedBefore == false) {
						 DashActiveLeft = true;
						 DashedBefore = true;
						 StartPosition.x = position.x;
						 state = DashState;
						 LOG("LEFT TO DASH LEFT");
					 }

					 if (PlayerInput.Space_active && CanJump == true) {

						 On_Ground = false;
						 state = JumpState;
						 LOG("LEFT TO JUMP");

					 }

				 }
				 if (state == RightState)
				 {

					 if (!PlayerInput.D_active)
					 {
						 state = IdleState;
						 //LOG("RIGHT TO IDLE");
					 }

					 if (PlayerInput.U_active && CanDash == true && DashedBefore == false) {
						 DashActiveRight = true;
						 DashedBefore = true;
						 StartPosition.x = position.x;
						 state = DashState;
						 LOG("LEFT TO DASH RIGHT");
					 }

					 if (PlayerInput.Space_active && CanJump == true) {
						 //Character_vel.y = 50;
						 state = JumpState;
						 LOG("LEFT TO JUMP");


					 }

				 }
				 if (state == FallState) {


					 //LOG("FALL STATE");
					 Movement();

					 if (PlayerInput.U_active && PlayerInput.D_active&& DashedBefore == false) {
						 DashActiveRight = true;
						 DashedBefore = true;
						 StartPosition.x = position.x;
						 state = DashState;
					 }
					 else if (PlayerInput.U_active && PlayerInput.A_active&& DashedBefore == false) {
						 DashActiveLeft = true;
						 DashedBefore = true;
						 StartPosition.x = position.x;
						 state = DashState;
					 }

					 //PlayerState = IdleState;
					 if (CanJump == true) {
						 LOG("22");
						 TouchingCollider = true;
						 state = IdleState;
					 }

				 }

			 }

			 else {


				 if (PlayerInput.AG_active) {
					 position.x -= Character_vel.x;

				 }

				 if (PlayerInput.DG_active) {
					 position.x += Character_vel.x;

				 }

				 if (PlayerInput.WG_active) {
					 position.y -= Character_vel.x;

				 }

				 if (PlayerInput.SG_active) {
					 position.y += Character_vel.x;

				 }
			 }
		 }
	 return true ;
}


bool j1Player::Update(float dt)
 {
	BROFILER_CATEGORY("Player Update()", Profiler::Color::PowderBlue)
	LastPosition = position;
	bool ret = true;
	//APLICACIO DELS DIFFERENTS PLAYER STATES
	switch (state)
	{
	case IdleState:
		//LOG("IDLE");
		TouchingCollider = false;
		//CurrentPosition.y -= Gravity;
		if (EndJump == true) {
			animation = &idle;

		}
		else {
			animation = &jump;
		}
		CanDash = true;
		JumpTicks = true;
		//LOG("IDLE STATE");
		break;

	case LeftState:
		//LOG("MOVING LEFT");
		flip = true;
		Movement();
		animation = &run;
		break;

	case RightState:
		//LOG("MOVING RIGHT");
		flip = false;
		Movement();
		animation = &run;
		break;

	case JumpState:
		
		EndJump = false;
		Gravity = GravitySave;
		animation = &jump;
		On_The_Ground();
		
		if (Jump_Ready == true) {
			MidAirUP = true;
			
			Jumping();
		}
		if (EndJump == true) {
			state = FallState;
			animation = &jump;
	    }

		
		LOG("JUMP STATE ACTIVE");
		break;

	case FallState:

		TouchingCollider = false;
		animation = &jump;
		
		break;


	case DashState:
		
		CanDash = false;
		animation = &dash;
		DashFunction();
		LOG("DASH");
		break;
	}


	Player_Rect.x = position.x;
	Player_Rect.y = position.y;

	
	SDL_Rect r = animation->GetCurrentFrame();

	if (flip == false)
	{
		BlitEntities(r, flip, position.x, position.y);
		collider->SetPos(position.x, position.y); //Makes the collider follow the player.
	}
	else
	{
		BlitEntities(r, flip, position.x, position.y);
		collider->SetPos(position.x+8, position.y); //Makes the collider follow the player.
	}

	return true;
}


 bool j1Player::PostUpdate()
 {
	 BROFILER_CATEGORY("Player PostUpdate()", Profiler::Color::Ivory)
	 return true;
 }

 bool j1Player::CleanUp()
 {
	 BROFILER_CATEGORY("Player CleanUp()", Profiler::Color::DimGray)
	 LOG("Unloading Player");

	 App->tex->UnLoad(texture);
	
	 if (collider != nullptr)
	 {
		 collider->to_delete = true;
		 collider = nullptr;
	 }

	 return true;
 }

 bool j1Player::Load(pugi::xml_node& data)
 {
	 position.x = data.child("position1").attribute("x").as_int();
	 position.y = data.child("position1").attribute("y").as_int();
	 return true;
 }


 bool j1Player::Save(pugi::xml_node&  data) const
 {
	 pugi::xml_node pos = data.append_child("position1");

	 pos.append_attribute("x") = position.x;
	 pos.append_attribute("y") = position.y;
	 return true;
 }


 void j1Player::OnCollision(Collider* A, Collider* B) 
 {
	 BROFILER_CATEGORY("Player OnCollision()", Profiler::Color::Wheat)

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
			 position.y = B->rect.y + B->rect.h;
		 }

		
		 if (B->type == ObjectType::Skeleton) {

			 if (App->entityManager->skeleton->state == AttackReadyState) {

				 if (A->rect.x + A->rect.w >= B->rect.x && A->rect.x <= B->rect.x)  //EL BO
				 {

					 position.x = Inipos.x;
					 position.y = Inipos.y;

				 }
				 //Right to Left
				 else if (A->rect.x <= B->rect.x + B->rect.w && A->rect.x + A->rect.w >= B->rect.x + B->rect.w) {

					 position.x = Inipos.x;
					 position.y = Inipos.y;
				 }

			 }

		 }

		 if (B->type == ObjectType::Skull) {
			 if ((position.y + A->rect.h) <= B->rect.y + 20) {

				 position.x = Inipos.x;
				 position.y = Inipos.y;
			 }
		 }


		 //from above
		 if ((position.y + A->rect.h) <= B->rect.y + 20) // from above
		 {
			 if ((A->rect.x + A->rect.w > B->rect.x) || (A->rect.x + A->rect.w < B->rect.x + B->rect.w)) {

				 On_Ground = true;
				 CanJump = true;
				 position.y = LastPosition.y;
				 position.y = B->rect.y - A->rect.h -5;
				 float Gravity2 = Gravity;
				 DashedBefore = false;

			 }
		 }
	 }

	 if (A->type == ObjectType::Player && B->type == ObjectType::LateralPlatform) {
		 if (((position.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((position.y + A->rect.h) > B->rect.y))
		 {
			 //Left to right
			 if (A->rect.x + A->rect.w >= B->rect.x && A->rect.x <= B->rect.x)  //EL BO
			 {

				 TouchingCollider = true;
				 Movement();
				 position.x = LastPosition.x;
				 LOG("PLAYER INTO WALL FROM THE LEFT");

			 }
			 //Right to Left
			 else if (A->rect.x <= B->rect.x + B->rect.w && A->rect.x + A->rect.w >= B->rect.x + B->rect.w) {

				 TouchingCollider = true;
				 Movement();
				 position.x = LastPosition.x;
				 LOG("PLAYER INTO WALL FROM THE RIGHT");
			 }
		 }
	 }

	 if (GOD_MODE == false) {
		 if (A->type == ObjectType::Player && B->type == ObjectType::Water) {

			 if ((position.y + A->rect.h) <= B->rect.y + 20) // from above
			 {
				 if (App->scene_ui->PlayerInvencibility == true) {
					 App->scene_ui->HealthChanged = true;
					 App->scene_ui->ResetEntitiesPositionNewGame = true;
					 App->entityManager->player->position.x = 90.0f;
					 App->entityManager->player->position.y = 250.0f;
					 if (App->scene_ui->LifeAt4 == true) {
						 LOG("4 TO 3");
						 App->scene_ui->LifeAt3 = true;
						 App->scene_ui->LifeAt4 = false;
						 App->scene_ui->PlayerInvencibility = false;
						 //App->scene_ui->HealthChanged = false;
					 }
					 else if (App->scene_ui->LifeAt3 == true) {
						 LOG("3 TO 2");
						 App->scene_ui->LifeAt2 = true;
						 App->scene_ui->LifeAt3 = false;
						 App->scene_ui->PlayerInvencibility = false;
						 //App->scene_ui->HealthChanged = false;
					 }
					 else if (App->scene_ui->LifeAt2 == true) {
						 LOG("2 TO 1");
						 App->scene_ui->LifeAt1 = true;
						 App->scene_ui->LifeAt2 = false;
						 App->scene_ui->PlayerInvencibility = false;
						 //App->scene_ui->HealthChanged = false;
					 }
					 else {
						 LOG("1 TO 0");
						 App->scene_ui->LifeAt0 = true;
						 App->scene_ui->LifeAt1 = false;
						 //App->scene_ui->HealthChanged = false;

					 }
				 }
			 }
		 }
	 }

	 if (A->type == ObjectType::Player && B->type == ObjectType::Victory)
	 {
		 if (((position.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((position.y + A->rect.h) > B->rect.y))
		 {
			 App->fade->FadeToBlack("SimpleLevel2.tmx");
		 }
	 }


	 if (A->type == ObjectType::Player && B->type == ObjectType::Teleporter1) {

		 if (((position.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((position.y + A->rect.h) > B->rect.y)) {

			 if (PlayerInput.I_active) {
				 //POTSER POSAR UN INPUT AQUI PER ACTIVAR EL TELEPORT?
				 B->rect.x; //COM AGAFO LA POSICIO DEL RECT DEL TELEPORTER 2 PERQUE EL RECT.X ES LA DEL TELEPORTER 1 (EMITER) NO LA DEL RECEIVER
				 B->rect.y;
				 // PlayerTP(B->rect.x,B->rect.y);
				 //POTSER POSAR COORDENADES EN EL CONFIG.XML I DIR TELEPORTER POSITION.X?
				 PlayerTP(179, 88); //AMB AIXO FUNCIONA PERO ESTA BRUTALMENT HARDCODEJAT
			 }
		 }
	 }


	 if (A->type == ObjectType::Player && B->type == ObjectType::CheckPoint) {

		 if (((position.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((position.y + A->rect.h) > B->rect.y)) {
			 App->SaveGame("save_game.xml");
			 LOG("SAVING CHECKPOINT");
		 }
	 }

	 if (A->type == ObjectType::Player && B->type == ObjectType::AudioArea1) {


		 if (((position.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((position.y + A->rect.h) > B->rect.y)) {

			 if (App->audio->Area1Level1 != true) {
				 Mix_HaltMusic();
				 App->audio->EnemyMusic = false;
				 App->audio->Area2Level1 = false;
				 App->audio->Area1Level1 = true;
				 App->audio->PlayMusic("audio/Music/L1_A1.ogg");

				 //ALTRE
			 }
		 }
	 }


	 if (A->type == ObjectType::Player && B->type == ObjectType::AudioArea2) {



		 if (((position.y + A->rect.h) < (B->rect.y + B->rect.h)) || ((position.y + A->rect.h) > B->rect.y)) {

			 if (App->audio->Area2Level1 != true) {
				 Mix_HaltMusic();
				 App->audio->EnemyMusic = false;
				 App->audio->Area1Level1 = false;
				 App->audio->Area2Level1 = true;
				 App->audio->PlayMusic("audio/Music/L1_A2.ogg");
			 }
		 }

	 }

 }
 
 bool j1Player::InitEntity()
 {
	 pugi::xml_document config;
	 config.load_file("config.xml");

	pugi::xml_node player = config.child("config").child("entities").child("player");

	//POSSAR EN EL XML
	 Inipos.x = player.attribute("inipos_x").as_float();
	 Inipos.y = player.attribute("inipos_y").as_float();;
	 Character_vel.x = player.attribute("velocity_x").as_int();
	 TempVelY = Character_vel.y = player.attribute("velocity_y").as_int();
	 GravitySave = Gravity = player.attribute("gravity").as_int();
	 DashDist = player.attribute("DashDistance").as_int();
	 Player_Width = player.attribute("width").as_int();
	 Player_Height = player.attribute("height").as_int();
	 DashedBefore = player.child("DashedBefore").attribute("active").as_bool();

	 // ESTA BE PERO TAMBE ES POT POSSAR
	 CanDash = player.child("CanDash").attribute("active").as_bool();
	 CanJump = player.child("CanJump").attribute("active").as_bool();
	 JumpTicks = player.child("JumpTicks").attribute("active").as_bool();
	 state = IdleState;
	 animation = &idle;
	 floor = position.y;
	 position = { Inipos.x, Inipos.y };
	 Player_Rect = { position.x, position.y, Player_Width, Player_Height };
	 collider = App->collision->AddCollider(Player_Rect, ObjectType::Player, App->entityManager); 
	 // On_Ground = true;
	

	 return true;
 }



 void j1Player::DashFunction(){

	 BROFILER_CATEGORY("Player Dash()", Profiler::Color::YellowGreen)
		 MidAirUP = false;
		 Gravity = 0;
		
			 if (StartPosition.x - DashDist < position.x && DashActiveLeft == true && TouchingCollider == false) {

				 position.x -= Character_vel.x * 2;
				 //CurrentPosition.y -= 1; //MAGIC NUMBER UNA VEGADA ELS COLIDERS FUNCIONIN PERFECTAMENT NO FARA FALTA
				 LOG("DASH FUNCTION LEFT");

			 }
			 else if (StartPosition.x + DashDist > position.x && DashActiveRight == true && TouchingCollider == false) {

				 position.x += Character_vel.x * 2;
				 //CurrentPosition.y -= 1; //MAGIC NUMBER UNA VEGADA ELS COLIDERS FUNCIONIN PERFECTAMENT NO FARA FALTA

				 LOG("DASH FUNCTION RIGHT");

			 }
			 else {
				 DashActiveLeft = false;
				 DashActiveRight = false;
				 //MidAirUP = false;
				 Gravity = GravitySave;
				 state = FallState;

				 LOG("DASH  TO JUMP");
			 }

		 

 }


 void j1Player::Jumping() {

	 BROFILER_CATEGORY("Player Jumping()", Profiler::Color::Yellow)
	 CanJump = false;

	 if (MidAirUP == true) {
		 LOG("MID AIR TRUE");
		 Character_vel.y -= Gravity * 0.75;



		

		 if (PlayerInput.A_active) {
			 position.x -= 1.5*Character_vel.x;


		 }



		 if (PlayerInput.D_active) {
			 position.x += 1.5*Character_vel.x;

			 LOG("GOING RIGHT INSIDE JUMP");
		 }

		 



		 if (Character_vel.y <= 0) {
			 float altura = position.y;
			 LOG("VELOCITY REACHED 0   %f", altura);
			 MidAirUP = false;
		 }

		 position.y -= Character_vel.y / 2;

	 }

	 if (MidAirUP == false) {
		 LOG("MID AIR UP == FALSE");
		

		 if (PlayerInput.D_active) {
			 FallLeft = true;
			 FallRight = false;

		 }
		 if (PlayerInput.A_active) {
			 FallRight = true;
			 FallLeft = false;
		 }


		 

		 if (On_Ground == true) {
			 LOG("TO IDLE FROM JUMP");


			 EndJump = true;
			 Character_vel.y = TempVelY;

		 }

		 else {

			 LOG("FALLING");
			 position.y += Character_vel.y;

			 ++FallingVel;
			 LOG("TIMES = %d", FallingVel);
		 }

	 }
 }



 void j1Player::PlayerTP(int TPposx, int TPposy) {

	 position.x = TPposx;
	 position.y = TPposy;

 }

 void j1Player::Movement() {

	 if (PlayerInput.A_active && TouchingCollider == false) {
		 position.x -= Character_vel.x;

	 }

	 if (PlayerInput.D_active&&TouchingCollider == false) {
		 position.x += Character_vel.x;

	 }
	 
 }