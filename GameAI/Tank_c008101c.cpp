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
		mTargetPos = Vector2D(e.button.x, e.button.y);
	}
	RotateHeadingToFacePosition(mTargetPos);

	switch (mBehaviour)
	{
	case STEERING_SEEK:
		mVelocity = Seek(mTargetPos);
		break;
	case STEERING_FLEE:
		mVelocity = Flee(mTargetPos);
		break;
	case STEERING_ARRIVE:
		mVelocity = Arrive(mTargetPos, mDeceleration);
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

	//Acceleration = Force/Mass
	Vector2D acceleration = force / GetMass();

	//Update velocity.
	mVelocity += acceleration * deltaTime;

	//Don't allow the tank does not go faster than max speed.
	mVelocity.Truncate(GetMaxSpeed()); //TODO: Add Penalty for going faster than MAX Speed.

	//Finally, update the position.
	Vector2D newPosition = GetPosition();
	newPosition.x += mVelocity.x * deltaTime;
	newPosition.y += mVelocity.y/**-1.0f*/ * deltaTime;	//Y flipped as adding to Y moves down screen.
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