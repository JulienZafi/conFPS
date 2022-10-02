
#include "Player.hpp"

#include <Windows.h>

Player::Player(Map const& map)
{
	// Initialize default position
	m_xPos = 14.7;
	m_yPos = 5.09;
	m_orientationAngle = 0.0;
	m_speed = 5.0;
	m_map = map;
}

Player::~Player(){}

double Player::GetXPosition() const
{
	return m_xPos;
}

double Player::GetYPosition() const
{
	return m_yPos;
}

double Player::GetOrientationAngle() const
{
	return m_orientationAngle;
}

void Player::UpdatePlayerPosition(double const &elapsedTime)
{
	// Handle CCW Rotation
	if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
	{
		m_orientationAngle -= (m_speed * 0.75f) * elapsedTime;
	}
	else{}

	// Handle CW Rotation
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
	{
		m_orientationAngle += (m_speed * 0.75f) * elapsedTime;
	}
	else{}

	// Handle Forwards movement & collision
	if (GetAsyncKeyState((unsigned short)'Z') & 0x8000)
	{
		m_xPos += sin(m_orientationAngle) * m_speed * elapsedTime;
		m_yPos += cos(m_orientationAngle) * m_speed * elapsedTime;
		if (m_map.GetMap().c_str()[(int)m_xPos * m_map.GetMapWidth() + (int)m_yPos] == '#')
		{
			m_xPos -= sin(m_orientationAngle) * m_speed * elapsedTime;
			m_yPos -= cos(m_orientationAngle) * m_speed * elapsedTime;
		}
		else{}
	}
	else{}

	// Handle backwards movement & collision
	if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
	{
		m_xPos -= sin(m_orientationAngle) * m_speed * elapsedTime;
		m_yPos -= cos(m_orientationAngle) * m_speed * elapsedTime;
		if (m_map.GetMap().c_str()[(int)m_xPos * m_map.GetMapWidth() + (int)m_yPos] == '#')
		{
			m_xPos += sin(m_orientationAngle) * m_speed * elapsedTime;
			m_yPos += cos(m_orientationAngle) * m_speed * elapsedTime;
		}
		else{}
	}
	else{}
}