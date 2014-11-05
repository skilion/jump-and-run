#pragma once
#ifndef __WORLD_H__
#define __WORLD_H__


#include <Platform.h>

#include <Containers/Array.h>
#include <File/File.h>
#include <Math/Rect.h>
#include <Math/Vector.h>
#include <Render/Render.h>
#include <Render/Texture.h>

#include "Const.h"

using namespace myengine;


const unsigned	TEX_SIZE_TILES	= TEX_SIZE_PIXEL / TILE_SIZE_PIXEL;
const unsigned	SCREEN_X_TILES	= SCREEN_X_PIXEL / TILE_SIZE_PIXEL;
const unsigned	SCREEN_Y_TILES	= SCREEN_Y_PIXEL / TILE_SIZE_PIXEL;
const unsigned	MAP_Y_TILES		= SCREEN_Y_PIXEL / TILE_SIZE_PIXEL;

const Vec2i		VEC2_TILE(TILE_SIZE_PIXEL, TILE_SIZE_PIXEL);

enum TileType {
	TILE_NORMAL,
	TILE_SOLID,
	TILE_LETHAL,
	TILE_COIN,
	TILE_ENEMY_STOP,	//Solid to enemy
	TILE_DIAMOND,
	TILE_END_LEVEL
};

const char * const TILE_TYPE_NAMES[] = {
	"normal",
	"solid",
	"lethal",
	"coin",
	"enemy stop",
	"diamond",
	"end level",
};

struct Tile {
	byte		tile1	:8;
	byte		tile2	:8;
	byte		type	:8;
	byte				:8; //empty atm
};


class World
{
public:
		World();
		~World();

	Tile			getTile(const Vec2i &pos) const;
	void			setTile(Vec2i &pos, Tile tile);
		
	void			setWorldTexture(Texture *worldTex);

	bool			collide(const Rect &rect);
	
	void			draw(int pixelShift, bool debug);
	
	void			make(unsigned lenght);
	void			shrink(unsigned newLenght);
	void			pixelToMap(const Vec2i &pixel, Vec2i &map);
	
	void			readFromFile(File *file);
	void			writeToFile(File *file) const;
	

private:
	void		drawTile(const Vec2i &pos, unsigned tileId);

	Tile		*map;
	unsigned	lenght;

	Texture		*worldTex;
};


extern World world;


#endif //!__WORLD_H__