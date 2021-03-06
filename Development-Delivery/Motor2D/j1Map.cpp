#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>
#include "j1Audio.h"
#include "j1Window.h"
#include "j1Collision.h"

#include "Brofiler/Brofiler.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("Map Draw();", Profiler::Color::PaleVioletRed)

	if(map_loaded == false)
		return;

	// TODO 5: Prepare the loop to iterate all the tiles in a layer
	
	MapLayer* layer = data.maplayers[0];

	p2List_item<MapLayer*>* item = data.maplayers.start;
	
	// Camera culling

	uint camera_rect_w;
	uint camera_rect_h;

	App->win->GetWindowSize(camera_rect_w, camera_rect_h);

	camera_collider.rect.w = camera_rect_w;
	camera_collider.rect.h = camera_rect_h;

	App->map->camera_collider.SetPos(-App->render->camera.x, -App->render->camera.y);

	for (item; item != NULL; item = item->next) {

		uint* gid = item->data->data;
		uint i = 0;

		if (item->data->properties.Get("Nodraw") != 0) {
			continue;
		}

		for (uint y = 0; y < data.height; ++y)
		{
			for (uint x = 0; x < data.width; ++x)
			{
				iPoint tileCoords = MapToWorld(x, y);
				float speed = 1.0f;

				if (item->data->name == "Clouds")
				{

					speed = 0.50f;

					tile_rect.x = (App->render->camera.x * speed) + data.tilesets[0]->GetPos(x, y).x - App->render->camera.x;
					tile_rect.y = (App->render->camera.y * speed) + data.tilesets[0]->GetPos(x, y).y - App->render->camera.y;

				}
				else if (item->data->name == "Water")
				{

					speed = 0.75f;

					tile_rect.x = (App->render->camera.x * speed) + data.tilesets[0]->GetPos(x, y).x - App->render->camera.x;
					tile_rect.y = (App->render->camera.y * speed) + data.tilesets[0]->GetPos(x, y).y - App->render->camera.y;

				}
				else
				{
					tile_rect.x = data.tilesets[0]->GetPos(x, y).x;
					tile_rect.y = data.tilesets[0]->GetPos(x, y).y;
				}
				
				tile_rect.h = App->map->data.tile_height;
				tile_rect.w = App->map->data.tile_height;

				if (camera_collider.CheckCollision(tile_rect))
				{
					App->render->Blit(data.tilesets[0]->texture, tileCoords.x, tileCoords.y, data.tilesets[0]->GetTileRectId(gid[i]), false, speed );
				}

				i++;
			}
		}
		
	}
	
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0, 0);
	// TODO 8(old): Create a method that translates x,y coordinates from map positions to world positions

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}

	// TODO 1: Add isometric map to world coordinates
	if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width / 2);
		ret.y = (x + y) * (data.tile_height / 2);
	}
	return ret;
}


iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);
	// TODO 2: Add orthographic world to map coordinates

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}

	// TODO 3: Add the case for isometric maps to WorldToMap

	if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = ((x / data.tile_width) + (y / data.tile_height) / 4);
		ret.y = ((y / data.tile_height) - (x / data.tile_width) / 4);
	}
	return ret;
}

SDL_Rect* TileSet::GetTileRectId(uint id) 
{
	SDL_Rect* rect = ReturnedRect;
	// TODO 7(old): Create a method that receives a tile id and returns it's Rect

	int new_id = id - firstgid;
	rect->w = tile_width;
	rect->h = tile_height;
	rect->x = margin + ((spacing + tile_width) * (new_id % num_tiles_width));
	rect->y = margin + ((spacing + tile_height) * (new_id / num_tiles_width));
	return rect;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}




