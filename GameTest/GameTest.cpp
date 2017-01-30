//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>
#include <string>
#include <iostream>
#include <ctime>
//------------------------------------------------------------------------
#include "app\app.h"
#include "app\AppSettings.h"
#include "App\Player.h"
#include "App\AsteroidController.h"

using namespace App;

//Game states
const int GAME_MENU = 1;
const int GAME_PLAY = 2;
const int GAME_POST_LEVEL = 3;
const int GAME_STORE = 4;
const int GAME_END = 5;
const int GAME_OVER = 6;

//Store consts
const int BUY_BOMBS = 1;
const int BUY_LIVES = 2;
const int UPGRADE_WEAPON = 3;
const int UPGRADE_SHIP = 4;

//Prices
const int BOMB_PRICE = 500;
const int ONE_UP_PRICE = 500;
const int WEAPON_UPGRADE_PRICE = 1200;
const int SHIP_UPGRADE_PRICE = 2000;

//Final level
const int GAME_COMPLETE = 6;
static int m_gameState = GAME_MENU;

//Game variables
int m_lives = 4;
int m_score = 0;
int m_cash = 2000;
int m_currentLevel = 0;
int m_bombCount = 1;
int m_inputBuffer = 15;

Player m_player(m_lives);
AsteroidController m_asteroidController(m_player, m_score, m_cash, m_currentLevel, m_gameState);

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//Set seed
	srand((unsigned int)time(NULL));
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	//Get game state and handle key events as necessary
	switch (m_gameState)
	{
		case GAME_MENU:
			//If return key is pressed, start game
			if (IsKeyPressed(13))
			{
				//Set new state and level
				m_gameState = GAME_PLAY;
				m_currentLevel = 1;

				//Starting sound
				PlaySound("Sounds/Emerge11.wav");

				//Set up the game objects
				m_player.New();
				m_asteroidController.New();
			}
			break;

		case GAME_PLAY:
			//If pressed space bar (fire) and player is alive
			if (IsKeyPressed(32) && m_player.IsAlive())
			{
				m_player.FirePrimaryWeapons();
				if (!IsSoundPlaying("Sounds/Laser_Shoot8.wav")) {
					PlaySound("Sounds/Laser_Shoot8.wav");
				}
			}
			//If "B" button is pressed and user has bombs, use one (if buffer is not active)
			if (IsKeyPressed(66) && m_inputBuffer == 0)
			{
				if (m_bombCount > 0)
				{
					m_asteroidController.BombUsed();
					m_bombCount--;

					//Buffer to stop multiple presses
					m_inputBuffer = 15;
				}
			}
			//If pressed the left arrow
			if (IsKeyPressed(37))
			{
				m_player.RotateShip(1);
			}
			//If pressed the right arrow
			else if (IsKeyPressed(39))
			{
				m_player.RotateShip(2);
			}
			//If pressed the forward arrow
			else if (IsKeyPressed(38))
			{
				m_player.Boost();
			}
			//If pressed the down arrow
			else if (IsKeyPressed(40))
			{
				m_player.Decelerate();
			}
			else
			{
				m_player.StopBoost();
			}
			break;

		case GAME_POST_LEVEL:
			//If return key is pressed, go to next level
			if (IsKeyPressed(13))
			{
				//Starting sound
				PlaySound("Sounds/Emerge11.wav");

				//Set new state
				m_gameState = GAME_PLAY;

				//Set up the game objects
				m_player.New();
				m_asteroidController.New();
			}
			//If "1" key is pressed, go to the store
			if (IsKeyPressed(49))
			{
				m_gameState = GAME_STORE;
				m_inputBuffer = 15;
			}
			break;

		case GAME_STORE:
			//Make sure buffer is on to prevent multiple presses
			if (m_inputBuffer == 0)
			{
				//If "1" key is pressed, user buys a bomb
				if (IsKeyPressed(49))
				{
					if (m_cash >= BOMB_PRICE)
					{
						//Play sound and change values
						PlaySound("Sounds/Emerge9.wav");
						m_cash = m_cash - BOMB_PRICE;
						m_bombCount++;
						m_inputBuffer = 15;
					}
				}

				//If "2" key is pressed, user buys a life
				else if (IsKeyPressed(50))
				{
					if (m_cash >= ONE_UP_PRICE)
					{
						//Play sound and change values
						PlaySound("Sounds/Emerge9.wav");
						m_cash = m_cash - ONE_UP_PRICE;
						m_lives++;
						m_inputBuffer = 15;
					}
				}

				//If "5" key is pressed, user upgrades ship weapons
				else if (IsKeyPressed(51))
				{
					//Make sure they have the money and haven't bought the upgrade already
					if (m_cash >= WEAPON_UPGRADE_PRICE && m_player.GetWeaponLevel() == 1)
					{
						//Play sound and change values
						PlaySound("Sounds/Emerge9.wav");
						m_cash = m_cash - WEAPON_UPGRADE_PRICE;
						m_player.UpgradeWeapon();
						m_inputBuffer = 15;
					}
				}

				//If "5" key is pressed, user upgrades ship armour
				else if (IsKeyPressed(52))
				{
					//Make sure they have the money and haven't bought the upgrade already
					if (m_cash >= SHIP_UPGRADE_PRICE && m_player.GetArmourLevel() == 1)
					{
						//Play sound and change values
						PlaySound("Sounds/Emerge9.wav");
						m_cash = m_cash - SHIP_UPGRADE_PRICE;
						m_player.UpgradeArmour();
						m_inputBuffer = 15;
					}
				}

				//If "5" key is pressed, return to the post level screen
				else if (IsKeyPressed(53))
				{
					m_gameState = GAME_POST_LEVEL;
				}
			}
			break;

		case GAME_END:
		case GAME_OVER:
			//If "1" key is pressed, start a new game
			if (IsKeyPressed(49))
			{
				//Set everything to default
				m_gameState = GAME_MENU;
				m_currentLevel = 1;
				m_lives = 4;
				m_score = 0;
				m_cash = 0;
				m_bombCount = 0;

				//Set up the game objects
				m_player.Reset();
				m_asteroidController.New();
			}
			break;
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine or Print) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	//Convert game vars from int to char* (level, lives, score, cash, bomb count)
	std::string lv = std::to_string(m_currentLevel);
	char const *pLevel = lv.c_str();
	std::string l = std::to_string(m_lives);
	char const *pLives = l.c_str();
	std::string s = std::to_string(m_score);
	char const *pScore = s.c_str();
	std::string c = std::to_string(m_cash);
	char const *pCash = c.c_str();
	std::string b = std::to_string(m_bombCount);
	char const *pBomb = b.c_str();

	//Render depending on game state
	switch (m_gameState)
	{
		case GAME_MENU:
			Print(GetScreenWidth() / 2 - 50, GetScreenHeight() / 1.5, "BLASTEROIDS", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_18));
			Print(GetScreenWidth() / 2 - 48, GetScreenHeight() / 1.6, "Created by Keith Brewster", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(GetScreenWidth() / 2 - 53, GetScreenHeight() / 2, "PRESS [ENTER] TO START", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			break;

		case GAME_PLAY:
			//Render game objects
			m_player.UpdateShip();
			m_asteroidController.Update();

			//HUD text
			Print(5, GetScreenHeight() - 20, "LEVEL: ", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(65, GetScreenHeight() - 19, pLevel, (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(5, GetScreenHeight() - 40, "LIVES: ", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(65, GetScreenHeight() - 39, pLives, (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(5, GetScreenHeight() - 60, "SCORE: ", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(65, GetScreenHeight() - 60, pScore, (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(5, GetScreenHeight() - 80, "CASH: ", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(65, GetScreenHeight() - 80, pCash, (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(5, GetScreenHeight() - 100, "BOMBS: ", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(65, GetScreenHeight() - 100, pBomb, (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));

			//Decrement the input buffer
			if (m_inputBuffer > 0)
				m_inputBuffer--;

			//If lives become zero, call game over
			if (m_lives == 0)
				m_gameState = GAME_OVER;

			break;

		case GAME_POST_LEVEL:
			//If the final level, change the state
			if (m_currentLevel == GAME_COMPLETE)
			{
				m_gameState = GAME_END;
			}
			else {
				//Render some new HUD options
				Print(GetScreenWidth() / 2 - 50, GetScreenHeight() / 1.5, "LEVEL COMPLETE", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_18));
				Print(GetScreenWidth() / 2 - 40, GetScreenHeight() / 1.6, "PRESS [1] TO GO TO STORE", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
				Print(GetScreenWidth() / 2 - 68, GetScreenHeight() / 2, "PRESS [ENTER] TO START NEXT LEVEL", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			}
			break;

		case GAME_STORE:
			float wRGB, aRGB;
			wRGB = (m_player.GetWeaponLevel() == 1) ? 1.0 : 0.3;
			aRGB = (m_player.GetArmourLevel() == 1) ? 1.0 : 0.3;

			//Render some new HUD options
			Print(GetScreenWidth() / 2 - 120, GetScreenHeight() / 1.3, "CURRENT SPACE CASH: ", (0.0), (1.0), (0.0), (GLUT_BITMAP_HELVETICA_18));
			Print(GetScreenWidth() / 2 + 100, GetScreenHeight() / 1.3, pCash, (0.0), (1.0), (0.0), (GLUT_BITMAP_HELVETICA_18));
			Print(GetScreenWidth() / 2 - 330, GetScreenHeight() / 1.5, "===========================STORE===========================", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_18));
			Print(GetScreenWidth() / 2 - 45, GetScreenHeight() / 1.6, "[1] BOMB - $500", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(GetScreenWidth() / 2 - 45, GetScreenHeight() / 1.6 - 20, "[2] 1-UP - $500", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(GetScreenWidth() / 2 - 45, GetScreenHeight() / 1.6 - 40, "[3] UPGRADE WEAPON - $1200", (wRGB), (wRGB), (wRGB), (GLUT_BITMAP_HELVETICA_10));
			Print(GetScreenWidth() / 2 - 45, GetScreenHeight() / 1.6 - 60, "[4] UPGRADE SHIP - $2000", (aRGB), (aRGB), (aRGB), (GLUT_BITMAP_HELVETICA_10));
			Print(GetScreenWidth() / 2 - 45, GetScreenHeight() / 1.6 - 80, "[5] BACK", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(GetScreenWidth() / 2 - 330, GetScreenHeight() / 1.5 - 150, "===========================================================", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_18));

			//Decrement the input buffer
			if (m_inputBuffer > 0)
				m_inputBuffer--;\

			break;

		case GAME_END:
		case GAME_OVER:
			//Render some new HUD options
			Print(GetScreenWidth() / 2 - 120, GetScreenHeight() / 1.5, (m_gameState == GAME_END) ? "CONGRATULATIONS, YOU WIN" : "OH DEAR. THAT'S GAME OVER", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_18));
			Print(GetScreenWidth() / 2 - 60, GetScreenHeight() / 1.6, "FINAL SCORE: ", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_18));
			Print(GetScreenWidth() / 2 + 80, GetScreenHeight() / 1.6, pScore, (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_18));
			Print(GetScreenWidth() / 2 - 55, GetScreenHeight() / 2, "PRESS [1] TO PLAY AGAIN", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			Print(GetScreenWidth() / 2 - 48, GetScreenHeight() / 2 - 20, "PRESS [ESC] TO QUIT", (1.0), (1.0), (1.0), (GLUT_BITMAP_HELVETICA_10));
			break;
	}
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}