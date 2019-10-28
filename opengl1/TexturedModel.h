#include "Utility.h"
#include "RawModel.h"
#include "ModelTexture.h"

#ifndef TEXTUREMODEL_H
#define TEXTUREMODEL_H

class TexturedModel
{
public:
	TexturedModel() : TexturedModel(RawModel(), ModelTexture()) {};
	TexturedModel(RawModel model, ModelTexture texture) : _rawModel(model), _texture(texture) {};
	~TexturedModel();

	RawModel getRawModel();
	ModelTexture &getTexture();
	bool operator<(const TexturedModel& p_other) const;

private:
	RawModel _rawModel;
	ModelTexture _texture;
};


#endif TEXTUREMODEL_H