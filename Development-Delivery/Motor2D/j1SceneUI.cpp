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
#include "j1MapEntity.h"


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
	OnMainMenu = true;
	OnIngameMenu = false;
	OnIngameUI = false;
	ResetEntitiesPositionNewGame = false;
	bMuteSettingsON = false;
	bMuteSettingsOFF = true;
	bMuteIngameON = false;
	bMuteIngameOFF = true;
	
	return ret;
}

// Called before the first frame
bool j1Scene_UI::Start()
{
	bool ret = true;

	//MAIN MENU----------------------------------------------------------------------------------------------------------------------------------------------------
	MainMenu_UI.add(App->gui->CreateSprite({ -1,-1 }, { 36,866,1037,479 }, true));//BACKGROUND MAIN MENU
	SDL_Rect PlayButtonMenu[3] = { { 1030,479,165,76 }, { 1030,479,165,76 }, { 1030,479,165,76 } };
	SDL_Rect SettingsButtonMenu[3] = { {278,111,100,90},{278,111,100,90},{278,111,100,90} };
	SDL_Rect LinkButtonMenu[3] = { {191,135,64,66}, {191,135,64,66}, {191,135,64,66} };
	SDL_Rect ExitButtonMenu[3] = { {219,213,65,67}, { 219,213,65,67 }, { 219,213,65,67 } };
	SDL_Rect CreditsButtonOpen[3] = { {290,210,70,70},{290,210,70,70},{290,210,70,70} };
	MainMenu_UI.add(App->gui->CreateSprite({ 250,5 }, { 1070,10,530,465 }, true));
	MainMenu_UI.add(App->gui->CreateSprite({ 375,0 }, { 690,480,280,65 }, true));
	MainMenu_Buttons.add(App->gui->CreateButton({ 325,100 }, Button_Type::START_PLAY, PlayButtonMenu[0], &PlayButtonMenu[1], &PlayButtonMenu[1], "Play Button Menu", true));
	MainMenu_Buttons.add(App->gui->CreateButton({ 550,100 }, Button_Type::START_CONTINUE, PlayButtonMenu[0], &PlayButtonMenu[1], &PlayButtonMenu[1], "Continue Button Menu", true));
	MainMenu_Buttons.add(App->gui->CreateButton({ 590,250 }, Button_Type::START_SETTINGS, SettingsButtonMenu[0], &SettingsButtonMenu[1], &SettingsButtonMenu[1], "Settings Button Menu", true));
	MainMenu_Buttons.add(App->gui->CreateButton({ 400,350 }, Button_Type::EXIT, ExitButtonMenu[0], &ExitButtonMenu[1], &ExitButtonMenu[1], "Exit Game Button Menu", true));
	MainMenu_Buttons.add(App->gui->CreateButton({ 300,350 }, Button_Type::LINK, LinkButtonMenu[0], &LinkButtonMenu[1], &LinkButtonMenu[1], "Link Button Menu", true));
	MainMenu_Buttons.add(App->gui->CreateButton({ 600,350 }, Button_Type::CREDITS_MENU, CreditsButtonOpen[0], &CreditsButtonOpen[1], &CreditsButtonOpen[1], "Credits Button Menu", true));

	//INGAME--------------------------------------------------------------------------------------------------------------------------------------------------------
	PlayerUI_Ingame.add(App->gui->CreateLabel({ 380, 5 }, "time", Label_Type::FONT, { 255,255,255,255 }, true));
	PlayerUI_Ingame.add(timer_label = App->gui->CreateLabel({ 465,5 }, timer_string, Label_Type::CONFIG, { 255,255,255,255 }, true));
	//BACKPLAYER SQUARE
	PlayerUI_Ingame.add(Map_Icon[0] = (App->gui->CreateSprite({ 900,4 }, { 200,34,52,50 }, true)));
	//HEALTH START
	PlayerUI_Ingame.add(lifes[0] = (App->gui->CreateSprite({ 5,10 }, { 458,45,110,34 }, true)));
	PlayerUI_Ingame.add(player_status[0] = (App->gui->CreateSprite({ 120,10 }, { 413,45,20,35 }, true)));
	//COLLECTABLE
	PlayerUI_Ingame.add(score_label = App->gui->CreateLabel({ 870,10 }, "x0", Label_Type::CONFIG, { 255,255,255,255 }, true));
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
	MuteIngame_Button.add(App->gui->CreateButton({ 350,325 }, Button_Type::MUTE, ButtonMuted[0], &ButtonMuted[0], &ButtonMuted[0], "Mute ON Button", true));
	UnMuteIngame_Button.add(App->gui->CreateButton({ 350,325 }, Button_Type::UNMUTE, ButtonNOTMuted[0], &ButtonNOTMuted[0], &ButtonNOTMuted[0], "Mute OFF Button", true));
	Buttons_Listed_Ingame.add(App->gui->CreateButton({ 600,275 }, Button_Type::MENU, ButtonToMenu[0], &ButtonToMenu[0], &ButtonToMenu[0], "To Menu", true));


	//SETTINGS MENU
	SettingsMenu_UI.add(App->gui->CreateSprite({ 285,0 }, { 590,10,460,465 }, true));
	SettingsMenu_UI.add(App->gui->CreateSprite({ 375,0 }, { 690,480,280,65 }, true));
	SDL_Rect SettingsMenu_MuteON[1] = { 460,275,100,90 };
	SDL_Rect SettingsMenu_MuteOFF[1] = { 460,388,100,90 };
	SDL_Rect SettingsMenu_Close[1] = { 290,210,70,70 };
	MuteSettings_Button.add(App->gui->CreateButton({ 575,300 }, Button_Type::MUTE, SettingsMenu_MuteON[0], &SettingsMenu_MuteON[0], &SettingsMenu_MuteON[0], "Settings Mute Button", true));
	UnMuteSettings_Button.add(App->gui->CreateButton({ 575,300 }, Button_Type::UNMUTE, SettingsMenu_MuteOFF[0], &SettingsMenu_MuteOFF[0], &SettingsMenu_MuteOFF[0], "Settings Unmute Button", true));
	SettingsMenu_Buttons.add(App->gui->CreateButton({ 675,0 }, Button_Type::CLOSE_SETTINGS, SettingsMenu_Close[0], &SettingsMenu_Close[0], &SettingsMenu_Close[0], "Close Settings Menu ", true));
	SettingsMenu_UI.add(App->gui->CreateSlider({ 400,100 }, { 429,206,158,30 }, true));
	


	//CREDITS MENU
	SDL_Rect CreditsClose_Button[3] = { {290,210,70,70},{290,210,70,70},{290,210,70,70} };
	CreditsMenu_UI.add(App->gui->CreateSprite({ 285,0 }, { 590,10,460,465 }, true));
	CreditsMenu_UI.add(App->gui->CreateLabel({ 450,40 }, "MADE BY:", Label_Type::CONFIG, { 255,255,255,255 }, true));
	CreditsMenu_Buttons.add(App->gui->CreateButton({ 690,0 }, Button_Type::CREDITS_CLOSE, SettingsMenu_Close[0], &SettingsMenu_Close[0], &SettingsMenu_Close[0], "Close Menu", true));

	



	//SETTINGS MENU LISTS
	p2List_item<UIitem_Button*>*SettingsMenuButton_item = SettingsMenu_Buttons.start;
	while (SettingsMenuButton_item != NULL)
	{
		SettingsMenuButton_item->data->visible = false;
		SettingsMenuButton_item = SettingsMenuButton_item->next;
	}
	p2List_item<UI_Item*>*SettingsMenu_item = SettingsMenu_UI.start;
	while (SettingsMenu_item != NULL)
	{
		SettingsMenu_item->data->visible = false;
		SettingsMenu_item = SettingsMenu_item->next;
	}
	//LISTS INGAME MENUS & INTERFACE
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
	p2List_item<UI_Item*>* PlayerUI_item = PlayerUI_Ingame.start;
	while (PlayerUI_item != NULL)
	{
		PlayerUI_item->data->visible = false;
		PlayerUI_item = PlayerUI_item->next;
	}
	//MAIN MENU LISTS
	p2List_item<UI_Item*>* MenuUI_item = MainMenu_UI.start;
	while (MenuUI_item != NULL)
	{
		MenuUI_item->data->visible = false;
		MenuUI_item = MenuUI_item->next;
	}
	p2List_item<UIitem_Button*>* MenuButtons_item = MainMenu_Buttons.start;
	while (MenuButtons_item != NULL)
	{
		MenuButtons_item->data->visible = false;
		MenuButtons_item = MenuButtons_item->next;
	}
	//CREDITS MENU
	p2List_item<UI_Item*>* CreditsUI_item = CreditsMenu_UI.start;
	while (CreditsUI_item != NULL)
	{
		CreditsUI_item->data->visible = false;
		CreditsUI_item = CreditsUI_item->next;
	}
	p2List_item<UIitem_Button*>* CreditsButtons_item = CreditsMenu_Buttons.start;
	while (CreditsButtons_item != NULL)
	{
		CreditsButtons_item->data->visible = false;
		CreditsButtons_item = CreditsButtons_item->next;
	}

	//MUTE & UNMUTE SETTINGS
	p2List_item<UIitem_Button*>* MuteButtons_item = MuteSettings_Button.start;
	while (MuteButtons_item != NULL)
	{
		MuteButtons_item->data->visible = false;
		MuteButtons_item = MuteButtons_item->next;
	}

	p2List_item<UIitem_Button*>* UnMuteButtons_item = UnMuteSettings_Button.start;
	while (UnMuteButtons_item != NULL)
	{
		UnMuteButtons_item->data->visible = false;
		UnMuteButtons_item = UnMuteButtons_item->next;
	}

	//MUTE & UNMUTE INGAME
	p2List_item<UIitem_Button*>* MuteButtons_item2 = MuteIngame_Button.start;
	while (MuteButtons_item2 != NULL)
	{
		MuteButtons_item2->data->visible = false;
		MuteButtons_item2 = MuteButtons_item2->next;
	}

	p2List_item<UIitem_Button*>* UnMuteButtons_item2 = UnMuteIngame_Button.start;
	while (UnMuteButtons_item2 != NULL)
	{
		UnMuteButtons_item2->data->visible = false;
		UnMuteButtons_item2 = UnMuteButtons_item2->next;
	}

	

	if (OnMainMenu == true) {
		MainMenuON();
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
		//MISSING HEALTH MODIFICATION WHEN IS ATTACKED



		/*if (App->entityManager->player->Life_Num == 4 && LifeAt4 == false) {
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
		}*/

	}

	ResetEntitiesPositionNewGame = false;
	

	//SETTINGS BUTTONS MAIN MENU
	if (OnSettingsMenu == true) {

		p2List_item<UIitem_Button*>* SettingsMenuButton_item = SettingsMenu_Buttons.start;
		while (SettingsMenuButton_item != NULL) {
			if (SettingsMenuButton_item->data->OnClick()) {

				switch (SettingsMenuButton_item->data->GetType())
				{

				/*case MUTE:
					LOG("MUTE BUTTON SETTINGS");
					App->audio->TempVolumeMusicValue = App->audio->VolumeMusicValue;
					App->audio->VolumeMusicValue = 0;
					App->audio->VolumeMusic();

					break;

				case UNMUTE:
					LOG("UNMUTE BUTTON SETTINGS");
					App->audio->VolumeMusicValue = 100;
					App->audio->VolumeMusic();

					break;*/

				case CLOSE_SETTINGS:
					LOG("CLOSE SETTINGS MENU");
					
						SettingsMenuOFF();
						OnSettingsMenu = false;
						OnMainMenu = true;
						OnIngameMenu = false;
						MuteSettingsOFF();
						UnMuteSettingsOFF();
						MuteSettingsOFF();
					    UnMuteSettingsOFF();
					
					
					break;

				}
			}
			SettingsMenuButton_item = SettingsMenuButton_item->next;
		}
	}

	//CREDITS MENU
	if (OnCreditsMenu == true) {

		p2List_item<UIitem_Button*>*CreditsMenuButton_item = CreditsMenu_Buttons.start;
		while (CreditsMenuButton_item != NULL) {
			if (CreditsMenuButton_item->data->OnClick()) {

				switch (CreditsMenuButton_item->data->GetType())
				{

				case CREDITS_CLOSE:
					LOG("CREDITS MENU CLOSED");
					CreditsMenuOFF();
					OnCreditsMenu = false;
					OnMainMenu = true;

					break;

				}
			}
			CreditsMenuButton_item = CreditsMenuButton_item->next;
		}
	}

	//MAIN MENU 
	if (OnMainMenu == true) {
		p2List_item<UIitem_Button*>* MainMenuButtons = MainMenu_Buttons.start;
		while (MainMenuButtons != NULL) {
			if (MainMenuButtons->data->OnClick()) {

				switch (MainMenuButtons->data->GetType())
				{

				case START_PLAY:
					
					LOG("MAIN MENU PLAY BUTTON");
					App->SaveGame("save_game.xml");
						
					MainMenuOFF();
					IngamePlayerUION();
					OnMainMenu = false;
					OnIngameMenu = false;
					App->entityManager->player->position.x = 90.0f;
					App->entityManager->player->position.y = 250.0f;

					ResetEntitiesPositionNewGame = true;
					App->entityManager->player->Maps_Num = 0;

				    break;

				case START_CONTINUE:
					LOG("MAIN MENU CONTINUE BUTTON");
					MainMenuOFF();
					IngamePlayerUION();
					OnMainMenu = false;
					OnIngameMenu =false;
				
					break;

				case START_SETTINGS:
					LOG("MAIN MENU SETTINGS BUTTON");
					SettingsMenuON();
					OnSettingsMenu = true;
					OnMainMenu = false;
					OnIngameMenu = false;
					if (bMuteSettingsOFF == true) {
						MuteSettingsOFF();
						UnMuteSettingsON();
					}
					else {
						MuteSettingsON();
						UnMuteSettingsOFF();
					}
					
					break;
				
				case LINK:
					ShellExecuteA(NULL, "open", "https://github.com/MarcArizaAlborni/Development_Plataformas", NULL, NULL, SW_SHOWNORMAL);
					break;

				case EXIT:
					return false;
					break;

				case CREDITS_MENU:
					LOG("CREDITS MENU OPEN");
					CreditsMenuON();
					OnCreditsMenu = true;
					OnMainMenu = false;

					break;

				}
			}
			MainMenuButtons = MainMenuButtons->next;
		}
	}

	
	//INGAME MENU
	if (OnIngameMenu == true) {
		p2List_item<UIitem_Button*>* ActionButtons = Buttons_Listed_Ingame.start;
		while (ActionButtons != NULL)
		{
			if (ActionButtons->data->OnClick())
			{
				switch (ActionButtons->data->GetType())
				{
				case SAVE:
					App->SaveGame("save_game.xml");
					LOG("SAVE BUTTON");


					break;

				case LOAD:
					App->LoadGame();
					LOG("LOAD BUTTON");

					break;

				case MENU:
					LOG("TO MENU BUTTON");
					OnMainMenu = true;
					OnIngameMenu = false;
					IngameMenuOFF();
					/*MainMenu();
					MainMenu();*/
					MainMenuON();
					App->scene_ui->MuteIngameOFF();
					App->scene_ui->UnMuteIngameOFF();
					IngamePlayerUIOFF();

					//OnSettingsMenu = false;

					break;

				/*case MUTE:
					LOG("MUTE BUTTON");
					App->audio->TempVolumeMusicValue = App->audio->VolumeMusicValue;
					App->audio->VolumeMusicValue = 0;
					App->audio->VolumeMusic();

					break;

				case UNMUTE:
					LOG("UNMUTE BUTTON");
					App->audio->VolumeMusicValue = 100;
					App->audio->VolumeMusic();

					break;*/

				case CLOSE:
					App->scene_ui->IngameMenuOFF();
					App->scene_ui->OnIngameMenu = false;

					App->scene_ui->MuteIngameOFF();
					App->scene_ui->UnMuteIngameOFF();
				}
			}
			ActionButtons = ActionButtons->next;
		}
	}

	//MUTE & UNMUTE SETTINGS
	if (OnSettingsMenu == true) {
		if (bMuteSettingsON == true) {
			p2List_item<UIitem_Button*>*Mute2MenuButton_item = MuteSettings_Button.start;
			while (Mute2MenuButton_item != NULL) {
				if (Mute2MenuButton_item->data->OnClick()) {

					switch (Mute2MenuButton_item->data->GetType())
					{

					case MUTE:
						LOG("MUTE INGAME");
						MuteSettingsOFF();
						UnMuteSettingsON();
						bMuteSettingsOFF = true;
						bMuteSettingsON = false;
						

						break;

					}
				}
				Mute2MenuButton_item = Mute2MenuButton_item->next;
			}
		}
		else {
			p2List_item<UIitem_Button*>*UnMute2MenuButton_item = UnMuteSettings_Button.start;
			while (UnMute2MenuButton_item != NULL) {
				if (UnMute2MenuButton_item->data->OnClick()) {

					switch (UnMute2MenuButton_item->data->GetType())
					{

					case UNMUTE:
						LOG("UNMUTE INGAME");
						MuteSettingsON();
						UnMuteSettingsOFF();
						bMuteSettingsOFF = false;
						bMuteSettingsON = true;

						break;

					}
				}
				UnMute2MenuButton_item = UnMute2MenuButton_item->next;
			}
		}
	}

	//MUTE & UNMUTE INGAME
	if (OnIngameMenu == true) {
		if (bMuteIngameON == true) {
			p2List_item<UIitem_Button*>*MuteIngame2_item = MuteIngame_Button.start;
			while (MuteIngame2_item != NULL) {
				if (MuteIngame2_item->data->OnClick()) {

					switch (MuteIngame2_item->data->GetType())
					{

					case MUTE:
						LOG("MUTE INGAME");
						MuteIngameOFF();
						UnMuteIngameON();
						bMuteIngameOFF = true;
						bMuteIngameON = false;

						break;

					}
				}
				MuteIngame2_item = MuteIngame2_item->next;
			}
		}
		else {
			p2List_item<UIitem_Button*>*UnMuteIngame2_item = UnMuteIngame_Button.start;
			while (UnMuteIngame2_item != NULL) {
				if (UnMuteIngame2_item->data->OnClick()) {

					switch (UnMuteIngame2_item->data->GetType())
					{

					case UNMUTE:
						LOG("UNMUTE INGAME");
						MuteIngameON();
						UnMuteIngameOFF();
						bMuteIngameOFF = false;
						bMuteIngameON = true;

						break;

					}
				}
				UnMuteIngame2_item = UnMuteIngame2_item->next;
			}
		}
		
	}

	return true;
}

