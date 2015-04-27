#pragma once
#ifndef TANK_C008101C_H
#define TANK_C008101C_H

#include "BaseTank.h"
#include <SDL.h>
#include <vector>
#include "Commons.h"

enum Deceleration
{ 
	sonic = 1,
	human = 2,
	snail = 3
};

class Waypoint;

class Tank_c008101c :
	protected BaseTank
{
//---------------------------------------------------------------
public:
	Tank_c008101c(SDL_Renderer* renderer, TankSetupDetails details);
	~Tank_c008101c();
	void CheckInput(SDL_Event e);

	void Render();
	void Update(float deltaTime, SDL_Event e);

//---------------------------------------------------------------
private:
	bool mNewTarget = false;
	bool mMoveToClicked = false;
	double mMaxSeeAhead = 80;
	Deceleration mDeceleration = human;
	vector<Waypoint*> mWaypoints;
	STEERING_BEHAVIOUR mBehaviour = STEERING_INTRPOSE;
	Texture2D* mAheadTex = nullptr;
	Texture2D* mAhead2Tex = nullptr;
	Texture2D* mWaypointTex = nullptr;
	Vector2D mSteeringForce;
	Vector2D mTargetPosition;
	Vector2D mTexCenter;
	Vector2D mAhead;
	Vector2D mAhead2;
	Waypoint* mTargetWaypoint = nullptr;

	GameObject* MostThreateningObstacle();
	void MoveInHeadingDirection(float deltaTime);
	Vector2D Arrive(Vector2D targetPosition);
	Vector2D Seek(Vector2D targetPosition);
	Vector2D Flee(Vector2D targetPosition);
	Vector2D ObstacleAvoidance();
	Vector2D PathFinding(Vector2D targetPosition);
	Waypoint* NearestWaypoint(vector<Waypoint*> waypoints, Vector2D source, Vector2D target);
	Waypoint* NextWaypoint(Waypoint* waypoint, Vector2D targetPosition);
};

//---------------------------------------------------------------

#endif //TANK_C008101C_H

