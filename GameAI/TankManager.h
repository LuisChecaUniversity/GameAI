#ifndef _TANKMANAGER_H
#define _TANKMANAGER_H

//--------------------------------------------------------------------------------------------------
// Tank Manager is a class that keeps hold of all the Tanks in the scene.
// This is used to read in the tanks from a text file and for scoring purposes.

#include "Commons.h"
#include <SDL.h>
#include <vector>
using namespace::std;

class BaseTank;
class GameObject;

//--------------------------------------------------------------------------------------------------

class TankManager
{
	//---------------------------------------------------------------
public:
	TankManager(SDL_Renderer* renderer);
	~TankManager();

	vector<BaseTank*>	GetTanks()									{return mTanks;}
	void				UpdateTanks(float deltaTime, SDL_Event e);
	void				RenderTanks();

	void				CheckForCollisions(vector<GameObject*> listOfObjects);

	//---------------------------------------------------------------
private:
	void		LoadTanks(SDL_Renderer* renderer);
	BaseTank*	GetTankObject(SDL_Renderer* renderer, TankSetupDetails details);

	//---------------------------------------------------------------
private:
	vector<BaseTank*> mTanks;
};

//--------------------------------------------------------------------------------------------------

#endif //_TANKMANAGER_H