// Called each loop iteration
bool j1Scene_UI::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneIntro", Profiler::Color::CadetBlue);

	bool ret = true;

	

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

//void j1Scene_UI::IngameMenu()
//{
//	p2List_item<UIitem_Button*>* button_item = Buttons_Listed_Ingame.start;
//	while (button_item != NULL)
//	{
//		button_item->data->visible = !button_item->data->visible;
//		button_item = button_item->next;
//	}
//	p2List_item<UI_Item*>* ui_item = Menu_Listed_Ingame.start;
//	while (ui_item != NULL)
//	{
//		ui_item->data->visible = !ui_item->data->visible;
//		ui_item = ui_item->next;
//	}
//	
//}

//VISIBLE LISTS----------------------------------------------------------------------------------------------------------------------------------------------------------------
void j1Scene_UI::IngameMenuON()
{
	p2List_item<UIitem_Button*>* button_item = Buttons_Listed_Ingame.start;
	while (button_item != NULL)
	{
		button_item->data->visible = true;
		button_item = button_item->next;
	}
	p2List_item<UI_Item*>* ui_item = Menu_Listed_Ingame.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = true;
		ui_item = ui_item->next;
	}
}

void j1Scene_UI::IngameMenuOFF()
{
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
}

void j1Scene_UI::SettingsMenuOFF()
{
	
	p2List_item<UI_Item*>* SettingsUI_item1 = SettingsMenu_UI.start;
	while (SettingsUI_item1 != NULL)
	{
		SettingsUI_item1->data->visible = false;
		SettingsUI_item1 = SettingsUI_item1->next;
	}

	p2List_item<UIitem_Button*>* SettingsButton_item1 = SettingsMenu_Buttons.start;
	while (SettingsButton_item1 != NULL)
	{
		SettingsButton_item1->data->visible = false;
		SettingsButton_item1 = SettingsButton_item1->next;
	}
}

