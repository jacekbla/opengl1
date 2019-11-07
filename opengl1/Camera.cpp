#include "Camera.h"



Camera::~Camera()
{
}

glm::vec3 Camera::getPosition() const
{
	return _position;
}

float Camera::getPitch() const
{
	return _pitch;
}

float Camera::getYaw() const
{
	return _yaw;
}

float Camera::getRoll() const
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
	if (GetKeyState(' ') & 0x8000)
	{
		_position.y += 0.2f;
	}
	if (GetKeyState('C') & 0x8000)
	{
		_position.y -= 0.2f;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		_pitch += 0.5f;
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		_pitch -= 0.5f;
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		_yaw -= 0.5f;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		_yaw += 0.5f;
	}
	POINT mousePos;
	GetCursorPos(&mousePos);
	if (GetKeyState(VK_RBUTTON) & 0x100)
	{
		int diffX = mousePos.x - _currMouseX;
		int diffY = mousePos.y - _currMouseY;
		if (diffX != 0 || diffY != 0)
		{
			_yaw += diffX * 0.1f;
			_pitch += diffY * 0.1f;
		}
	}
	_currMouseX = mousePos.x;
	_currMouseY = mousePos.y;
}
