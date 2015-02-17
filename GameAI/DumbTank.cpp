#include "DumbTank.h"
#include "TankManager.h"

//--------------------------------------------------------------------------------------------------

DumbTank::DumbTank(SDL_Renderer* renderer, TankSetupDetails details) : BaseTank(renderer, details)
{
}

//--------------------------------------------------------------------------------------------------

DumbTank::~DumbTank()
{
}

//--------------------------------------------------------------------------------------------------

void DumbTank::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------

void DumbTank::Update(float deltaTime, SDL_Event e)
{
	BaseTank::Update(deltaTime, e);

	TankManager::Instance()->GetVisibleTanks(this);
}

//--------------------------------------------------------------------------------------------------

void DumbTank::MoveInHeadingDirection(float deltaTime)
{
}

//--------------------------------------------------------------------------------------------------