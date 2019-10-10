#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 1: Create a struct for the map layer
// ----------------------------------------------------

struct MapLayer
{
	p2SString name;
	uint width = 0u;
	uint height = 0u;
	uint* tile_ids;

	// TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y)
	{
		return (y * width) + x;
	};
};


// ----------------------------------------------------
struct TileSet
{

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

	// TODO 7: Create a method that receives a tile id and returns it's Rect

	SDL_Rect GetTileRectId(uint tile_id) 
	{
		SDL_Rect rect;

		uint new_id = tile_id - firstgid;
		rect.w = tile_width;
		rect.h = tile_height;
		rect.x = margin + ((spacing + tile_width) * (new_id % num_tiles_width));
		rect.y = margin + ((spacing + tile_height) * (new_id / num_tiles_width));
		return rect;
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
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;

	// TODO 2: Add a list/array of layers to the map!
	p2List<MapLayer*>   maplayers;
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
	inline uint GetPosX(uint x)
	{
		TileSet* id;

		return x * id->tile_width;
	}

	inline uint GetPosY(uint y)
	{
		TileSet* id;
		return y * id->tile_height;
	}


private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	// TODO 3: Create a method that loads a single layer
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__