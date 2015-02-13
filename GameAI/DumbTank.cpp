#include "DumbTank.h"

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
}

//--------------------------------------------------------------------------------------------------

void DumbTank::MoveInHeadingDirection(float deltaTime)
{
}

//--------------------------------------------------------------------------------------------------