void j1Scene_UI::CreditsMenuON()
{
	p2List_item<UI_Item*>* Credits_UI2 = CreditsMenu_UI.start;
	while (Credits_UI2 != NULL)
	{
		Credits_UI2->data->visible = true;
		Credits_UI2 = Credits_UI2->next;
	}

	p2List_item<UIitem_Button*>* Credits_Button2 = CreditsMenu_Buttons.start;
	while (Credits_Button2 != NULL)
	{
		Credits_Button2->data->visible = true;
		Credits_Button2 = Credits_Button2->next;
	}
}


void j1Scene_UI::CreditsMenuOFF()
{
	p2List_item<UI_Item*>* Credits_UI = CreditsMenu_UI.start;
	while (Credits_UI != NULL)
	{
		Credits_UI->data->visible = false;
		Credits_UI = Credits_UI->next;
	}

	p2List_item<UIitem_Button*>* Credits_Button = CreditsMenu_Buttons.start;
	while (Credits_Button != NULL)
	{
		Credits_Button->data->visible = false;
		Credits_Button = Credits_Button->next;
	}
}

void j1Scene_UI::SettingsMenuON()
{
	
	p2List_item<UI_Item*>* SettingsUI_item1 = SettingsMenu_UI.start;
	while (SettingsUI_item1 != NULL)
	{
		SettingsUI_item1->data->visible = true;
		SettingsUI_item1 = SettingsUI_item1->next;
	}

	p2List_item<UIitem_Button*>* SettingsButton_item1 = SettingsMenu_Buttons.start;
	while (SettingsButton_item1 != NULL)
	{
		SettingsButton_item1->data->visible = true;
		SettingsButton_item1 = SettingsButton_item1->next;
	}
}

