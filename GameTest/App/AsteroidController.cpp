#include "stdafx.h"
#include "AsteroidController.h"

using namespace App;

AsteroidController::AsteroidController(Player &player, int &score, int &cash, int &level, int &state) : m_player(player), m_score(score), m_cash(cash), m_currentLevel(level), m_gameState(state)
{
}

void AsteroidController::New()
{
	//Change the asteroid info depending on the user's current level
	m_asteroidCount = 0;

	//Change depending on the level
	switch (m_currentLevel)
	{
		case LEVEL_ONE:
			m_asteroidCount = 1;
			break;
		case LEVEL_TWO:
			m_asteroidCount = 2;
			break;
		case LEVEL_THREE:
			m_asteroidCount = 3;
			break;
		case LEVEL_FOUR:
			m_asteroidCount = 4;
			break;
		case LEVEL_FIVE:
			m_asteroidCount = 4;
			break;
	}

	//For the number of asteroids passed in, create
	for (int i = 0; i < m_asteroidCount; i++)
	{
		//Randomize direction
		float direction = (float)(rand() % 360 + 1);

		//Randomize starting location
		float x = (float)(rand() % 800 + 1);
		float y = (float)(rand() % 800 + 1);

		m_asteroids[i].New(direction, x, y);
	}
}

void AsteroidController::Update()
{
	//Render if there's any large asteroids
	for (int i = 0; i < m_asteroidCount; i++)
	{
		if (m_asteroids[i].GetIsActive()) 
		{
			m_asteroids[i].Update();
		}
	}

	//Render if there's any medium asteroids
	for (int j = 0; j < (m_asteroidCount * 3); j++)
	{
		if (m_mediumAsteroids[j].GetIsActive())
		{
			m_mediumAsteroids[j].Update();
		}
	}

	//Render if there's any small asteroids
	for (int k = 0; k < (m_asteroidCount * 12); k++)
	{
		if (m_smallAsteroids[k].GetIsActive())
		{
			m_smallAsteroids[k].Update();
		}
	}

	//Check collisions
	CheckPlayerCollision();
	CheckBulletCollision();
}

void AsteroidController::LargeAsteroidDestroyed(float xPos, float yPos)
{
	//Play destruction noise
	PlaySound("Sounds/Explosion10.wav");

	//Increase cash
	m_cash += 100;

	//Since splitting into 3, make sure only 3 are created at a tiime
	int count = 0;

	//Split into medium asteroids
	for (int i = 0; i < (m_asteroidCount * 3); i++)
	{
		if (!m_mediumAsteroids[i].GetIsActive())
		{
			//Randomize direction
			float direction = (float)(rand() % 360 + 1);

			//Create new asteroid
			m_mediumAsteroids[i].New(direction, xPos, yPos);

			//Increment counter
			count++;

			//If 3 are created, break
			if (count == 3) 
			{
				break;
			}
		}
	}
}

void AsteroidController::MediumAsteroidDestroyed(float xPos, float yPos)
{
	//Play destruction noise
	PlaySound("Sounds/Explosion10.wav");

	//Increase cash
	m_cash += 50;

	//Since splitting into 3, make sure only 3 are created at a tiime
	int count = 0;

	//Split into medium asteroids
	for (int i = 0; i < (m_asteroidCount * 12); i++)
	{
		if (!m_smallAsteroids[i].GetIsActive())
		{
			//Randomize direction
			float direction = (float)(rand() % 360 + 1);

			//Create new asteroid
			m_smallAsteroids[i].New(direction, xPos, yPos);

			//Increment counter
			count++;

			//If 3 are created, break
			if (count == 3)
			{
				break;
			}
		}
	}
}

void AsteroidController::SmallAsteroidDestroyed(float xPos, float yPos)
{
	//Play destruction noise
	PlaySound("Sounds/Beep16.wav");

	//Increase cash
	m_cash += 25;

	//Check if the level is complete (all small asteroids are destroyed)
	CheckLevelComplete();
}

