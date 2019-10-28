#include "TexturedModel.h"


TexturedModel::~TexturedModel()
{
}

RawModel TexturedModel::getRawModel()
{
	return _rawModel;
}

ModelTexture &TexturedModel::getTexture()
{
	return _texture;
}

bool TexturedModel::operator<(const TexturedModel& p_other) const
{
	return this < &p_other;//_texture.getTextureID() < p_other.getTexture().getTextureID();
}