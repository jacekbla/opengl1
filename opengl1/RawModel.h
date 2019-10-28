#ifndef RAW_MODEL_H
#define RAW_MODEL_H

class RawModel
{
public:
	RawModel(int p_vao_id, int p_vertex_count)
		: _vao_id(p_vao_id),
		_vertex_count(p_vertex_count) {};
	RawModel() : RawModel(0, 0) {};
	~RawModel();
	int getVaoID();
	int getVertexCount();

private:
	int _vao_id;
	int _vertex_count;
};

#endif RAW_MODEL_H
