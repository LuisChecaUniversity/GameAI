#pragma once
#ifndef TANK_C008101C_H
#define TANK_C008101C_H

#include "BaseTank.h"
#include <SDL.h>
#include "Commons.h"


enum Deceleration
{ 
	sonic = 1,
	human = 2,
	snail = 3
};

class Tank_c008101c :
	protected BaseTank
{
	//---------------------------------------------------------------
public:
	Tank_c008101c(SDL_Renderer* renderer, TankSetupDetails details);
	~Tank_c008101c();
	void CheckInput(SDL_Event e);

	void ChangeState(BASE_TANK_STATE newState);
	void Render();
	void Update(float deltaTime, SDL_Event e);

	//---------------------------------------------------------------
private:
	double mMaxSeeAhead = 80;
	Deceleration mDeceleration = snail;
	STEERING_BEHAVIOUR mBehaviour = STEERING_ARRIVE;
	Texture2D* mAheadTex;
	Texture2D* mAhead2Tex;
	Vector2D mSteeringForce;
	Vector2D mTargetPosition;
	Vector2D mTexCenter;
	Vector2D mAhead;
	Vector2D mAhead2;

	void MoveInHeadingDirection(float deltaTime);
	Vector2D Seek(Vector2D targetPosition);
	Vector2D Flee(Vector2D targetPosition);
	Vector2D Arrive(Vector2D targetPosition, Deceleration deceleration);
	Vector2D ObstacleAvoidance();
	GameObject* MostThreateningObstacle();
};

//---------------------------------------------------------------

#endif //TANK_C008101C_H

