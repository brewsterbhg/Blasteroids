#include "stdafx.h"
#include "Player.h"
#include "app.h"
#include <cmath>

using namespace App;

class MY_CONSTS
{
public:
	static const float ACCELERATION_RATE;
	static const float DRAG_RATE;
	static const float MAX_ACCELERATION;
};
//Set up float consts
const float MY_CONSTS::ACCELERATION_RATE = 0.005f;
const float MY_CONSTS::DRAG_RATE = 0.008f;
const float MY_CONSTS::MAX_ACCELERATION = 0.1;

Player::Player(int &lives) : m_currentLives(lives)
{
}

void Player::New()
{
	//Set up initial ship values
	m_isAlive = true;
	m_isBoosting = false;
	m_isFiring = false;

	//Ship movement variables
	m_angle = 0;
	m_currentAcceleration = 0;
	_x = 525;
	_y = 400;

	//If the player owns advanced armour, add a hit
	if (m_currentArmourLevel == ADVANCED_HULL)
		AddArmour();
}

void Player::Render()
{
	//Render the ship if alive
	if (m_isAlive)
	{
		//Render only when modulo hits for blinky effect
		if (m_invincibilityTimer == 0 || m_invincibilityTimer % 8 == 0)
		{
			//Ship sides
			float shipFrontX = _x;
			float shipFrontY = _y + (SHIP_HEIGHT / 2);
			float shipRightX = _x + (SHIP_WIDTH / 2);
			float shipRightY = _y - (SHIP_HEIGHT / 2);
			float shipLeftX = _x - (SHIP_WIDTH / 2);
			float shipLeftY = _y - (SHIP_HEIGHT / 2);
			float shipBottomX = _x;
			float shipBottomY = _y - (SHIP_HEIGHT / 4);

			//Get the needed trig values
			float cosRadians = cos(m_angle * (PI / 180));
			float sinRadians = sin(m_angle * (PI / 180));
			float xNew;
			float yNew;

			//Translate
			shipFrontX -= _x;
			shipFrontY -= _y;
			shipLeftX -= _x;
			shipLeftY -= _y;
			shipRightX -= _x;
			shipRightY -= _y;
			shipBottomX -= _x;
			shipBottomY -= _y;

			//Set new front
			xNew = shipFrontX * cosRadians - shipFrontY * sinRadians;
			yNew = shipFrontX * sinRadians + shipFrontY * cosRadians;
			shipFrontX = xNew + _x;
			shipFrontY = yNew + _y;

			//Set new left
			xNew = shipLeftX * cosRadians - shipLeftY * sinRadians;
			yNew = shipLeftX * sinRadians + shipLeftY * cosRadians;
			shipLeftX = xNew + _x;
			shipLeftY = yNew + _y;

			//Set new right
			xNew = shipRightX * cosRadians - shipRightY * sinRadians;
			yNew = shipRightX * sinRadians + shipRightY * cosRadians;
			shipRightX = xNew + _x;
			shipRightY = yNew + _y;

			//Set new bottom
			xNew = shipBottomX * cosRadians - shipBottomY * sinRadians;
			yNew = shipBottomX * sinRadians + shipBottomY * cosRadians;
			shipBottomX = xNew + _x;
			shipBottomY = yNew + _y;

			//Draw the shape of the ship
			DrawLine(shipLeftX, shipLeftY, shipFrontX, shipFrontY);
			DrawLine(shipFrontX, shipFrontY, shipRightX, shipRightY);
			DrawLine(shipRightX, shipRightY, shipBottomX, shipBottomY);
			DrawLine(shipBottomX, shipBottomY, shipLeftX, shipLeftY);

			m_firePointX = shipFrontX;
			m_firePointY = shipFrontY;
		}
	}
	//If dead, render an explosion
	else 
	{
		//Render only when modulo hits for blinky effect
		if (m_invincibilityTimer % 5 == 0)
		{
			//Explosion points
			float ePointOneX = _x;
			float ePointOneY = _y + 25;
			float ePointTwoX = _x + 25;
			float ePointTwoY = _y;
			float ePointThreeX = _x + 25;
			float ePointThreeY = _y - 25;
			float ePointFourX = _x - 25;
			float ePointFourY = _y - 25;
			float ePointFiveX = _x - 25;
			float ePointFiveY = _y;
			float ePointSixX = _x;
			float ePointSixY = _y - 25;
			float ePointSevenX = _x - 25;
			float ePointSevenY = _y + 25;
			float ePointEightX = _x + 25;
			float ePointEightY = _y + 25;

			//Draw the points
			DrawLine(_x, _y, ePointOneX, ePointOneY, (1.0), (1.0), (0.0));
			DrawLine(_x, _y, ePointTwoX, ePointTwoY, (1.0), (1.0), (0.0));
			DrawLine(_x, _y, ePointThreeX, ePointThreeY, (1.0), (1.0), (0.0));
			DrawLine(_x, _y, ePointFourX, ePointFourY, (1.0), (1.0), (0.0));
			DrawLine(_x, _y, ePointFiveX, ePointFiveY, (1.0), (1.0), (0.0));
			DrawLine(_x, _y, ePointSixX, ePointSixY, (1.0), (1.0), (0.0));
			DrawLine(_x, _y, ePointSevenX, ePointSevenY, (1.0), (1.0), (0.0));
			DrawLine(_x, _y, ePointEightX, ePointEightY, (1.0), (1.0), (0.0));
		}
	}
}

