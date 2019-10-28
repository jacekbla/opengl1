#include "Camera.h"



Camera::Camera()
{
}

Camera::Camera(float p_pitch, float p_yaw, float p_roll)
{
	_pitch = p_pitch;
	_yaw = p_yaw;
	_roll = p_roll;
}


Camera::~Camera()
{
}

glm::vec3 Camera::getPosition()
{
	return _position;
}

float Camera::getPitch()
{
	return _pitch;
}

float Camera::getYaw()
{
	return _yaw;
}

float Camera::getRoll()
{
	return _roll;
}

void Camera::setPosition(glm::vec3 p_position)
{
	_position = p_position;
}

void Camera::invertPitch()
{
	_pitch = -_pitch;
}

void Camera::move()
{
	if (GetKeyState('W') & 0x8000)
	{
		_position.z -= 0.2f;
	}
	if (GetKeyState('D') & 0x8000)
	{
		_position.x += 0.2f;
	}
	if (GetKeyState('A') & 0x8000)
	{
		_position.x -= 0.2f;
	}
	if (GetKeyState('S') & 0x8000)
	{
		_position.z += 0.2f;
	}
	if (GetKeyState('Z') & 0x8000)
	{
		_pitch += 0.2f;
	}
	if (GetKeyState('X') & 0x8000)
	{
		_pitch -= 0.2f;
	}
	if (GetKeyState(' ') & 0x8000)
	{
		_position.y += 0.2f;
	}
	if (GetKeyState('C') & 0x8000)
	{
		_position.y -= 0.2f;
	}
}
