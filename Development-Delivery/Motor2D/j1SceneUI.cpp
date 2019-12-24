#include "j1App.h"
#include "j1SceneUI.h"
#include "j1Scene.h"
#include "j1MenuScene.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1ElementUI.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1LabelUI.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1EntityManager.h"
#include "j1FadeToBlack.h"

#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

j1Scene_UI::j1Scene_UI() : j1Module()
{
	name.create("scene_UI");
}

// Destructor
j1Scene_UI::~j1Scene_UI()
{}

// Called before render is available
bool j1Scene_UI::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene UI");
	bool ret = true;
	path.create(config.child("path").child_value());
	return ret;
}

// Called before the first frame
bool j1Scene_UI::Start()
{
	bool ret = true;

	

	
	App->gui->CreateLabel({ 380, 5 }, "time", Label_Type::FONT, { 255,255,255,255 }, true);
	timer_label = App->gui->CreateLabel({ 465,5 }, timer_string, Label_Type::CONFIG, { 255,255,255,255 }, true);
	//BACKPLAYER SQUARE
   // Character_Back[0] = (App->gui->CreateSprite({ 113,4 }, { 264,37,35,50 }, true));
	Map_Icon[0] = (App->gui->CreateSprite({ 900,4 }, { 200,34,52,50 }, true));
	
	//HEALTH START

	lifes[0] = (App->gui->CreateSprite({ 5,10 }, { 458,45,110,34 }, true));
	player_status[0] = (App->gui->CreateSprite({ 120,10 }, { 413,45,20,35 }, true));
	
	
	LifeAt1 = true;
	LifeAt2 = true;
	LifeAt3 = true;
	LifeAt4 = true;


	//COLLECTABLE

	
	score_label = App->gui->CreateLabel({ 870,10 }, "x0", Label_Type::CONFIG, { 255,255,255,255 }, true);
	


	//BUTTONS MENU INGAME TESTING

	SDL_Rect ButtonsStates[3] = { {80,30,100,100},{80,245,100,100},{80,140,100,100} };
	SDL_Rect ButtonLoad[2] = { {225,290,105,110},{225,290,105,110} };
	SDL_Rect ButtonSave[2] = { {335,290,110,120},{335,300,110,120} };
	SDL_Rect ButtonClose[1] = { 290,210,70,70 };
	SDL_Rect ButtonMuted[1] = { 460,275,100,90 };
	SDL_Rect ButtonNOTMuted[1] = { 460,388,100,90 };
	SDL_Rect ButtonToMenu[1] = {230,415,100,100};
	

	//MENU INGAME TEXT AND OTHER THINGS

	
	Menu_Listed_Ingame.add(App->gui->CreateSprite({ 300,0 }, { 590,10,460,465 }, true));
	Menu_Listed_Ingame.add(App->gui->CreateSprite({ 400,0 }, { 690,480,280,65 }, true));
	
	Menu_Listed_Ingame.add(App->gui->CreateLabel({ 485,10 }, "PAUSED", Label_Type::CONFIG, { 255,255,255,255 }, true));

	Buttons_Listed_Ingame.add(App->gui->CreateButton({400,75 }, Button_Type::LOAD, ButtonLoad[0], &ButtonLoad[1], &ButtonLoad[1], "Loading Button", true));
	Buttons_Listed_Ingame.add(App->gui->CreateButton({ 575,75 }, Button_Type::SAVE, ButtonSave[0], &ButtonSave[1], &ButtonSave[1], "Saving Butoon", true));
	Buttons_Listed_Ingame.add(App->gui->CreateButton({ 700,0 }, Button_Type::CLOSE, ButtonClose[0], &ButtonClose[0], &ButtonClose[0], "Close Menu", true));
	Buttons_Listed_Ingame.add(App->gui->CreateButton({ 350,325 }, Button_Type::MUTE, ButtonMuted[0], &ButtonMuted[0], &ButtonMuted[0], "Mute ON Button", true));
	Buttons_Listed_Ingame.add(App->gui->CreateButton({ 350,225 }, Button_Type::UNMUTE, ButtonNOTMuted[0], &ButtonNOTMuted[0], &ButtonNOTMuted[0], "Mute OFF Button", true));
	Buttons_Listed_Ingame.add(App->gui->CreateButton({ 600,275 }, Button_Type::MENU, ButtonToMenu[0], &ButtonToMenu[0], &ButtonToMenu[0], "To Menu", true));


	p2List_item<UIitem_Button*>* button_item = Buttons_Listed_Ingame.start;
	while (button_item != NULL)
	{
		button_item->data->visible = false;
		button_item = button_item->next;
	}
	p2List_item<UI_Item*>* ui_item = Menu_Listed_Ingame.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = false;
		ui_item = ui_item->next;
	}

	return ret;
}