void Player::RotateShip(int direction)
{
	//Get the direction and adjust angle accordingly
	switch (direction) 
	{
		case LEFT:
			m_angle += ROTATION_AMOUNT;
			break;
		case RIGHT:
			m_angle -= ROTATION_AMOUNT;
			break;
	}
}

void Player::Boost()
{
	//Set boosting state
	m_isBoosting = true;

	//Increment acceleration until max is reached
	if (m_currentAcceleration < MY_CONSTS::MAX_ACCELERATION) {
		m_currentAcceleration += MY_CONSTS::ACCELERATION_RATE;
	}
	else {
		m_currentAcceleration = MY_CONSTS::MAX_ACCELERATION;
	}
}

void Player::StopBoost()
{
	m_isBoosting = false;
}

void Player::Decelerate()
{
	if (m_currentAcceleration > 0)
	{
		m_currentAcceleration -= MY_CONSTS::ACCELERATION_RATE;
	}
}

void Player::UpdateShip()
{
	//Decrement the invincibility timer if above zero
	if (m_invincibilityTimer > 0)
	{
		m_invincibilityTimer--;
	}
	//If timer is zero
	else if (m_invincibilityTimer == 0 && !m_isAlive)
	{
		//Set new timer so you don't die as soon as you spawn
		m_invincibilityTimer = INVINCIBILITY_TIME;

		Reset();
	}

	//If alive
	if (m_isAlive) {
		//If not boosting
		if (!m_isBoosting) {
			//If current acceleration is above 0, begin to decelerate
			if (m_currentAcceleration > 0) {
				m_currentAcceleration -= MY_CONSTS::DRAG_RATE;
			}
			//Make sure we don't go into negative acceleration
			else {
				m_currentAcceleration = 0;
			}
		}

		//Determine velocity with current acceleration rate and trig
		m_velX -= m_currentAcceleration * sin(m_angle * (PI / 180));
		m_velY += m_currentAcceleration * cos(m_angle * (PI / 180));

		//Update position accounting for velocity
		_x += m_velX;
		_y += m_velY;

		//Slow down velocity with drag rate
		m_velX = m_velX - m_velX * MY_CONSTS::DRAG_RATE;
		m_velY = m_velY - m_velY * MY_CONSTS::DRAG_RATE;

		//Render any current bullets
		for (int s = 0; s < MAX_SHOTS; s++)
		{
			//If they have the basic shot
			if (m_currentWeaponLevel == BASIC_SHOT)
			{
				m_shots[s].Render();
			}
			//If they have the spread shot
			if (m_currentWeaponLevel == SPREAD_SHOT)
			{
				m_shots[s].Render();
				m_spreadShotOne[s].Render();
				m_spreadShotTwo[s].Render();
			}
		}

		//If buffering
		if (m_isBuffering)
		{
			//Decrement current buffer
			m_currentBuffer--;

			//If buffer is zero, reset bool
			if (m_currentBuffer == 0)
				m_isBuffering = false;
		}

		//Check for wrap
		CheckForWrap(_x, _y, SHIP_WIDTH / 2);
	}
	Render();
}

void Player::FirePrimaryWeapons()
{
	//Make sure the player is alive and shots aren't buffering
	if (m_isAlive && !m_isBuffering)
	{
		//Check the bullet buffer
		int shotIndex = 0;
		bool shot = false;

		//Go through the shots array
 		for (int s = 0; s < MAX_SHOTS; s++) 
		{
			//If they have the basic shot
			if (m_currentWeaponLevel == BASIC_SHOT) 
			{
				//If shot at index is not active
				if (!m_shots[s].CheckIfActive())
				{
					//Set that a shot is available and the index
					shotIndex = s;
					shot = true;
				}
			}
			//If they have the spread shot
			else if (m_currentWeaponLevel == SPREAD_SHOT)
			{
				if (!m_shots[s].CheckIfActive())
				{
					shotIndex = s;
					shot = true;
				}
				if (!m_spreadShotOne[s].CheckIfActive())
				{
					shotIndex = s;
					shot = true;
				}
				if (!m_spreadShotTwo[s].CheckIfActive())
				{
					shotIndex = s;
					shot = true;
				}
			}
		}
		if (shot)
		{
			//If a shot was found, create the bullet
			if (m_currentWeaponLevel == BASIC_SHOT)
			{
				m_shots[shotIndex].New(m_firePointX, m_firePointY, m_angle);
			}
			else if (m_currentWeaponLevel == SPREAD_SHOT)
			{
				m_shots[shotIndex].New(m_firePointX, m_firePointY, m_angle);
				m_spreadShotOne[shotIndex].New(m_firePointX, m_firePointY, m_angle + 30);
 				m_spreadShotTwo[shotIndex].New(m_firePointX, m_firePointY, m_angle - 30);
			}

			//Set buffers
			m_isBuffering = true;
			m_currentBuffer = BUFFER_TIME;
		}
	}
}

