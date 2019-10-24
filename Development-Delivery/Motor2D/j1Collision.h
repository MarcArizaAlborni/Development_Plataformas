#pragma once

#include "j1Module.h"
#include "SDL/include/SDL.h"

enum class ObjectType;

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	ObjectType type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, ObjectType type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void inline SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collision : public j1Module
{

public:

	j1Collision();

	// Destructor
	virtual ~j1Collision();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update();

	// Called before quitting
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, ObjectType type,  j1Module* callback = nullptr);
	void DebugDraw();
	bool debug = true;

private:

	Collider* collider;

};