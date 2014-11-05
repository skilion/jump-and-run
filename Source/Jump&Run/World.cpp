#include <Render/Color.h>

#include <Memory.h>

#include "World.h"

using namespace myengine;


World world;


//=============================================================================
// Constructor
//=============================================================================
World::World()
{
	map = nullptr;
	lenght = 0;
}

//=============================================================================
// Destructor
//=============================================================================
World::~World()
{
	delete [] map;
}

//=============================================================================
// Returns the tile at the specified position
//=============================================================================
Tile World::getTile(const Vec2i &pos) const
{
	return map[pos.getX() * MAP_Y_TILES + pos.getY()];
}

//=============================================================================
// Sets the tile at the specified position map
//=============================================================================
void World::setTile(Vec2i &pos, Tile tile)
{
	map[pos.getX() * MAP_Y_TILES + pos.getY()] = tile;
}

//=============================================================================
// Sets the world texture used for storing the tiles
//=============================================================================
void World::setWorldTexture(Texture *worldTex)
{
	this->worldTex = worldTex;
}

//=============================================================================
// Returns true if the specified rect collide with a solid tile
//=============================================================================
bool World::collide(const Rect &rect)
{
	Vec2i start, end;
	pixelToMap(rect.getMin(), start);
	pixelToMap(rect.getMax(), end);

	for(int x = start.getX(); x <= end.getX(); x++) {
		for(int y = start.getY(); y <= end.getY(); y++) {
			switch(map[x * MAP_Y_TILES + y].type)
			{
			case TILE_SOLID:
			case TILE_LETHAL:
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// Draws the world
//=============================================================================
void World::draw(int pixelShift, bool debug)
{
	render->setDrawColor(Color(1, 1, 1, 1));

	unsigned tileShift = pixelShift / TILE_SIZE_PIXEL;
	unsigned startDrawX = -(pixelShift % TILE_SIZE_PIXEL);

	Vec2i pos(startDrawX, 0);
	for(unsigned x = 0; x <= SCREEN_X_TILES * 2; x++)
	{
		unsigned mapX = tileShift + x;
		if(mapX >= lenght) break; //do not exceed the lenght of the world
		for(unsigned y = 0; y < SCREEN_Y_TILES; y++)
		{
			Tile &tile = map[mapX * MAP_Y_TILES + y];

			//Foreground tile
			drawTile(pos, tile.tile1);

			//Background tile
			if(tile.tile2 != 0) drawTile(pos, tile.tile2);

			if(debug) {  //Highlights special blocks
				if(tile.type != 0) {
					switch(tile.type)
					{
					case TILE_SOLID:
						render->setDrawColor(Color(0, 0, 1, 0.5f));
						break;

					case TILE_LETHAL:
						render->setDrawColor(Color(1, 0, 0, 0.5f));
						break;

					case TILE_COIN:
						render->setDrawColor(Color(1, 1, 0, 0.5f));
						break;

					case TILE_ENEMY_STOP:
						render->setDrawColor(Color(1, 0, 1, 0.5f));
						break;

					case TILE_DIAMOND:
						render->setDrawColor(Color(0, 1, 1, 0.5f));
						break;

					case TILE_END_LEVEL:
						render->setDrawColor(Color(0, 1, 0, 0.5f));
						break;
					}

					render->drawFilledRect(pos, VEC2_TILE);
					render->setDrawColor(Color(1, 1, 1, 1));
				}
			}
			pos.setY(pos.getY() + TILE_SIZE_PIXEL);
		}
		pos.setY(0);
		pos.setX(pos.getX() + TILE_SIZE_PIXEL);
	}

	//Draw a grid
	if(debug) {
		render->setDrawColor(Color(1, 1, 1, 0.25f));
		for(int x = startDrawX + TILE_SIZE_PIXEL; x <= SCREEN_X_PIXEL; x += TILE_SIZE_PIXEL) {
			render->drawLine(Vec2i(x, 0), Vec2i(x, SCREEN_Y_PIXEL));
		}
		for(unsigned y = TILE_SIZE_PIXEL; y <= SCREEN_Y_PIXEL; y += TILE_SIZE_PIXEL) {
			render->drawLine(Vec2i(0, y), Vec2i(SCREEN_X_PIXEL, y));
		}
	}
}

//=============================================================================
// Make a new empty world
//=============================================================================
void World::make(unsigned lenght)
{
	delete [] map;
	this->lenght = lenght;

	map = new Tile[lenght * MAP_Y_TILES];
	Memory::memset(map, 0, lenght * MAP_Y_TILES * sizeof(Tile));
}

//=============================================================================
// Shrinks the level
//=============================================================================
void World::shrink(unsigned newLenght)
{
	if(newLenght < lenght) {
		lenght = newLenght;
		Tile *oldMap = map;

		map = new Tile[newLenght * MAP_Y_TILES];
		Memory::memcpy(map, oldMap, newLenght * MAP_Y_TILES * sizeof(Tile));

		delete [] oldMap;
	}
}

//=============================================================================
// Converts screen coordinates to map coordinates
//=============================================================================
void World::pixelToMap(const Vec2i &screen, Vec2i &map)
{
	int x = screen.getX() / TILE_SIZE_PIXEL;
	if(x < 0) x = 0;
	if(x >= static_cast<int>(lenght)) x ? (lenght - 1) : 0;
	map.setX(x);

	int y = screen.getY() / TILE_SIZE_PIXEL;
	if(y < 0) y = 0;
	if(y >= MAP_Y_TILES) y = MAP_Y_TILES - 1;
	map.setY(y);
}

//=============================================================================
// Save the world in the specified file
//=============================================================================
void World::readFromFile(File *file)
{
	delete [] map;

	file->readUnsignedInt(lenght);
	map = new Tile[lenght * MAP_Y_TILES];
	
	//memset(map, 0, lenght * MAP_Y_TILES * sizeof(Tile));

	file->read(reinterpret_cast<unsigned char*>(map), lenght * MAP_Y_TILES * sizeof(Tile));
}

//=============================================================================
// Save the world in the specified file
//=============================================================================
void World::writeToFile(File *file) const
{
	file->writeUnsignedInt(lenght);
	file->write(reinterpret_cast<const unsigned char*>(map), lenght * MAP_Y_TILES * sizeof(Tile));
}

//=============================================================================
// Draws the specified tile
//=============================================================================
void World::drawTile(const Vec2i &pos, unsigned tileId)
{
	unsigned s = tileId % TEX_SIZE_TILES;
	unsigned t = /*TEX_SIZE_TILES - */(tileId / TEX_SIZE_TILES)/* - 1*/;

	Vec2f st1(s / static_cast<float>(TEX_SIZE_TILES), t / static_cast<float>(TEX_SIZE_TILES));
	s++;
	t++;
	Vec2f st2(s / static_cast<float>(TEX_SIZE_TILES), t / static_cast<float>(TEX_SIZE_TILES));

	worldTex->drawTile(pos, VEC2_TILE, st1/* + Vec2f(0.001f, 0.001f)*/, st2/* - Vec2f(0.001f, 0.001f)*/);
}