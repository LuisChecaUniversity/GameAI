#include "Tank_c008101c.h"
#include "Collisions.h"
#include "Waypoint.h"
#include "ObstacleManager.h"
#include "TankManager.h"
#include "WaypointManager.h"

//--------------------------------------------------------------------------------------------------
Tank_c008101c::Tank_c008101c(SDL_Renderer* renderer, TankSetupDetails details) : BaseTank(renderer, details)
{
	mAheadTex = new Texture2D(renderer);
	mAheadTex->LoadFromFile(kMinePath);

	mAhead2Tex = new Texture2D(renderer);
	mAhead2Tex->LoadFromFile(kMinePath);

	mTexCenter = Vector2D(mAheadTex->GetWidth(), mAheadTex->GetHeight()) * -0.5;

	for (int i = 0; i < 17; i++)
	{
		mWaypoints.push_back(WaypointManager::Instance()->GetWaypointWithID(i));
	}

	mWaypointTex = mWaypoints[0]->mTexture;
}

//--------------------------------------------------------------------------------------------------
Tank_c008101c::~Tank_c008101c()
{
	delete mAheadTex;
	delete mAhead2Tex;
	mAheadTex = nullptr;
	mAhead2Tex = nullptr;
	mTargetWaypoint = nullptr;
	mWaypointTex;
	mWaypoints.clear();
}

//--------------------------------------------------------------------------------------------------
void Tank_c008101c::CheckInput(SDL_Event e)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_7:
		mBehaviour = STEERING_INTRPOSE;
		break;
	case SDLK_8:
		mBehaviour = STEERING_SEEK;
		break;
	case SDLK_9:
		mBehaviour = STEERING_FLEE;
		break;
	case SDLK_0:
		mBehaviour = STEERING_ARRIVE;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------
void Tank_c008101c::Update(float deltaTime, SDL_Event e)
{
	CheckInput(e);

	if (e.button.state == SDL_PRESSED)
	{
		mTargetPosition = Vector2D(e.button.x, e.button.y);
		mNewTarget = true;
	}

	switch (mBehaviour)
	{
	/*case STEERING_SEEK:
		mSteeringForce = Seek(mTargetPosition);
		break;
	case STEERING_FLEE:
		mSteeringForce = Flee(mTargetPosition);
		break;
	case STEERING_ARRIVE:
		mSteeringForce = Arrive(mTargetPosition);
		break;*/
	case STEERING_INTRPOSE:
		mSteeringForce = PathFinding(mTargetPosition);
		break;
	case STEERING_OBSTACLEAVOIDANCE:
		break;
	default:
		break;
	}

	BaseTank::Update(deltaTime, e);
	if (mVelocity.LengthSq() != 0)
	{
		RotateHeadingToFacePosition(GetCentralPosition() + mVelocity);
	}
	//TankManager::Instance()->GetVisibleTanks(this);
}

//--------------------------------------------------------------------------------------------------
void Tank_c008101c::Render()
{
	BaseTank::Render();
	mAheadTex->Render(mAhead + mTexCenter, 0);
	mAhead2Tex->Render(mAhead2 + mTexCenter, 0);
}

//--------------------------------------------------------------------------------------------------
void Tank_c008101c::MoveInHeadingDirection(float deltaTime)
{
	Vector2D avoidance = ObstacleAvoidance();
	Vector2D force = avoidance + mSteeringForce;

	//Acceleration = Force/Mass
	Vector2D acceleration = force / GetMass();

	//Update velocity.
	mVelocity += acceleration * deltaTime;

	//Don't allow the tank does not go faster than max speed.
	mVelocity.Truncate(GetMaxSpeed()); //TODO: Add Penalty for going faster than MAX Speed.
	
	//Finally, update the position.
	Vector2D newPosition = GetPosition();
	newPosition += mVelocity * deltaTime;
	SetPosition(newPosition);
}

