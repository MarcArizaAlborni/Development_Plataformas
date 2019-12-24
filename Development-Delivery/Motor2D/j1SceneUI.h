#ifndef _SCENE_UI__H
#define _SCENE_UI_H

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "p2List.h"
#include "j1Gui.h"
#include "j1Button.h"
#include "j1SpriteUI.h"
#include "j1LabelUI.h"


class j1Scene_UI : public j1Module
{
public:

	j1Scene_UI();

	// Destructor
	virtual ~j1Scene_UI();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	void IngameMenu(); // OPEN AND CLOSE MENU WHEN INGAME
	void MainMenu(); //MAIN MENU SCREEN
	

public:

	char		player_score_string[5] = "0";
	char		timer_string[20] = "0";
	uint        player_lifes = 3;
	UI_Item*    lifes[4] = { nullptr,nullptr,nullptr,nullptr };
	UI_Item*    player_status[4] = { nullptr,nullptr,nullptr,nullptr };
	UI_Item*    Character_Back[1] = { nullptr};
	UI_Item*    Map_Icon[1] = { nullptr };
	UI_Label*	score_label = nullptr;
	UI_Label*	timer_label = nullptr;
	bool LifeAt4;
	bool LifeAt3;
	bool LifeAt2;
	bool LifeAt1;

	p2List<UIitem_Button*>	Buttons_Listed_Ingame;
	p2List<UI_Item*>		Menu_Listed_Ingame; //Pause menu when esc pressed
	

private:
	p2SString			path;
	p2List<j1Module*>	current_level;
	j1PerfTimer			ptimer;
};

#endif // !_SCENE_UI_H