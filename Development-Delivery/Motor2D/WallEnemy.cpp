#include "WallEnemy.h"

j1WallEnemy::j1WallEnemy(iPoint WallPos, EntityType type):j1Entity(position,type)
{
}

j1WallEnemy::~j1WallEnemy()
{
}

bool j1WallEnemy::Start()
{
	return false;
}

bool j1WallEnemy::Update(float dt)
{
	return false;
}

bool j1WallEnemy::PostUpdate()
{
	return false;
}

bool j1WallEnemy::CleanUp()
{
	return false;
}

void j1WallEnemy::OnCollision(Collider * A, Collider * B)
{
}

void j1WallEnemy::Moving(p2DynArray<iPoint>& path, float dt)
{
}
