#ifndef __j1GUI_INPUTBOX_H__
#define __j1GUI_INPUTBOX_H__

#include "j1Gui_Elements.h"

class Gui_Label;
class Gui_Image;

class Gui_InputBox:public Gui_Elements
{
public:
	Gui_InputBox(Element_type type, iPoint position, SDL_Rect rect, SDL_Texture* tex);

	~Gui_InputBox();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

public:

	Gui_Label* Label;
	Gui_Image* Image;
	
};

#endif //__j1GUI_INPUTBOX_H__