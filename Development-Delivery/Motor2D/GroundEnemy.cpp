#include "GroundEnemy.h"
#include "FlyingEnemy.h"
#include "j1Entities.h"
#include "j1Player.h"

j1GroundEnemy::j1GroundEnemy(iPoint FEnemyPos, EntityType type) :j1Entity(position, type)
{
}

j1GroundEnemy::~j1GroundEnemy()
{
}

bool j1GroundEnemy::Start()
{
	return false;
}

bool j1GroundEnemy::Update(float dt)
{
	return false;
}

bool j1GroundEnemy::PostUpdate()
{
	return false;
}

bool j1GroundEnemy::CleanUp()
{
	return false;
}

void j1GroundEnemy::OnCollision(Collider * A, Collider * B)
{
}

void j1GroundEnemy::Moving(p2DynArray<iPoint>& path, float dt)
{
}
