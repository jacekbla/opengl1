#include "ModelTexture.h"



ModelTexture::~ModelTexture()
{
}

int ModelTexture::getTextureID()
{
	return _textureID;
}

float &ModelTexture::getShineDamper()
{
	return _shineDamper;
}

float &ModelTexture::getReflectivity()
{
	return _reflectivity;
}

void ModelTexture::setShineDamper(float p_shineDamper)
{
	_shineDamper = p_shineDamper;
}

void ModelTexture::setReflectivity(float p_reflectivity)
{
	_reflectivity = p_reflectivity;
}
