#include "Utility.h"

#ifndef MODELTEXTURE_H
#define MODELTEXTURE_H

class ModelTexture
{
public:
	ModelTexture() : ModelTexture(-1) {};
	ModelTexture(int id) : _textureID(id) {};
	~ModelTexture();

	int getTextureID();

	float &getShineDamper();
	float &getReflectivity();
	void setShineDamper(float p_shineDamper);
	void setReflectivity(float p_reflectivity);

private:
	int _textureID;
	float _shineDamper = 1.0f;
	float _reflectivity = 0.0f;
};

#endif MODELTEXTURE_H