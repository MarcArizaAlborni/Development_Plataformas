#include "j1Entities.h"
#include "p2Point.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "j1Pathfinding.h"



struct SDL_Texture;
struct Collider;


enum EntityType;


class j1WallEnemy :public j1Entity {

public:

	j1WallEnemy(iPoint WallPos, EntityType type);

	virtual ~j1WallEnemy();


	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* A, Collider* B);

	void Moving(p2DynArray<iPoint>& path, float dt);


public:

	//RECT
	SDL_Rect Wall_Rect;
	//COLLIDER
	Collider* Wall_Collider;
	//TEXTURE
	SDL_Texture* Wall_Text;
	//ANIMATION
	Animation* WallAnim;


	p2DynArray<iPoint>* path;
};