#include "Light.h"



Light::~Light()
{
}

glm::vec3 &Light::getPostion()
{
	return _position;
}

glm::vec3 &Light::getColor()
{
	return _color;
}

glm::vec3 &Light::getSkyColor()
{
	return _skyColor;
}

float &Light::getStrenght()
{
	return _strenght;
}

void Light::setPostion(glm::vec3 p_position)
{
	_position = p_position;
}

void Light::setColor(glm::vec3 p_color)
{
	_color = p_color;
}

void Light::setSkyColor(glm::vec3 p_skyColor)
{
	_skyColor = p_skyColor;
}

void Light::setStrenght(float p_strenght)
{
	_strenght = p_strenght;
}
