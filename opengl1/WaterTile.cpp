#include "WaterTile.h"

const float WaterTile::TILE_SIZE = 5.0f;

WaterTile::WaterTile(float p_centerX, float p_centerZ, float p_height)
{
	_x = p_centerX;
	_z = p_centerZ;
	_height = p_height;
}


WaterTile::~WaterTile()
{
}

float WaterTile::_getHeight()
{
	return _height;
}

float WaterTile::_getX()
{
	return _x;
}

float WaterTile::_getZ()
{
	return _z;
}
