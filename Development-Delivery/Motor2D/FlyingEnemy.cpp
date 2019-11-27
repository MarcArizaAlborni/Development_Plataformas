#include "FlyingEnemy.h"
#include "j1Entities.h"
#include "j1Player.h"

j1FlyingEnemy::j1FlyingEnemy(iPoint FEnemyPos, EntityType type):j1Entity(position,type)
{

}

j1FlyingEnemy::~j1FlyingEnemy()
{
}

bool j1FlyingEnemy::Start()
{
	return false;
}

bool j1FlyingEnemy::Update(float dt)
{
	return false;
}

bool j1FlyingEnemy::PostUpdate()
{
	return false;
}

bool j1FlyingEnemy::CleanUp()
{
	return false;
}

void j1FlyingEnemy::OnCollision(Collider * A, Collider * B)
{
}

void j1FlyingEnemy::Moving(p2DynArray<iPoint>& path, float dt)
{
}
