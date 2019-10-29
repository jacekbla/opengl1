#include "Utility.h"

#ifndef CAMERA_H
#define CAMERA_H


class Camera
{
public:
	explicit Camera() : Camera(0.0f, 0.0f, 0.0f) {};
	explicit Camera(float p_pitch, float p_yaw, float p_roll) : _pitch(p_pitch), _yaw(p_yaw), _roll(p_roll) {};
	~Camera();

	glm::vec3 getPosition() const;
	float getPitch() const;
	float getYaw() const;
	float getRoll() const;
	void setPosition(glm::vec3 p_position);

	void invertPitch();
	void move();

private:
	glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
	float _pitch = 0.0f;
	float _yaw = 0.0f;
	float _roll = 0.0f;
};


#endif CAMERA_H