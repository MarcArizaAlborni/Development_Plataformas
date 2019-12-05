#include "j1Gui_InputBox.h"
#include "j1Gui_Elements.h"
#include "j1Gui_Image.h"
#include "j1Gui_Label.h"
#include "j1Fonts.h"

Gui_InputBox::Gui_InputBox(Element_type type, iPoint position, SDL_Rect rect, SDL_Texture* tex) : Gui_Elements(type, position, rect, tex)
{
	Image = new Gui_Image(type,position, rect, tex);
	Label = new Gui_Label(type, position, rect, tex);
}

Gui_InputBox::~Gui_InputBox() {}

bool Gui_InputBox::Awake() {

	return true;
}

bool Gui_InputBox::Start()
{
	texture = App->font->Print("Your Name");
	return true;
}

bool Gui_InputBox::PreUpdate() {

	return true;
}

bool Gui_InputBox::PostUpdate() {

	App->render->Blit(texture, pos.x, pos.y, &Rect, 0);
	return true;
}

bool Gui_InputBox::CleanUp() {

	return true;
}