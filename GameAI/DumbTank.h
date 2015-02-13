#ifndef DUMBTANK_H
#define DUMBTANK_H

#include "BaseTank.h"
#include <SDL.h>
#include "Commons.h"

//---------------------------------------------------------------

class DumbTank : protected BaseTank
{
	//---------------------------------------------------------------
public:
	DumbTank(SDL_Renderer* renderer, TankSetupDetails details);
	~DumbTank();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);

	//---------------------------------------------------------------
protected:
	void	MoveInHeadingDirection(float deltaTime);
};

//---------------------------------------------------------------

#endif //DUMBTANK_H