//--------------------------------------------------------------------------------------------------
Vector2D Tank_c008101c::Seek(Vector2D targetPosition)
{
	Vector2D origin = GetCentralPosition();
	origin = Vector2D(round(origin.x), round(origin.y));
	Vector2D resultingVelocity = Vec2DNormalize(targetPosition - origin) * GetMaxSpeed();

	return (resultingVelocity - mVelocity);
}

//--------------------------------------------------------------------------------------------------
Vector2D Tank_c008101c::Flee(Vector2D targetPosition)
{
	//only flee if the target is within 'panic distance'. Work in distance squared space.
	const double PanicDistanceSq = 100.0f * 100.0;
	if (Vec2DDistanceSq(GetPosition(), targetPosition) > PanicDistanceSq)
	{
		return Vector2D();
	}

	Vector2D resultingVelocity = Vec2DNormalize(GetPosition() - targetPosition) * GetMaxSpeed();

	return (resultingVelocity - mVelocity);
}

//--------------------------------------------------------------------------------------------------
Vector2D Tank_c008101c::Arrive(Vector2D targetPosition)
{
	Vector2D origin = GetCentralPosition();
	origin = Vector2D(round(origin.x), round(origin.y));
	Vector2D vectorToTarget = targetPosition - origin;

	//calculate the distance to the target
	double distance = vectorToTarget.Length();

	if (distance > 0)
	{
		const double decelerationFineTune = 1.0;

		double speed = distance / ((double)mDeceleration * decelerationFineTune);

		speed = min(speed, GetMaxSpeed());

		Vector2D resultingVelocity = vectorToTarget * speed / distance;
		resultingVelocity -= mVelocity;
		return resultingVelocity;
	}

	return Vector2D();
}

//--------------------------------------------------------------------------------------------------
Vector2D Tank_c008101c::ObstacleAvoidance()
{
	double dynamicLength = mVelocity.Length() / GetMaxSpeed();
	mAhead = GetCentralPosition() + Vec2DNormalize(mVelocity) * mMaxSeeAhead * dynamicLength;
	mAhead2 = GetCentralPosition() + Vec2DNormalize(mVelocity) * mMaxSeeAhead * 0.5 * dynamicLength;

	GameObject* mostThreatening = MostThreateningObstacle();

	if (mostThreatening != nullptr)
	{
		return Vec2DNormalize(mAhead - mostThreatening->GetCentralPosition()) * GetMaxSpeed();
	}

	return Vector2D();
}

//--------------------------------------------------------------------------------------------------
GameObject* Tank_c008101c::MostThreateningObstacle()
{
	GameObject* most = nullptr;

	for(GameObject* obstacle : ObstacleManager::Instance()->GetObstacles())
	{
		int offset = mTexture->GetWidth() / 3;
		Vector2D vOffset = Vector2D(offset, offset);
		Rect2D rect = obstacle->GetAdjustedBoundingBox();
		rect.x -= offset;
		rect.y -= offset;
		rect.height += offset;
		rect.width += offset;
		bool collision = Collisions::Instance()->PointInBox(mAhead + vOffset, rect)
			|| Collisions::Instance()->PointInBox(mAhead2 - vOffset, rect);

		Vector2D pos = GetPosition();
		if (collision && (most == nullptr || Vec2DDistance(pos, obstacle->GetCentralPosition()) < Vec2DDistance(pos, most->GetCentralPosition())))
		{
			most = obstacle;
		}
	}
	return most;
}

