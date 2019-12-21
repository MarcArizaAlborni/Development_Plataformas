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

	App->gui->CreateSprite({ 0,0 }, { 649,203,451,24 }, true);

	
	App->gui->CreateLabel({ 380, 5 }, "time", Label_Type::FONT, { 255,255,255,255 }, true);
	timer_label = App->gui->CreateLabel({ 465,5 }, timer_string, Label_Type::CONFIG, { 255,255,255,255 }, true);
	//BACKPLAYER SQUARE
    Character_Back[0] = (App->gui->CreateSprite({ 113,4 }, { 264,37,35,50 }, true));
	Map_Icon[0] = (App->gui->CreateSprite({ 900,4 }, { 200,34,52,50 }, true));

	//HEALTH
	if (App->entityManager->player->Life_Num == 4) {
		lifes[0] = (App->gui->CreateSprite({ 5,10 }, { 458,45,110,34 }, true));
		player_status[0] = (App->gui->CreateSprite({ 120,10 }, { 413,45,20,35 }, true));
	}
	else if (App->entityManager->player->Life_Num == 3){
		lifes[1] = (App->gui->CreateSprite({ 5,10 }, { 458,80,110,34 }, true)); 
		player_status[1] = (App->gui->CreateSprite({ 120,10 }, { 376,45,20,35 }, true));
	}
	else if (App->entityManager->player->Life_Num == 2) {
		lifes[2] = (App->gui->CreateSprite({ 5,10}, { 458,111,110,34 }, true));
		player_status[2] = (App->gui->CreateSprite({ 120,10 }, { 339,45,20,35 }, true));
	}
	else if (App->entityManager->player->Life_Num == 1) {
		lifes[3] = (App->gui->CreateSprite({ 5,10 }, { 458,144,110,34 }, true));
		player_status[3] = (App->gui->CreateSprite({ 120,10 }, { 308,45,20,35 }, true));
	}

	//COLLECTABLE

	
	score_label = App->gui->CreateLabel({ 870,10 }, "x0", Label_Type::CONFIG, { 255,255,255,255 }, true);
	
	

	p2List_item<UIitem_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		button_item->data->visible = false;
		button_item = button_item->next;
	}
	p2List_item<UI_Item*>* ui_item = pause_ui_list.start;
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
	BROFILER_CATEGORY("Update_SceneUI", Profiler::Color::DarkKhaki);
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


	p2List_item<UIitem_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		if (button_item->data->OnClick())
		{
			switch (button_item->data->GetType())
			{
			case PLAY:
				ChangeVisibility();
				//App->paused = false;
				break;
			case SAVE:
				//App->SaveGame("save_game.xml");
				break;
			case LOAD:
				//App->LoadGame("save_game.xml");
				break;
			case EXIT:
				//App->fadescene->FadeToBlack2(App->scene, App->scene_menu);
				break;
			}
		}
		button_item = button_item->next;
	}

	return true;
}

// Called each loop iteration
bool j1Scene_UI::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneIntro", Profiler::Color::MediumVioletRed);

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ChangeVisibility();
	}

	return ret;
}

// Called before quitting
bool j1Scene_UI::CleanUp()
{
	LOG("Freeing Scene Menu");
	button_list.clear();
	App->gui->Disable();
	return true;
}

void j1Scene_UI::ChangeVisibility()
{
	p2List_item<UIitem_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		button_item->data->visible = !button_item->data->visible;
		button_item = button_item->next;
	}
	p2List_item<UI_Item*>* ui_item = pause_ui_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = !ui_item->data->visible;
		ui_item = ui_item->next;
	}
}