void AsteroidController::CheckLevelComplete()
{
	//Flag for level complete
	bool found = false;

	//Check if all large asteroids have been destroyed
	for (int i = 0; i < m_asteroidCount; i++)
	{
		if (m_asteroids[i].GetIsActive())
		{
			found = true;
			break;
		}
	}

	if (!found) {
		//Check if all medium asteroids have been destroyed
		for (int j = 0; j < (m_asteroidCount * 3); j++)
		{
			if (m_mediumAsteroids[j].GetIsActive())
			{
				found = true;
				break;
			}
		}
	}

	if (!found) {
		//Check if all small asteroids have been destroyed
		for (int k = 0; k < (m_asteroidCount * 12); k++)
		{
			if (m_smallAsteroids[k].GetIsActive())
			{
				found = true;
				break;
			}
		}
	}

	//Increase game state (goes from play to post level) if all asteroids are destroyed
	if (!found)
	{
		//Clear the rest of the bullets
		for (int i = 0; i < 10; i++)
		{
			m_player.DeactivateBullet(i);
		}

		//Play victory noise
		PlaySound("Sounds/Sweep4.wav");
		m_currentLevel++;
		m_gameState++;
	}
}

void AsteroidController::CheckPlayerCollision()
{
	float shipCenterX = m_player.GetShipCenterX();
	float shipCenterY = m_player.GetShipCenterY();
	float shipRadius = m_player.GetShipRadius();

	//For large asteroids
	for (int i = 0; i < m_asteroidCount; i++)
	{
		//Check if the asteroid is active
		if (m_asteroids[i].GetIsActive())
		{
			//Check for collisions
			bool collide = m_asteroids[i].CheckCollision(shipCenterX, shipCenterY, shipRadius);

			//If there's a collision
			if (collide) {
				m_player.ShipHit();
			}
		}
	}

	//For medium asteroids
	for (int j = 0; j < (m_asteroidCount * 3); j++)
	{
		//Check if the asteroid is active
		if (m_mediumAsteroids[j].GetIsActive())
		{
			//Check for collisions
			bool collide = m_mediumAsteroids[j].CheckCollision(shipCenterX, shipCenterY, shipRadius);

			//If there's a collision
			if (collide) {
				m_player.ShipHit();
			}
		}
	}

	//For small asteroids
	for (int k = 0; k < (m_asteroidCount * 12); k++)
	{
		//Check if the asteroid is active
		if (m_smallAsteroids[k].GetIsActive())
		{
			//Check for collisions
			bool collide = m_smallAsteroids[k].CheckCollision(shipCenterX, shipCenterY, shipRadius);

			//If there's a collision
			if (collide) {
				m_player.ShipHit();
			}
		}
	}
}

