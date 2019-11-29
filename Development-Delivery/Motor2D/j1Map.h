#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Collision.h"

// TODO 1: Create a struct for the map layer
// ----------------------------------------------------

struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};


enum class ObjectType
{
	Platform,
	LateralPlatform,
	Player,
	Water,
	Victory,
	Teleporter1,
	Teleporter2,
	AudioArea,
	CheckPoint,
	TransformationPoint,
	Unknown

};
struct MapLayer
{
	p2SString	name;
	uint		width;
	uint		height;
	uint*		data;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y)
	{
		return data[(y * width) + x];
	};

};

struct ObjectProperties
{
	uint			id;
	p2SString		name;
	uint			x;
	uint			y;
	uint			width;
	uint			height;
	ObjectType		type;

};

struct ObjectGroup
{
	uint				id;
	p2SString			name;
	ObjectProperties*	object_properties;
	uint				object_count;
};


// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rect
	SDL_Rect* ReturnedRect = new SDL_Rect;
	SDL_Rect* GetTileRectId(uint tile_id);
	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

	inline p2Point<uint> GetPos(uint x, uint y) {
		p2Point<uint> ret;
		ret.x = x * tile_width;
		ret.y = y * tile_height;

		return ret;
	}

};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int						width;
	int						height;
	int						tile_width;
	int						tile_height;
	SDL_Color				background_color;
	MapTypes				type;
	p2List<TileSet*>		tilesets;
	// TODO 2: Add a list/array of layers to the map!
	p2List<MapLayer*>		maplayers;
	p2List<ObjectGroup*>	objectgroups;
	

	p2SString MusicAudio_Files;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	// Create a method that loads a single layer
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	// Create a methid to load the Object Group
	bool LoadObjectGroup(pugi::xml_node& node, ObjectGroup* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;
	Collider camera_collider;
	SDL_Rect tile_rect;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__