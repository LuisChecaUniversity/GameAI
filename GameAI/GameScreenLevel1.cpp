#include "GameScreenLevel1.h"
#include <iostream>
#include "LevelMap.h"
#include "Texture2D.h"
#include "Collisions.h"
#include "WaypointManager.h"
#include "ObstacleManager.h"
#include "TankManager.h"
#include "ProjectileManager.h"

using namespace::std;

//--------------------------------------------------------------------------------------------------

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	srand(NULL);
		
	//Set the level map.
	mLevelMap = new LevelMap(renderer);

	//Set up the waypoints.
	WaypointManager::Instance();

	//Set up the obstacles.
	mObstacleManager = new ObstacleManager(renderer);

	//Set up the tanks.
	mTankManager = new TankManager(renderer);
}

//--------------------------------------------------------------------------------------------------

GameScreenLevel1::~GameScreenLevel1()
{
	//Level map.
	delete mLevelMap;
	mLevelMap = NULL;

	//Obstacle manager.
	delete mObstacleManager;
	mObstacleManager = NULL;

	//Tank manager.
	delete mTankManager;
	mTankManager = NULL;
}

//--------------------------------------------------------------------------------------------------

void GameScreenLevel1::Render()
{
	//Draw the background.
	mLevelMap->Render();

	//Draw the obstacles over the level map.
	mObstacleManager->RenderObstacles();

	//Draw the tanks after everything else.
	mTankManager->RenderTanks();

	//Draw the bullets.
	ProjectileManager::Instance()->RenderProjectiles();
}

//--------------------------------------------------------------------------------------------------

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	switch(e.type)
	{
		//Deal with keyboard input.
		case SDL_KEYUP:
			switch(e.key.keysym.sym)
			{
				case SDLK_DOWN:
				break;

				default:
				break;
			}
		break;
	}

	//Update the obstacles.
	mObstacleManager->UpdateObstacles(deltaTime, e);

	//Update the tanks.
	mTankManager->UpdateTanks(deltaTime, e);
	//Do Collision checks.
	mTankManager->CheckForCollisions(mObstacleManager->GetObstacles());

	//Update the bullets.
	ProjectileManager::Instance()->UpdateProjectiles(deltaTime);
	//Do collision checks.
	ProjectileManager::Instance()->CheckForCollisions(mObstacleManager->GetObstacles());
	ProjectileManager::Instance()->CheckForCollisions(mTankManager->GetTanks());

}

//--------------------------------------------------------------------------------------------------