//--------------------------------------------------------------------------------------------------
Vector2D Tank_c008101c::PathFinding(Vector2D targetPosition)
{
	double dist = 0;
	double dist2 = 0;
	double dist3 = 0;
	double dist4 = 0;
	Waypoint* w = nullptr;

	// Initial state: no target waypoint set
	if (mTargetWaypoint == nullptr && !mNewTarget)
	{
		return Vector2D();
	}
	// New target has been clicked
	if (mNewTarget)
	{
		// Reset waypoint texture
		if (mTargetWaypoint != nullptr)
		{
			mTargetWaypoint->mTexture = mWaypointTex;
		}
		w = NearestWaypoint(mWaypoints, GetCentralPosition(), targetPosition);
		// New waypoint is target waypoint
		dist2 = Vec2DDistanceSq(GetCentralPosition(), w->GetPosition());
		dist3 = Vec2DDistanceSq(GetCentralPosition(), targetPosition);
		if (dist3 < dist2)
		{
			mMoveToClicked = true;
			return Arrive(targetPosition);
		}
		else
		{
			mTargetWaypoint = w;
			mNewTarget = false;
			mMoveToClicked = false;
		}
	}
	else if (mMoveToClicked)
	{
		return Arrive(targetPosition);
	}
	else if (mTargetWaypoint != nullptr)
	{
		dist = Vec2DDistanceSq(GetCentralPosition(), mTargetWaypoint->GetPosition());
		// Next waypoint if close enough
		if (dist <= 3600)
		{
			w = NextWaypoint(mTargetWaypoint, targetPosition);
		}
		else
		{
			return Seek(mTargetWaypoint->GetPosition());
		}

		// If had a target and have new target, check new target is better
		//dist = Vec2DDistanceSq(mTargetWaypoint->GetPosition(), targetPosition);
		dist2 = Vec2DDistanceSq(w->GetPosition(), targetPosition);
		dist3 = Vec2DDistanceSq(GetCentralPosition(), w->GetPosition());
		dist4 = Vec2DDistanceSq(GetCentralPosition(), targetPosition);
		//Vec2DDistanceSq(GetCentralPosition(), mTargetWaypoint->GetPosition())
		
		if (dist4 < dist3 || dist4 < dist2)
		{
			mMoveToClicked = true;
			// Reset waypoint texture
			mTargetWaypoint->mTexture = mWaypointTex;
			return Arrive(targetPosition);
		}
		else //if (dist2 > dist)
		{
			// Reset waypoint texture
			mTargetWaypoint->mTexture = mWaypointTex;
			mTargetWaypoint = w;
		}
	}

	// New waypoint is null: exit, do nothing
	if (w == nullptr)
	{
		return Vector2D();
	}

	// Mark waypoint by changing texture
	mTargetWaypoint->mTexture = mAheadTex;
	// Move to target
	return Seek(mTargetWaypoint->GetPosition());
}

Waypoint* Tank_c008101c::NearestWaypoint(vector<Waypoint*> waypoints, Vector2D source, Vector2D target)
{
	Waypoint* nearest = nullptr;
	double minSource = MaxDouble;
	double minTarget = MaxDouble;
	double sourceDist = 0;
	double targetDist = 0;

	for (Waypoint* w : waypoints)
	{
		sourceDist = Vec2DDistanceSq(source, w->GetPosition());
		targetDist = Vec2DDistanceSq(target, w->GetPosition());

		if (sourceDist < minSource)
		{
			minSource = sourceDist;
			minTarget = targetDist;
			nearest = w;
		}
	}

	for (Waypoint* w : waypoints)
	{
		sourceDist = Vec2DDistanceSq(source, w->GetPosition());
		targetDist = Vec2DDistanceSq(target, w->GetPosition());

		if (sourceDist + targetDist < minSource + minTarget)
		{
			minSource = sourceDist;
			minTarget = targetDist;
			nearest = w;
		}
	}

	return nearest;
}

Waypoint* Tank_c008101c::NextWaypoint(Waypoint* waypoint, Vector2D targetPosition)
{
	vector<Waypoint*> nextWaypoints;
	for (int id : waypoint->GetConnectedWaypointIDs())
	{
		nextWaypoints.push_back(mWaypoints[id]);
	}
	return NearestWaypoint(nextWaypoints, waypoint->GetPosition(), targetPosition);
}