// Called each loop iteration
bool j1Scene_UI::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool j1Scene_UI::Update(float dt)
{
	if (App->entityManager->player != NULL) {

		BROFILER_CATEGORY("Update_SceneUI", Profiler::Color::OrangeRed);
		float timer = (float)ptimer.ReadMs() / 1000;
		sprintf_s(timer_string, 20, "%.2f", timer);
		timer_label->ChangeText(timer_string);

		//COLLECTABLE MAPS
		if (App->entityManager->player->Maps_Num == 0) {
			score_label->ChangeText("x0");
		}
		else if (App->entityManager->player->Maps_Num == 1) {
			score_label->ChangeText("x1");
		}
		else if (App->entityManager->player->Maps_Num == 2) {
			score_label->ChangeText("x2");
		}
		else if (App->entityManager->player->Maps_Num == 3) {
			score_label->ChangeText("x3");
		}
		else if (App->entityManager->player->Maps_Num == 4) {
			score_label->ChangeText("x4");
		}


		//HEALTH 
		//Not sure how to change sprite yet



		if (App->entityManager->player->Life_Num == 4 && LifeAt4 == false) {
			lifes[0] = (App->gui->CreateSprite({ 5,10 }, { 458,45,110,34 }, true));
			player_status[0] = (App->gui->CreateSprite({ 120,10 }, { 413,45,20,35 }, true));
		}
		else if (App->entityManager->player->Life_Num == 3 && LifeAt3 == false) {
			lifes[1] = (App->gui->CreateSprite({ 5,10 }, { 458,80,110,34 }, true));
			player_status[1] = (App->gui->CreateSprite({ 120,10 }, { 376,45,20,35 }, true));
		}
		else if (App->entityManager->player->Life_Num == 2 && LifeAt2 == false) {
			lifes[2] = (App->gui->CreateSprite({ 5,10 }, { 458,111,110,34 }, true));
			player_status[2] = (App->gui->CreateSprite({ 120,10 }, { 339,45,20,35 }, true));
		}
		else if (App->entityManager->player->Life_Num == 1 && LifeAt1 == false) {
			lifes[3] = (App->gui->CreateSprite({ 5,10 }, { 458,144,110,34 }, true));
			player_status[3] = (App->gui->CreateSprite({ 120,10 }, { 308,45,20,35 }, true));
		}



	}




	p2List_item<UIitem_Button*>* ActionButtons = Buttons_Listed_Ingame.start;
	while (ActionButtons != NULL)
	{
		if (ActionButtons->data->OnClick())
		{
			switch (ActionButtons->data->GetType())
			{
			case PLAY:
				
				break;
			case SAVE:
				App->SaveGame("save_game.xml");
				LOG("SAVE BUTTON");
				break;
			case LOAD:
				App->LoadGame();
				LOG("LOAD BUTTON");
				break;
			case EXIT:
				
				break;

			case MENU:
				LOG("TO MENU BUTTON");
				break;

			case MUTE:
				LOG("MUTE BUTTON");
				App->audio->TempVolumeMusicValue = App->audio->VolumeMusicValue;
				App->audio->VolumeMusicValue = 0;
				App->audio->VolumeMusic();
				break;

			case UNMUTE:
				LOG("UNMUTE BUTTON");
				App->audio->VolumeMusicValue= 100;
				App->audio->VolumeMusic();
				break;

			case CLOSE:
				IngameMenu();
				break;
			}
		}
		ActionButtons = ActionButtons->next;
	}

	return true;
}

// Called each loop iteration
bool j1Scene_UI::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneIntro", Profiler::Color::CadetBlue);

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		IngameMenu();
	}

	return ret;
}

// Called before quitting
bool j1Scene_UI::CleanUp()
{
	LOG("Freeing Scene Menu");
	Buttons_Listed_Ingame.clear();
	App->gui->Disable();
	return true;
}

void j1Scene_UI::IngameMenu()
{
	p2List_item<UIitem_Button*>* button_item = Buttons_Listed_Ingame.start;
	while (button_item != NULL)
	{
		button_item->data->visible = !button_item->data->visible;
		button_item = button_item->next;
	}
	p2List_item<UI_Item*>* ui_item = Menu_Listed_Ingame.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = !ui_item->data->visible;
		ui_item = ui_item->next;
	}
}

