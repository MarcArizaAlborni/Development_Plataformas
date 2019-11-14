#include "j1Entities.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Map.h"
#include "p2Animation.h"
#include "j1Player.h"
#include "p2Log.h"





j1Entity::j1Entity(iPoint pos, ENTITY_TYPE Type) :position(pos), type(Type) {

}

bool j1Entity::CleanUp() {

	if (collider != nullptr)
		collider->to_delete = true;

	return true;
}