// Called before quitting
bool j1Map::CleanUp()
{
	BROFILER_CATEGORY("Map CleanUp();", Profiler::Color::LemonChiffon)
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// TODO 2: clean up all layer data
	// Remove all layers
	p2List_item<MapLayer*>* maplayers_item;
	maplayers_item = data.maplayers.start;

	while (maplayers_item != NULL)
	{
		RELEASE(maplayers_item->data);
		maplayers_item = maplayers_item->next;
	}
	data.maplayers.clear();

	//REMOVE OBJECTS
	p2List_item<ObjectGroup*>* object_iterator = data.objectgroups.start;
	while (object_iterator != NULL)
	{
		RELEASE(object_iterator->data);				//RELEASE deletes all elements in a list (deletes a buffer).
		object_iterator = object_iterator->next;
	}
	data.objectgroups.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// TODO 4: Iterate all layers and load each of them
	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, lay);
		}

		data.maplayers.add(lay);
	}

	// Load Object Group info -------------------------------------------
	pugi::xml_node objectgroup;
	for (objectgroup = map_file.child("map").child("objectgroup"); objectgroup && ret; objectgroup = objectgroup.next_sibling("objectgroup"))
	{
		ObjectGroup* obj = new ObjectGroup();

		if (ret == true)
		{
			ret = LoadObjectGroup(objectgroup, obj);
		}

		data.objectgroups.add(obj);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// TODO 4: Add info here about your loaded layers
		// Adapt this code with your own variables
		
		p2List_item<MapLayer*>* item_layer = data.maplayers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}

		p2List_item<ObjectGroup*>* obj_layer = data.objectgroups.start;
		while (obj_layer != NULL)
		{
			ObjectGroup* o = obj_layer->data;
			LOG("Group ----");
			LOG("Gname: %s", o->name.GetString());

			obj_layer = obj_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	//App->audio->PlayMusic(App->map->data.MusicAudio_Files.GetString());
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		data.MusicAudio_Files = map.child("properties").child("property").attribute("value").as_string();

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

// TODO 3: Create the definition for a function that loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	if ( node == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'layer' tag.");
			ret = false;
	}
	else
	{
		layer->name = node.attribute("name").as_string();
		layer->width = node.attribute("width").as_uint();
		layer->height = node.attribute("height").as_uint();
		LoadProperties(node, layer->properties);
	    layer->data = new uint[layer->width * layer->height];

		memset(layer->data, 0 , sizeof(uint) * layer->height * layer->width);

		uint i = 0;

		for (pugi::xml_node tile_info = node.child("data").child("tile"); tile_info; tile_info = tile_info.next_sibling("tile"))
		{
			layer->data[i] = tile_info.attribute("gid").as_uint();
			i++;
		}
	}
	
	return ret;
}

bool j1Map::LoadObjectGroup(pugi::xml_node& node, ObjectGroup* object)
{
	bool ret = true;

	if (node == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'objectgroup' tag.");
		ret = false;
	}
	else
	{

		object->id = node.attribute("id").as_uint();
		object->name = node.attribute("name").as_string();

		uint num_objects = 0;
		for (pugi::xml_node object_node = node.child("object"); object_node && ret; object_node = object_node.next_sibling("object"))
		{ 
			if (ret == true)
			{
				num_objects++;
			}
			else
			{
				LOG("Error parsing tileset xml file: Cannot find 'object' tag. ");
			}
		}

		object->object_count = num_objects;
		object->object_properties = new ObjectProperties[num_objects];
		memset(object->object_properties, 0, num_objects * sizeof(ObjectProperties));

		uint i = 0;
		for (pugi::xml_node object_node = node.child("object"); object_node && ret; object_node = object_node.next_sibling("object"))
		{
			if (ret == true)
			{

				object->object_properties[i].id = object_node.attribute("id").as_uint();
				object->object_properties[i].name = object_node.attribute("name").as_string();
				object->object_properties[i].x = object_node.attribute("x").as_uint();
				object->object_properties[i].y = object_node.attribute("y").as_uint();
				object->object_properties[i].width = object_node.attribute("width").as_uint();
				object->object_properties[i].height = object_node.attribute("height").as_uint();


				p2SString type(object_node.attribute("type").as_string());

				if (type == "Platform")
				{
					object->object_properties[i].type = ObjectType::Platform;
				}
				else if (type == "Player")
				{
					object->object_properties[i].type = ObjectType::Player;
				}
				else if (type == "Water")
				{
					object->object_properties[i].type = ObjectType::Water;
				}
				else if (type == "Victory")
				{
					object->object_properties[i].type = ObjectType::Victory;
				}
				else if (type == "Teleporter1")
				{
					object->object_properties[i].type = ObjectType::Teleporter1;
				}
				else if (type == "Teleporter2")
				{
					object->object_properties[i].type = ObjectType::Teleporter2;
				}
				else if (type == "AudioArea1")
				{
					object->object_properties[i].type = ObjectType::AudioArea1;
				}
				else if (type == "AudioArea2")
				{
					object->object_properties[i].type = ObjectType::AudioArea2;
				}
				else if (type == "CheckPoint")
				{
					object->object_properties[i].type = ObjectType::CheckPoint;
				}
				else if (type == "TransformationPoint")
				{
					object->object_properties[i].type = ObjectType::TransformationPoint;
				}
				else if (type == "LateralPlatform")
				{
					object->object_properties[i].type = ObjectType::LateralPlatform;
				}
				else if (type == "LateralPlatformLeft")
				{
					object->object_properties[i].type = ObjectType::LateralPlatformLeft;
				}
				else if (type == "JumpDetector")
				{
					object->object_properties[i].type = ObjectType::JumpDetector;
				}
				
				else
				{
					object->object_properties[i].type = ObjectType::Unknown;
				}

				i++;
			}
			else
			{
				LOG("Error parsing tileset xml file: Cannot find 'object' tag. ");
			}
		}

	}

	return ret;
}

bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.add(p);
		}
	}

	return ret;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = data.maplayers.start;

	for (item = data.maplayers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}
