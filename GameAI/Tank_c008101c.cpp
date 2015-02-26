#include "Tank_c008101c.h"
#include "Collisions.h"
#include "TankManager.h"
#include "ObstacleManager.h"

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
void Tank_c008101c::CheckInput(SDL_Event e)
{
	switch (e.key.keysym.sym)
	{
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

	if (e.button.button == SDL_BUTTON_LEFT)
	{
		mTargetPosition = Vector2D(e.button.x, e.button.y);
	}
	RotateHeadingToFacePosition(mTargetPosition);

	switch (mBehaviour)
	{
	case STEERING_SEEK:
		if(!mTargetPosition.isZero()) mVelocity = Seek(mTargetPosition);
		//mBehaviour = STEERING_OBSTACLEAVOIDANCE;
		break;
	case STEERING_FLEE:
		mVelocity = Flee(mTargetPosition);
		//mBehaviour = STEERING_OBSTACLEAVOIDANCE;
		break;
	case STEERING_ARRIVE:
		mVelocity = Arrive(mTargetPosition, mDeceleration);
		//mBehaviour = STEERING_OBSTACLEAVOIDANCE;
		break;
	case STEERING_INTRPOSE:
		break;
	case STEERING_OBSTACLEAVOIDANCE:
		break;
	default:
		break;
	}

	BaseTank::Update(deltaTime, e);

	//TankManager::Instance()->GetVisibleTanks(this);
}

//--------------------------------------------------------------------------------------------------
void Tank_c008101c::MoveInHeadingDirection(float deltaTime)
{
	//Get the force that propels in current heading.
	Vector2D force = (mHeading*mCurrentSpeed) - mVelocity;
	force.Truncate(GetMaxForce());

	//Acceleration = Force/Mass
	Vector2D acceleration = force / GetMass();

	//Update velocity.
	mVelocity += acceleration * deltaTime;

	// Try to avoid obstacles
	mVelocity += ObstacleAvoidance();

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
	Vector2D resultingVelocity = Vec2DNormalize(targetPosition - GetPosition()) * GetMaxSpeed();

	return (resultingVelocity - mVelocity);
}

//--------------------------------------------------------------------------------------------------
Vector2D Tank_c008101c::Flee(Vector2D targetPosition)
{
	//only flee if the target is within 'panic distance'. Work in distance
	//squared space.
	/* const double PanicDistanceSq = 100.0f * 100.0;
	if (Vec2DDistanceSq(m_pVehicle->Pos(), target) > PanicDistanceSq)
	{
	return Vector2D(0,0);
	}
	*/

	Vector2D resultingVelocity = Vec2DNormalize(GetPosition() - targetPosition) * GetMaxSpeed();

	return (resultingVelocity - mVelocity);
}

//--------------------------- Arrive -------------------------------------
Vector2D Tank_c008101c::Arrive(Vector2D targetPosition, Deceleration deceleration)
{
	Vector2D vectorToTarget = targetPosition - GetPosition();

	//calculate the distance to the target
	double distance = vectorToTarget.Length();

	if (distance > 0)
	{
		const double decelerationFineTune = 0.3;

		//calculate the speed required to reach the target given the desired
		//deceleration
		double speed = distance / ((double)deceleration * decelerationFineTune);

		//make sure the velocity does not exceed the max
		speed = min(speed, GetMaxSpeed());

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		Vector2D resultingVelocity = vectorToTarget * speed / distance;

		return (resultingVelocity - mVelocity);
	}

	return Vector2D(0, 0);
}
//--------------------------------------------------------------------------------------------------
Vector2D Tank_c008101c::ObstacleAvoidance()
{
	double dynamicLength = mVelocity.Length() / GetMaxSpeed();
	mAhead = GetPosition() + Vec2DNormalize(mVelocity) * mMaxSeeAhead * dynamicLength;
	mAhead2 = mAhead * 0.5;

	GameObject* mostThreatening = MostThreateningObstacle();

	if (mostThreatening != nullptr)
	{
		return Vec2DNormalize(mAhead - mostThreatening->GetCentralPosition()) * mMaxAvoidForce;
	}

	return Vector2D();
}

GameObject* Tank_c008101c::MostThreateningObstacle()
{
	GameObject* most = nullptr;

	for(GameObject* obstacle : ObstacleManager::Instance()->GetObstacles())
	{
		Rect2D rect = obstacle->GetAdjustedBoundingBox();
		bool collision = Collisions::Instance()->PointInBox(mAhead, rect)
			|| Collisions::Instance()->PointInBox(mAhead2, rect);

		Vector2D pos = GetPosition();
		if (collision && (most == nullptr || Vec2DDistance(pos, obstacle->GetPosition()) < Vec2DDistance(pos, most->GetPosition())))
		{
			most = obstacle;
		}
	}
	return most;
}