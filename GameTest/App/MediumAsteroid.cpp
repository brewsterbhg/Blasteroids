#include "stdafx.h"
#include "MediumAsteroid.h"
#include "app.h"
#include <cmath>

using namespace App;

class MY_CONSTS
{
public:
	static const float MD_VEL;
};

//Set up float consts
const float MY_CONSTS::MD_VEL = 2.5;

void MediumAsteroid::New(float direction, float x, float y)
{
	//Set whether the asteroid is active or not
	m_isActive = true;

	//Set direction and coordinates
	m_direction = direction;
	_x = x;
	_y = y;

	//Set the HP
	m_health = MEDIUM_ASTEROID_HEALTH;

	//Set inital colours
	_R = 1.0;
	_G = 1.0;
	_B = 1.0;

	//Render the asteroid
	Render();
}

void MediumAsteroid::Render()
{
	if (m_isActive) {
		float firstPointX = _x - 30;
		float firstPointY = _y + 30;
		float secondPointX = _x - 35;
		float secondPointY = _y + 15;
		float thirdPointX = _x - 35;
		float thirdPointY = _y - 20;
		float fourthPointX = _x - 15;
		float fourthPointY = _y - 20;
		float fifthPointX = _x - 20;
		float fifthPointY = _y - 30;
		float sixthPointX = _x + 15;
		float sixthPointY = _y - 30;
		float seventhPointX = _x + 30;
		float seventhPointY = _y - 15;
		float eighthPointX = _x + 10;
		float eightPointY = _y + 10;
		float ninthPointX = _x + 30;
		float ninthPointY = _y + 20;
		float tenthPointX = _x + 20;
		float tenthPointY = _y + 30;

		//Get the needed trig values
		float cosRadians = cos(m_angle * (PI / 180));
		float sinRadians = sin(m_angle * (PI / 180));
		float xNew;
		float yNew;

		//Translate
		firstPointX -= _x;
		firstPointY -= _y;
		secondPointX -= _x;
		secondPointY -= _y;
		thirdPointX -= _x;
		thirdPointY -= _y;
		fourthPointX -= _x;
		fourthPointY -= _y;
		fifthPointX -= _x;
		fifthPointY -= _y;
		sixthPointX -= _x;
		sixthPointY -= _y;
		seventhPointX -= _x;
		seventhPointY -= _y;
		eighthPointX -= _x;
		eightPointY -= _y;
		ninthPointX -= _x;
		ninthPointY -= _y;
		tenthPointX -= _x;
		tenthPointY -= _y;

		xNew = firstPointX * cosRadians - firstPointY * sinRadians;
		yNew = firstPointX * sinRadians + firstPointY * cosRadians;
		firstPointX = xNew + _x;
		firstPointY = yNew + _y;

		xNew = secondPointX * cosRadians - secondPointY * sinRadians;
		yNew = secondPointX * sinRadians + secondPointY * cosRadians;
		secondPointX = xNew + _x;
		secondPointY = yNew + _y;

		xNew = thirdPointX * cosRadians - thirdPointY * sinRadians;
		yNew = thirdPointX * sinRadians + thirdPointY * cosRadians;
		thirdPointX = xNew + _x;
		thirdPointY = yNew + _y;

		xNew = fourthPointX * cosRadians - fourthPointY * sinRadians;
		yNew = fourthPointX * sinRadians + fourthPointY * cosRadians;
		fourthPointX = xNew + _x;
		fourthPointY = yNew + _y;

		xNew = fifthPointX * cosRadians - fifthPointY * sinRadians;
		yNew = fifthPointX * sinRadians + fifthPointY * cosRadians;
		fifthPointX = xNew + _x;
		fifthPointY = yNew + _y;

		xNew = sixthPointX * cosRadians - sixthPointY * sinRadians;
		yNew = sixthPointX * sinRadians + sixthPointY * cosRadians;
		sixthPointX = xNew + _x;
		sixthPointY = yNew + _y;

		xNew = seventhPointX * cosRadians - seventhPointY * sinRadians;
		yNew = seventhPointX * sinRadians + seventhPointY * cosRadians;
		seventhPointX = xNew + _x;
		seventhPointY = yNew + _y;

		xNew = eighthPointX * cosRadians - eightPointY * sinRadians;
		yNew = eighthPointX * sinRadians + eightPointY * cosRadians;
		eighthPointX = xNew + _x;
		eightPointY = yNew + _y;

		xNew = ninthPointX * cosRadians - ninthPointY * sinRadians;
		yNew = ninthPointX * sinRadians + ninthPointY * cosRadians;
		ninthPointX = xNew + _x;
		ninthPointY = yNew + _y;

		xNew = tenthPointX * cosRadians - tenthPointY * sinRadians;
		yNew = tenthPointX * sinRadians + tenthPointY * cosRadians;
		tenthPointX = xNew + _x;
		tenthPointY = yNew + _y;

		//Draw the shape of the asteroid
		DrawLine(firstPointX, firstPointY, secondPointX, secondPointY, (_R), (_G), (_B));
		DrawLine(secondPointX, secondPointY, thirdPointX, thirdPointY, (_R), (_G), (_B));
		DrawLine(thirdPointX, thirdPointY, fourthPointX, fourthPointY, (_R), (_G), (_B));
		DrawLine(fourthPointX, fourthPointY, fifthPointX, fifthPointY, (_R), (_G), (_B));
		DrawLine(fifthPointX, fifthPointY, sixthPointX, sixthPointY, (_R), (_G), (_B));
		DrawLine(sixthPointX, sixthPointY, seventhPointX, seventhPointY, (_R), (_G), (_B));
		DrawLine(seventhPointX, seventhPointY, eighthPointX, eightPointY, (_R), (_G), (_B));
		DrawLine(eighthPointX, eightPointY, ninthPointX, ninthPointY, (_R), (_G), (_B));
		DrawLine(ninthPointX, ninthPointY, tenthPointX, tenthPointY, (_R), (_G), (_B));
		DrawLine(tenthPointX, tenthPointY, firstPointX, firstPointY, (_R), (_G), (_B));
	}
}

