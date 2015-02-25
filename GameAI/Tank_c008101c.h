#pragma once
#ifndef TANK_C008101C_H
#define TANK_C008101C_H

#include "BaseTank.h"
#include <SDL.h>
#include "Commons.h"

class Tank_c008101c :
	protected BaseTank
{
	//---------------------------------------------------------------
public:
	Tank_c008101c(SDL_Renderer* renderer, TankSetupDetails details);
	~Tank_c008101c();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);

	//---------------------------------------------------------------
protected:
	void MoveInHeadingDirection(float deltaTime);
};

//---------------------------------------------------------------

#endif //TANK_C008101C_H

