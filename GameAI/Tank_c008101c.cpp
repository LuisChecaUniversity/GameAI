#include "Tank_c008101c.h"
#include "TankManager.h"

//--------------------------------------------------------------------------------------------------

Tank_c008101c::Tank_c008101c(SDL_Renderer* renderer, TankSetupDetails details) : BaseTank(renderer, details)
{
}

//--------------------------------------------------------------------------------------------------

Tank_c008101c::~Tank_c008101c()
{
}

//--------------------------------------------------------------------------------------------------

void Tank_c008101c::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------

void Tank_c008101c::Update(float deltaTime, SDL_Event e)
{
	BaseTank::Update(deltaTime, e);

	TankManager::Instance()->GetVisibleTanks(this);
}

//--------------------------------------------------------------------------------------------------

void Tank_c008101c::MoveInHeadingDirection(float deltaTime)
{
}

//--------------------------------------------------------------------------------------------------