//void j1Scene_UI::IngamePlayerUI()
//{
//	p2List_item<UI_Item*>* PlayerUI_item = PlayerUI_Ingame.start;
//	while (PlayerUI_item != NULL)
//	{
//		PlayerUI_item->data->visible = !PlayerUI_item->data->visible;
//		PlayerUI_item = PlayerUI_item->next;
//	}
//}

//void j1Scene_UI::MainMenu()
//{
//
//	p2List_item<UIitem_Button*>* button_Menu = MainMenu_Buttons.start;
//	while (button_Menu != NULL)
//	{
//		button_Menu->data->visible = !button_Menu->data->visible;
//		button_Menu = button_Menu->next;
//	}
//	p2List_item<UI_Item*>* ui_Menu = MainMenu_UI.start;
//	while (ui_Menu != NULL)
//	{
//		ui_Menu->data->visible = !ui_Menu->data->visible;
//		ui_Menu = ui_Menu->next;
//	}
//
//}

void j1Scene_UI::IngamePlayerUIOFF()
{
	p2List_item<UI_Item*>* PlayerUI_item = PlayerUI_Ingame.start;
	while (PlayerUI_item != NULL)
	{
		PlayerUI_item->data->visible = false;
		PlayerUI_item = PlayerUI_item->next;
	}
}