void MediumAsteroid::Update()
{
	Render();

	//Spin the asteroid
	m_angle += 1;

	//Reset the angle if fully rotated
	if (m_angle == 360) {
		m_angle = 0;
	}

	//Account for direction
	m_trajectoryX = MY_CONSTS::MD_VEL * sin(m_direction * (PI / 180));
	m_trajectoryY = MY_CONSTS::MD_VEL * cos(m_direction * (PI / 180));

	//Move in the direction the asteroid is facing
	_x += m_trajectoryX;
	_y += m_trajectoryY;

	CheckForWrap(_x, _y, MD_WIDTH / 2);
}

void MediumAsteroid::AsteroidHit(int numOfHits)
{
	//For the number of hits
	for (int i = 0; i < numOfHits; i++)
	{
		//Decrement health
		m_health--;

		//If first hit, put green up to max and blue and red to 0
		if (m_health == MEDIUM_ASTEROID_HEALTH - 1)
		{
			_G = 1.0;
			_B = 0.0;
			_R = 0.0;
		}

		//If not first hit, slowly decrement green value
		else if (_G > 0 && _G <= 225)
			_G -= 0.17;

		//Increment the red value higher to give the impression of damage
		_R += 0.17;

		//If the asteroid has been destroyed
		if (m_health <= 0) {
			DestroyAsteroid();
		}
	}
}

void MediumAsteroid::DestroyAsteroid()
{
	m_isActive = false;
}

bool MediumAsteroid::CheckCollision(float objectX, float objectY, float radius)
{
	bool collide = false;

	//Check collisions for asteroid if active
	if (m_isActive) 
	{
		collide = hypot(_x - objectX, _y - objectY) <= (25 + radius);
	}

	return collide;
}

float MediumAsteroid::GetX()
{
	return _x;
}

float MediumAsteroid::GetY()
{
	return _y;
}

bool MediumAsteroid::GetIsActive()
{
	return m_isActive;
}