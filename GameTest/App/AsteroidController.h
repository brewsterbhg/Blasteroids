#pragma once
#include <vector>
#include "Player.h"
#include "Asteroid.h"
#include "MediumAsteroid.h"
#include "SmallAsteroid.h"
#include "..\App\app.h"

class AsteroidController
{
	//Constants
	static const int LEVEL_ONE = 1;
	static const int LEVEL_TWO = 2;
	static const int LEVEL_THREE = 3;
	static const int LEVEL_FOUR = 4;
	static const int LEVEL_FIVE = 5;
	static const int BOMB_DAMAGE = 5;

private:
	//The player object
	Player& m_player;

	//Game variables
	int& m_score;
	int& m_cash;
	int& m_currentLevel;

	//Change game state once level is complete
	int& m_gameState;

	//Renders # of asteroids depending on level
	int m_asteroidCount;

	//Array to hold large asteroids
	Asteroid m_asteroids[4];

	//Array to hold medium asteroids
	MediumAsteroid m_mediumAsteroids[12];

	//Array to hold small asteroids
	SmallAsteroid m_smallAsteroids[36];

public:
	AsteroidController(Player&, int&, int&, int&, int&);

	//-------------------------------------------------------------------------------------------
	// void New();
	//-------------------------------------------------------------------------------------------
	// Sets up a new wave of asteroids
	//-------------------------------------------------------------------------------------------
	void New();

	//-------------------------------------------------------------------------------------------
	// void Render();
	//-------------------------------------------------------------------------------------------
	// Render the asteroids
	//-------------------------------------------------------------------------------------------
	void Update();

	//-------------------------------------------------------------------------------------------
	// void LargeAsteroidDestroyed();
	//-------------------------------------------------------------------------------------------
	// On destruction of a large asteroid
	//-------------------------------------------------------------------------------------------
	void LargeAsteroidDestroyed(float, float);

	//-------------------------------------------------------------------------------------------
	// void MediumAsteroidDestroyed();
	//-------------------------------------------------------------------------------------------
	// On destruction of a medium asteroid
	//-------------------------------------------------------------------------------------------
	void MediumAsteroidDestroyed(float, float);

	//-------------------------------------------------------------------------------------------
	// void SmallAsteroidDestroyed();
	//-------------------------------------------------------------------------------------------
	// On destruction of a small asteroid
	//-------------------------------------------------------------------------------------------
	void SmallAsteroidDestroyed(float, float);

	//-------------------------------------------------------------------------------------------
	// void CheckLevelComplete();
	//-------------------------------------------------------------------------------------------
	// Checks if a level has been completed
	//-------------------------------------------------------------------------------------------
	void CheckLevelComplete();

	//-------------------------------------------------------------------------------------------
	// void CheckPlayerCollision();
	//-------------------------------------------------------------------------------------------
	// Checks collision between the player and the asteroids
	//-------------------------------------------------------------------------------------------
	void CheckPlayerCollision();

	//-------------------------------------------------------------------------------------------
	// void CheckBulletCollision();
	//-------------------------------------------------------------------------------------------
	// Checks collision between the bullets and the asteroids
	//-------------------------------------------------------------------------------------------
	void CheckBulletCollision();

	//-------------------------------------------------------------------------------------------
	// void BombUsed();
	//-------------------------------------------------------------------------------------------
	// Called when the player uses a bomb
	//-------------------------------------------------------------------------------------------
	void BombUsed();
};

