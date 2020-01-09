#include "Utility.h"

#ifndef LIGHT_H
#define LIGHT_H


class Light
{
public:
	Light(glm::vec3 p_position, glm::vec3 p_color, glm::vec3 p_skyColor) : _position(p_position), _color(p_color), _skyColor(p_skyColor) {};
	~Light();

	glm::vec3 &getPostion();
	glm::vec3 &getColor();
	glm::vec3 &getSkyColor();
	void setPostion(glm::vec3 p_position);
	void setColor(glm::vec3 p_color);
	void setSkyColor(glm::vec3 p_skyColor);

private:
	glm::vec3 _position;
	glm::vec3 _color;
	glm::vec3 _skyColor;
};


#endif LIGHT_H