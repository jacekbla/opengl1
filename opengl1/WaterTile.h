#ifndef WATERTILE_H
#define WATERTILE_H

class WaterTile
{
public:
	static const float TILE_SIZE;

	WaterTile(float p_centerX, float p_centerZ, float p_height);
	~WaterTile();

	float _getHeight();
	float _getX();
	float _getZ();

private:
	float _height;
	float _x;
	float _z;
};


#endif WATERTILE_H