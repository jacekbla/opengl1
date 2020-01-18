#include "WaterGenerator.h"


WaterGenerator::WaterGenerator()
{
}


WaterGenerator::~WaterGenerator()
{
}

WaterTile WaterGenerator::generate(int p_gridCount, float p_x, float p_height, float p_z, Loader& p_loader)
{
	int totalVertexCount = p_gridCount * p_gridCount * _VERTICES_PER_SQUARE;
	std::vector<std::pair<glm::fvec2, glm::fvec4>> waterMeshData = createMeshData(p_gridCount, totalVertexCount);
	std::vector<float> positions;
	std::vector<float> indicators;
	for(std::pair<glm::fvec2, glm::fvec4> pair : waterMeshData)
	{
		positions.push_back(pair.first[0]);
		positions.push_back(pair.first[1]);

		indicators.push_back(pair.second[0]);
		indicators.push_back(pair.second[1]);
		indicators.push_back(pair.second[2]);
		indicators.push_back(pair.second[3]);
	}
	std::reverse(positions.begin(), positions.end());
	std::reverse(indicators.begin(), indicators.end());
	RawModel rawModel = p_loader.loadWaterToVAO(positions, indicators);
	return WaterTile(p_x, p_z, p_height, rawModel);
}

std::vector<std::pair<glm::fvec2, glm::fvec4>> WaterGenerator::createMeshData(int p_gridCount, int p_totalVertexCount)
{
	int byteSize = _VERTEX_SIZE_BYTES * p_totalVertexCount;
	std::vector<std::pair<glm::fvec2, glm::fvec4>> data;
	for (int row = 0; row < p_gridCount; row++) {
		for (int col = 0; col < p_gridCount; col++) {
			storeGridSquare(col, row, data);
		}
	}
	return data;
}

void WaterGenerator::storeGridSquare(int p_col, int p_row, std::vector<std::pair<glm::fvec2, glm::fvec4>>& p_data)
{
	glm::fvec2* cornerPos = calculateCornerPositions(p_col, p_row);
	storeTriangle(cornerPos, p_data, true);
	storeTriangle(cornerPos, p_data, false);
	delete cornerPos;
}

void WaterGenerator::storeTriangle(glm::fvec2 * p_cornerPos, std::vector<std::pair<glm::fvec2, glm::fvec4>>& p_data, bool p_left)
{
	int index0 = p_left ? 0 : 2;
	int index1 = 1;
	int index2 = p_left ? 2 : 3;
	p_data.push_back(std::pair<glm::fvec2, glm::fvec4>(p_cornerPos[index0], getIndicators(index0, p_cornerPos, index1, index2)));
	p_data.push_back(std::pair<glm::fvec2, glm::fvec4>(p_cornerPos[index1], getIndicators(index1, p_cornerPos, index2, index0)));
	p_data.push_back(std::pair<glm::fvec2, glm::fvec4>(p_cornerPos[index2], getIndicators(index2, p_cornerPos, index0, index1)));
}

glm::fvec2 * WaterGenerator::calculateCornerPositions(int p_col, int p_row)
{
	glm::fvec2* vertices = new glm::fvec2[4];
	vertices[0] = glm::fvec2(p_col, p_row);
	vertices[1] = glm::fvec2(p_col, p_row + 1);
	vertices[2] = glm::fvec2(p_col + 1, p_row);
	vertices[3] = glm::fvec2(p_col + 1, p_row + 1);
	return vertices;
}

glm::fvec4 WaterGenerator::getIndicators(int p_currentVertex, glm::fvec2 * p_vertexPositions, int p_vertex1, int p_vertex2)
{
	glm::fvec2 currentVertexPos = p_vertexPositions[p_currentVertex];
	glm::fvec2 vertex1Pos = p_vertexPositions[p_vertex1];
	glm::fvec2 vertex2Pos = p_vertexPositions[p_vertex2];
	glm::fvec2 offset1(vertex1Pos.x - currentVertexPos.x, vertex1Pos.y - currentVertexPos.y);
	glm::fvec2 offset2(vertex2Pos.x - currentVertexPos.x, vertex2Pos.y - currentVertexPos.y);

	return glm::fvec4(offset1.x, offset1.y, offset2.x, offset2.y);
}
