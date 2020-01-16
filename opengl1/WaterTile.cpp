#include "WaterTile.h"

const float WaterTile::TILE_SIZE = 0.1f;

WaterTile::~WaterTile()
{
}

float WaterTile::getHeight() const
{
	return _height;
}

float WaterTile::getX() const
{
	return _x;
}

float WaterTile::getZ() const
{
	return _z;
}

int WaterTile::getVertexCount() const
{
	return _quad.getVertexCount();
}

int WaterTile::getVaoID() const
{
	return _quad.getVaoID();
}
