#include "stdafx.h"
#include "BasicShot.h"
#include <cmath>
#include "app.h"

using namespace App;

void BasicShot::New(float shipX, float shipY, float angle)
{
	//Set the initial coordinates to the ships position
	_x = shipX;
	_y = shipY;

	//Set the angle
	m_angle = angle;

	//Set initial states
	m_isActive = true;

	//Set inital timer on the shot
	m_timer = TIME_TO_LIVE;
}

void BasicShot::Render()
{
	//If the shot is active
	if (m_isActive) 
	{
		//Draw the bullet
		DrawLine(_x, _y, _x + BULLET_LENGTH, _y + BULLET_LENGTH);

		UpdateShot();
	}
}

void BasicShot::UpdateShot()
{
	//If the shot is active
	if (m_isActive) 
	{
		//Account for direction
		m_trajectoryX = BULLET_SPEED * sin(m_angle * (PI / 180));
		m_trajectoryY = BULLET_SPEED * cos(m_angle * (PI / 180));

		//Move in the direction the ship is facing
		_x -= m_trajectoryX;
		_y += m_trajectoryY;

		//Decrement timer
		m_timer--;

		//If timer is complete, deactivate the shot
		if (m_timer == 0)
			DeactivateShot();

		CheckForWrap(_x, _y, 5);
	}
}

bool BasicShot::CheckIfActive()
{
	return m_isActive;
}

void BasicShot::DeactivateShot()
{
	m_isActive = false;
}

float BasicShot::BulletX()
{
	return _x;
}

float BasicShot::BulletY()
{
	return _y;
}