void Player::ShipHit()
{
	//If the user has advanced hull and an extra hit, absorb it and give invincibility
	if (m_currentArmourLevel == ADVANCED_HULL && m_extraHit == 1)
	{
		PlaySound("Sounds/Beep20.wav");
		m_invincibilityTimer = EXPLOSION_TIME;
		m_extraHit--;
	}
	//Only call if ship is alive and invincibility timer is not on
	else if (m_isAlive && m_invincibilityTimer == 0)
	{
		m_isAlive = false;

		//Play crash noise
		if (!IsSoundPlaying("Sounds/Explosion4.wav")) 
		{
			PlaySound("Sounds/Explosion4.wav");
		}

		//Start invincibility timer
		m_invincibilityTimer = EXPLOSION_TIME;

		//Decrement lives
		m_currentLives--;
	}
}

void Player::DeactivateBullet(int index)
{
	//If they have the basic shot
	if (m_currentWeaponLevel == BASIC_SHOT) 
	{
		if (m_shots[index].CheckIfActive())
		{
			m_shots[index].DeactivateShot();
		}
	}
	//If they have the spread shot
	else if (m_currentWeaponLevel == SPREAD_SHOT) 
	{
		if (m_shots[index].CheckIfActive())
		{
			m_shots[index].DeactivateShot();
		}
		if (m_spreadShotOne[index].CheckIfActive())
		{
			m_spreadShotOne[index].DeactivateShot();
		}
		if (m_spreadShotTwo[index].CheckIfActive())
		{
			m_spreadShotTwo[index].DeactivateShot();
		}
	}
}

void Player::UpgradeWeapon()
{
	//If basic shot, upgrade to spreadshot
	if (m_currentWeaponLevel == BASIC_SHOT)
	{
		m_currentWeaponLevel = SPREAD_SHOT;
	}
}

void Player::UpgradeArmour()
{
	//If basic shot, upgrade to spreadshot
	if (m_currentArmourLevel == BASIC_HULL)
	{
		m_currentArmourLevel = ADVANCED_HULL;
	}
}

void Player::AddArmour()
{
	if (m_currentArmourLevel == ADVANCED_HULL)
	{
		m_extraHit = 1;
	}
}

void Player::Reset()
{
	//Set up initial ship values
	m_isAlive = true;
	_x = 525;
	_y = 400;
	m_currentAcceleration = 0;
	m_velX = 0;
	m_velY = 0;
}

float Player::GetShipCenterX()
{
	return _x;
}

float Player::GetShipCenterY()
{
	return _y;
}

float Player::GetShipRadius()
{
	return SHIP_WIDTH / 2;
}

float Player::GetBulletX(int index)
{
	//Find the bullet at the index
	if (m_shots[index].CheckIfActive())
	{
		return m_shots[index].BulletX();
	}

	return -9999999.0;
}

float Player::GetBulletY(int index)
{
	//Find the bullet at the index
	if (m_shots[index].CheckIfActive())
	{
		return m_shots[index].BulletY();
	}

	return -9999999.0;
}

std::vector<float> Player::GetSpreadShotX(int index)
{
	std::vector<float> shots;

	if (m_shots[index].CheckIfActive())
	{
		shots.push_back(m_shots[index].BulletX());
	}
	if (m_spreadShotOne[index].CheckIfActive())
	{
		shots.push_back(m_spreadShotOne[index].BulletX());
	}
	if (m_spreadShotTwo[index].CheckIfActive())
	{
		shots.push_back(m_spreadShotTwo[index].BulletX());
	}

	return shots;
}

std::vector<float> Player::GetSpreadShotY(int index)
{
	std::vector<float> shots;

	if (m_shots[index].CheckIfActive())
	{
		shots.push_back(m_shots[index].BulletY());
	}
	if (m_spreadShotOne[index].CheckIfActive())
	{
		shots.push_back(m_spreadShotOne[index].BulletY());
	}
	if (m_spreadShotTwo[index].CheckIfActive())
	{
		shots.push_back(m_spreadShotTwo[index].BulletY());
	}

	return shots;
}


bool Player::IsAlive()
{
	return m_isAlive;
}

int Player::GetWeaponLevel()
{
	return m_currentWeaponLevel;
}

int Player::GetArmourLevel()
{
	return m_currentArmourLevel;
}