void j1Scene_UI::IngamePlayerUION()
{
	p2List_item<UI_Item*>* PlayerUI_item = PlayerUI_Ingame.start;
	while (PlayerUI_item != NULL)
	{
		PlayerUI_item->data->visible = true;
		PlayerUI_item = PlayerUI_item->next;
	}
}

void j1Scene_UI::MainMenuOFF()
{
	p2List_item<UIitem_Button*>* button_Menu = MainMenu_Buttons.start;
	while (button_Menu != NULL)
	{
		button_Menu->data->visible = false;
		button_Menu = button_Menu->next;
	}
	p2List_item<UI_Item*>* ui_Menu = MainMenu_UI.start;
	while (ui_Menu != NULL)
	{
		ui_Menu->data->visible = false;
		ui_Menu = ui_Menu->next;
	}
}

void j1Scene_UI::MainMenuON()
{
	p2List_item<UIitem_Button*>* button_Menu2 = MainMenu_Buttons.start;
	while (button_Menu2 != NULL)
	{
		button_Menu2->data->visible = true;
		button_Menu2 = button_Menu2->next;
	}
	p2List_item<UI_Item*>* ui_Menu2 = MainMenu_UI.start;
	while (ui_Menu2 != NULL)
	{
		ui_Menu2->data->visible = true;
		ui_Menu2 = ui_Menu2->next;
	}
}

