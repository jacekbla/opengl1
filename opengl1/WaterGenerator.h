#include "Utility.h"
#include "WaterTile.h"
#include "Loader.h"


#ifndef WATER_GENERATOR_H
#define WATER_GENERATOR_H

class WaterGenerator
{
public:
	explicit WaterGenerator();
	~WaterGenerator();

	static WaterTile generate(int p_gridCount, float p_x, float p_height, float p_z, Loader& p_loader);

private:
	static const int _VERTICES_PER_SQUARE = 3 * 2;	//2 triangles
	static const int _VERTEX_SIZE_BYTES = 8 + 4;		//x, z position + indicator

	static std::vector<std::pair<glm::fvec2, glm::fvec4>> createMeshData(int p_gridCount, int p_totalVertexCount);
	static void storeGridSquare(int p_col, int p_row, std::vector<std::pair<glm::fvec2, glm::fvec4>>& p_data);
	static void storeTriangle(glm::fvec2* p_cornerPos, std::vector<std::pair<glm::fvec2, glm::fvec4>>& p_data, bool p_left);
	static glm::fvec2* calculateCornerPositions(int p_col, int p_row);
	static glm::fvec4 getIndicators(int p_currentVertex, glm::fvec2* p_vertexPositions, int p_vertex1, int p_vertex2);
};

#endif WATER_GENERATOR_H