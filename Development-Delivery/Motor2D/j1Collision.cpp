#include "p2Log.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Input.h"

#include "Brofiler/Brofiler.h"


j1Collision::j1Collision() : j1Module()
{
	name.create("collision");
	
	debug = false;
}

j1Collision::~j1Collision()
{

}

bool j1Collision::Awake(pugi::xml_node config)
{

	return true;
}

bool j1Collision::Start()
{
	BROFILER_CATEGORY("FadeToBlack Start();", Profiler::Color::Chartreuse)
	LoadCollider();
	return true;
}

bool j1Collision::PreUpdate()
{
	BROFILER_CATEGORY("Collision PreUpdate();", Profiler::Color::Chocolate)

	p2List_item<Collider*>* collider_iterator = collider.start;
	for (collider_iterator; collider_iterator != nullptr; collider_iterator = collider_iterator->next) {

		if (collider_iterator->data->to_delete == true)
		{
			collider.del(collider_iterator);
		}
	}

	Collider* c1;
	Collider* c2;
	
	for (collider_iterator=collider.start; collider_iterator != nullptr; collider_iterator = collider_iterator->next)
	{
		
		c1 = collider_iterator->data;

		p2List_item<Collider*>* collider_iterator2 = collider.start;
		for (collider_iterator2; collider_iterator2 != nullptr; collider_iterator2 = collider_iterator2->next)
		{

			c2 = collider_iterator2->data;

			if (c1 != c2)
			{
				if (c1->CheckCollision(c2->rect) == true && (c1->type == ObjectType::Player || c2->type == ObjectType::Player))
				{
					if (c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (c2->callback)
						c2->callback->OnCollision(c2, c1);
				}

			    if (c1->CheckCollision(c2->rect) == true && (c1->type == ObjectType::Skeleton || c2->type == ObjectType::Skeleton)) {
					if (c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (c2->callback)
						c2->callback->OnCollision(c2, c1);
				}

			    if (c1->CheckCollision(c2->rect) == true && (c1->type == ObjectType::Bee || c2->type == ObjectType::Bee)) {
					 if (c1->callback)
						 c1->callback->OnCollision(c1, c2);

					 if (c2->callback)
						 c2->callback->OnCollision(c2, c1);
			    }

				if (c1->CheckCollision(c2->rect) == true && (c1->type == ObjectType::Slime || c2->type == ObjectType::Slime)) {
					if (c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (c2->callback)
						c2->callback->OnCollision(c2, c1);
				}

				if (c1->CheckCollision(c2->rect) == true && (c1->type == ObjectType::Skull || c2->type == ObjectType::Skull)) {
					if (c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (c2->callback)
						c2->callback->OnCollision(c2, c1);
				}

				if (c1->CheckCollision(c2->rect) == true && (c1->type == ObjectType::MapEntityCollider || c2->type == ObjectType::MapEntityCollider)) {
					if (c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
			}
		}
	}

	return true;
}

bool j1Collision::Update(float dt)
{
	BROFILER_CATEGORY("FadeToBlack Update();", Profiler::Color::Coral)
	DebugDraw();
	return true;
}

bool j1Collision::CleanUp()
{
	return true;
}


void j1Collision::DebugDraw()
{
	if (App->input->keyboard[SDL_SCANCODE_F9] == KEY_DOWN) {

		if (debug) {
			debug = false;
		}
		else
		{
			debug = true;
		}
	}
	if (debug == false)
		return;

	uint alpha = 100;

	p2List_item<Collider*>* collider_iterator = collider.start;

	for (collider_iterator; collider_iterator != nullptr; collider_iterator = collider_iterator->next)
	{
	
		switch (collider_iterator->data->type)
		{
		case ObjectType::Platform: // red
			App->render->DrawQuad(collider_iterator->data->rect, 255, 0, 0, alpha);
			break;
		case ObjectType::Player: // yellow
			App->render->DrawQuad(collider_iterator->data->rect, 255, 255, 0, alpha);
			break;

		case ObjectType::Skeleton:
			App->render->DrawQuad(collider_iterator->data->rect, 133, 222, 0, alpha);
			break;

		case ObjectType::Skull:
			App->render->DrawQuad(collider_iterator->data->rect, 66, 0, 0, alpha);
			break;

		case ObjectType::Slime:
			App->render->DrawQuad(collider_iterator->data->rect, 77, 111, 0, alpha);
			break;

		case ObjectType::Bee:
			App->render->DrawQuad(collider_iterator->data->rect, 88, 222, 0, alpha);
			break;

		case ObjectType::Water: // blue
			App->render->DrawQuad(collider_iterator->data->rect, 0, 0, 255, alpha);
			break;

		case ObjectType::Victory: // Green
			App->render->DrawQuad(collider_iterator->data->rect, 0, 255, 0, alpha);
			break;

		case ObjectType::Teleporter1: // blue fosc
			App->render->DrawQuad(collider_iterator->data->rect, 0, 0, 204, alpha);
			break;

		case ObjectType::Teleporter2: //blue fosc
			App->render->DrawQuad(collider_iterator->data->rect, 0, 0, 204, alpha);
			break;

		case ObjectType::AudioArea1: // 
			App->render->DrawQuad(collider_iterator->data->rect, 0, 255, 128, alpha);
			break;

		case ObjectType::AudioArea2: // 
			App->render->DrawQuad(collider_iterator->data->rect, 0, 255, 128, alpha);
			break;

		case ObjectType::CheckPoint: //
			App->render->DrawQuad(collider_iterator->data->rect, 128, 255, 0, alpha);
			break;

		case ObjectType::TransformationPoint: //
			App->render->DrawQuad(collider_iterator->data->rect, 100, 100, 0, alpha);
			break;

		case ObjectType::LateralPlatform: //
			App->render->DrawQuad(collider_iterator->data->rect, 255, 255, 0, alpha);
			break;
		case ObjectType::LateralPlatformLeft: //
			App->render->DrawQuad(collider_iterator->data->rect, 255, 255, 0, alpha);
			break;
		case ObjectType::JumpDetector: //
			App->render->DrawQuad(collider_iterator->data->rect, 100, 255, 0, alpha);
			break;
		case ObjectType::MapEntityCollider: //
			App->render->DrawQuad(collider_iterator->data->rect, 100, 255, 0, alpha);
			break;
		}
	}

}

Collider* j1Collision::AddCollider(SDL_Rect rectangle, ObjectType type, j1Module* callback )
{

	Collider* rect = new Collider;

	rect->callback = callback;
	rect->rect = rectangle;
	rect->type = type;

	collider.add(rect);

	LOG("Added collider");

	return rect;
}

void j1Collision::LoadCollider() {
	p2List_item<ObjectGroup*>* item = App->map->data.objectgroups.start;
	for( item; item != nullptr; item->next) {
		for (int i = 0; i < item->data->object_count; i++) {

			SDL_Rect* rect = new SDL_Rect;

			rect->x = item->data->object_properties[i].x;
			rect->y = item->data->object_properties[i].y;
			rect->w = item->data->object_properties[i].width;
			rect->h = item->data->object_properties[i].height;

			AddCollider(*rect, item->data->object_properties[i].type, nullptr);
		}

		item = item->next;
	}
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	
	return ((r.x + r.w > rect.x) &&
		(r.x < rect.x + rect.w) &&
		(r.y + r.h > rect.y) && 
		(r.y < rect.y + rect.h));

}

Collider::Collider(ObjectProperties object) {

	rect.x = object.x;
	rect.y = object.y;
	rect.w = object.width;
	rect.h = object.height;
	type = object.type;
}