// MUTE & UNMUTE CHECKBOX
void j1Scene_UI::MuteSettingsON(){
	p2List_item<UIitem_Button*>* button_Menu2 = MuteSettings_Button.start;
	while (button_Menu2 != NULL)
	{
		button_Menu2->data->visible = true;
		button_Menu2 = button_Menu2->next;
	}
}
void j1Scene_UI::MuteSettingsOFF(){
	p2List_item<UIitem_Button*>* button_Menu2 = MuteSettings_Button.start;
	while (button_Menu2 != NULL)
	{
		button_Menu2->data->visible = false;
		button_Menu2 = button_Menu2->next;
	}
}
void j1Scene_UI::MuteIngameON(){
	p2List_item<UIitem_Button*>* button_Menu2 = MuteIngame_Button.start;
	while (button_Menu2 != NULL)
	{
		button_Menu2->data->visible = true;
		button_Menu2 = button_Menu2->next;
	}
}
void j1Scene_UI::MuteIngameOFF(){
	p2List_item<UIitem_Button*>* button_Menu2 = MuteIngame_Button.start;
	while (button_Menu2 != NULL)
	{
		button_Menu2->data->visible = false;
		button_Menu2 = button_Menu2->next;
	}
}
void j1Scene_UI::UnMuteSettingsON(){
	p2List_item<UIitem_Button*>* button_Menu2 = UnMuteSettings_Button.start;
	while (button_Menu2 != NULL)
	{
		button_Menu2->data->visible = true;
		button_Menu2 = button_Menu2->next;
	}
}
void j1Scene_UI::UnMuteSettingsOFF(){
	p2List_item<UIitem_Button*>* button_Menu2 = UnMuteSettings_Button.start;
	while (button_Menu2 != NULL)
	{
		button_Menu2->data->visible = false;
		button_Menu2 = button_Menu2->next;
	}
}
void j1Scene_UI::UnMuteIngameON(){
	p2List_item<UIitem_Button*>* button_Menu2 = UnMuteIngame_Button.start;
	while (button_Menu2 != NULL)
	{
		button_Menu2->data->visible = true;
		button_Menu2 = button_Menu2->next;
	}
}
void j1Scene_UI::UnMuteIngameOFF(){
	p2List_item<UIitem_Button*>* button_Menu2 = UnMuteIngame_Button.start;
	while (button_Menu2 != NULL)
	{
		button_Menu2->data->visible = false;
		button_Menu2 = button_Menu2->next;
	}
}



