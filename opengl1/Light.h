#include "Utility.h"

#ifndef LIGHT_H
#define LIGHT_H


class Light
{
public:
	Light(glm::vec3 p_position, glm::vec3 p_color, glm::vec3 p_skyColor, float p_strenght) : _position(p_position), _color(p_color), _skyColor(p_skyColor), _strenght(p_strenght) {};
	~Light();

	glm::vec3 &getPostion();
	glm::vec3 &getColor();
	glm::vec3 &getSkyColor();
	float &getStrenght();
	void setPostion(glm::vec3 p_position);
	void setColor(glm::vec3 p_color);
	void setSkyColor(glm::vec3 p_skyColor);
	void setStrenght(float p_strenght);

private:
	glm::vec3 _position;
	glm::vec3 _color;
	glm::vec3 _skyColor;
	float _strenght;
};


#endif LIGHT_H