void AsteroidController::CheckBulletCollision()
{
	float bulletX;
	float bulletY;
	float radius;
	std::vector<float> shotX;
	std::vector<float> shotY;

	bool collide = false;

	for (int bulletCount = 0; bulletCount < 10; bulletCount++)
	{
		//Get bullet properties
		bulletX = m_player.GetBulletX(bulletCount);
		bulletY = m_player.GetBulletY(bulletCount);
		//Just use 5 as a buffer for the bullet
		radius = 5.0;

		//For large asteroids
		for (int i = 0; i < m_asteroidCount; i++)
		{
			if (m_asteroids[i].GetIsActive())
			{
				//Check for collisions
				collide = m_asteroids[i].CheckCollision(bulletX, bulletY, radius);

				//If the player has spreadshot, gotta check all the bullets
				if(m_player.GetWeaponLevel() == 2)
				{
					//Vectors of shot info
					shotX = m_player.GetSpreadShotX(bulletCount);
					shotY = m_player.GetSpreadShotY(bulletCount);

					//Iterate the vectors
					for (int a = 0; a < shotX.size(); a++)
					{
						collide = m_asteroids[i].CheckCollision(shotX[a], shotY[a], radius);
						if (collide)
						{
   							break;
						}
					}
				}

				//If there's a collision
				if (collide) 
				{
					//Make the bullet inactive
					m_player.DeactivateBullet(bulletCount);

					//Call the asteroid hit function
					m_asteroids[i].AsteroidHit();
					
					//Increment score
					m_score += 5;

					//If the asteroid has been destroyed, spawn a wave of mediums
					if (!m_asteroids[i].GetIsActive())
					{
						LargeAsteroidDestroyed(m_asteroids[i].GetX(), m_asteroids[i].GetY());
					}
				}
			}
		}

		//For medium asteroids
		for (int j = 0; j < (m_asteroidCount * 3); j++)
		{
			if (m_mediumAsteroids[j].GetIsActive())
			{
				//Check for collisions
				collide = m_mediumAsteroids[j].CheckCollision(bulletX, bulletY, radius);

				//If the player has spreadshot, gotta check all the bullets
				if (m_player.GetWeaponLevel() == 2)
				{
					//Vectors of shot info
					shotX = m_player.GetSpreadShotX(bulletCount);
					shotY = m_player.GetSpreadShotY(bulletCount);

					//Iterate the vectors
					for (int b = 0; b < shotX.size(); b++)
					{
						collide = m_mediumAsteroids[j].CheckCollision(shotX[b], shotY[b], radius);
						if (collide)
						{
							break;
						}
					}
				}

				//If there's a collision
				if (collide) {
					//Make the bullet inactive
					m_player.DeactivateBullet(bulletCount);

					//Call the asteroid hit function
					m_mediumAsteroids[j].AsteroidHit();

					//Increment score
					m_score += 5;

					//If the asteroid has been destroyed, spawn a wave of smalls
					if (!m_mediumAsteroids[j].GetIsActive())
					{
						MediumAsteroidDestroyed(m_mediumAsteroids[j].GetX(), m_mediumAsteroids[j].GetY());
					}
				}
			}
		}

		//For small asteroids
		for (int k = 0; k < (m_asteroidCount * 12); k++)
		{
			if (m_smallAsteroids[k].GetIsActive())
			{
				//Check for collisions
				collide = m_smallAsteroids[k].CheckCollision(bulletX, bulletY, radius);

				//If the player has spreadshot, gotta check all the bullets
				if (m_player.GetWeaponLevel() == 2)
				{
					//Vectors of shot info
					shotX = m_player.GetSpreadShotX(bulletCount);
					shotY = m_player.GetSpreadShotY(bulletCount);

					//Iterate the vectors
					for (int c = 0; c < shotX.size(); c++)
					{
						collide = m_smallAsteroids[k].CheckCollision(shotX[c], shotY[c], radius);
						if (collide)
						{
							break;
						}
					}
				}

				//If there's a collision
				if (collide) {
					//Make the bullet inactive
					m_player.DeactivateBullet(bulletCount);

					//Call the asteroid hit function
					m_smallAsteroids[k].AsteroidHit();

					//Increment score
					m_score += 10;

					//If the asteroid has been destroyed
					if (!m_smallAsteroids[k].GetIsActive())
					{
						SmallAsteroidDestroyed(m_smallAsteroids[k].GetX(), m_smallAsteroids[k].GetY());
					}
				}
			}
		}
	}
}

void AsteroidController::BombUsed()
{
	//Play destruction noise
	PlaySound("Sounds/Explosion14.wav");

	//Loop through asteroids and damage them all (put this in reverse order so destroyed asteroids don't have their children destroyed)
	for (int k = 0; k < (m_asteroidCount * 12); k++)
	{
		if (m_smallAsteroids[k].GetIsActive())
		{
			m_smallAsteroids[k].AsteroidHit(BOMB_DAMAGE);

			//If the asteroid has been destroyed
			if (!m_smallAsteroids[k].GetIsActive())
			{
				SmallAsteroidDestroyed(m_smallAsteroids[k].GetX(), m_smallAsteroids[k].GetY());
			}
		}
	}
	for (int j = 0; j < (m_asteroidCount * 3); j++)
	{
		if (m_mediumAsteroids[j].GetIsActive())
		{
			m_mediumAsteroids[j].AsteroidHit(BOMB_DAMAGE);

			//If the asteroid has been destroyed, spawn a wave of smalls
			if (!m_mediumAsteroids[j].GetIsActive())
			{
				MediumAsteroidDestroyed(m_mediumAsteroids[j].GetX(), m_mediumAsteroids[j].GetY());
			}
		}
	}
	for (int i = 0; i < m_asteroidCount; i++)
	{
		if (m_asteroids[i].GetIsActive())
		{
			m_asteroids[i].AsteroidHit(BOMB_DAMAGE);

			//If the asteroid has been destroyed, spawn a wave of mediums
			if (!m_asteroids[i].GetIsActive())
			{
				LargeAsteroidDestroyed(m_asteroids[i].GetX(), m_asteroids[i].GetY());
			}
		}
	}
}