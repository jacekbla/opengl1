#include "Entity.h"



Entity::~Entity()
{
}

TexturedModel Entity::getModel()
{
	return _model;
}

glm::vec3 Entity::getPosition()
{
	return _position;
}

float Entity::getRotX()
{
	return _rotX;
}

float Entity::getRotY()
{
	return _rotY;
}

float Entity::getRotZ()
{
	return _rotZ;
}

float Entity::getScale()
{
	return _scale;
}

void Entity::setModel(TexturedModel p_model)
{
	_model = p_model;
}

void Entity::setPosition(glm::vec3 p_position)
{
	_position = p_position;
}

void Entity::setRotX(float p_rotX)
{
	_rotX = p_rotX;
}

void Entity::setRotY(float p_rotY)
{
	_rotY = p_rotY;
}

void Entity::setRotZ(float p_rotZ)
{
	_rotZ = p_rotZ;
}

void Entity::setScale(float p_scale)
{
	_scale = p_scale;
}

void Entity::increasePosition(float p_dx, float p_dy, float p_dz)
{
	_position.x += p_dx;
	_position.y += p_dy;
	_position.z += p_dz;
}

void Entity::increaseRotation(float p_dx, float p_dy, float p_dz)
{
	_rotX += p_dx;
	_rotY += p_dy;
	_rotZ += p_dz;
}
