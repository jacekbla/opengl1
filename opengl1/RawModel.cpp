#include "RawModel.h"

RawModel::~RawModel()
{
}

int RawModel::getVaoID()
{
	return _vao_id;
}

int RawModel::getVertexCount()
{
	return _vertex_count;
}
