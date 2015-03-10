#include "Tank_c008101c.h"
#include "Collisions.h"
#include "TankManager.h"
#include "ObstacleManager.h"

//--------------------------------------------------------------------------------------------------
Tank_c008101c::Tank_c008101c(SDL_Renderer* renderer, TankSetupDetails details) : BaseTank(renderer, details)
{
	mAheadTex = new Texture2D(renderer);
	mAheadTex->LoadFromFile("Images/Mine.png");

	mAhead2Tex = new Texture2D(renderer);
	mAhead2Tex->LoadFromFile("Images/Mine.png");

	Vector2D mTexCenter = Vector2D(-mAheadTex->GetWidth(), mAheadTex->GetHeight()) * 0.5;
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

	switch (mBehaviour)
	{
	case STEERING_SEEK:
		mSteeringForce = Seek(mTargetPosition);
		break;
	case STEERING_FLEE:
		mSteeringForce = Flee(mTargetPosition);
		break;
	case STEERING_ARRIVE:
		mSteeringForce = Arrive(mTargetPosition, mDeceleration);
		break;
	case STEERING_INTRPOSE:
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

//--------------------------------------------------------------------------------------------------
Vector2D Tank_c008101c::Arrive(Vector2D targetPosition, Deceleration deceleration)
{
	Vector2D origin = GetCentralPosition();
	origin = Vector2D(round(origin.x), round(origin.y));
	Vector2D vectorToTarget = targetPosition - origin;

	//calculate the distance to the target
	double distance = vectorToTarget.Length();

	if (distance > 0)
	{
		const double decelerationFineTune = 0.8;

		double speed = distance / ((double)deceleration * decelerationFineTune);

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
		return (Vec2DNormalize(mAhead - mostThreatening->GetCentralPosition()) * GetMaxSpeed());
	}

	return Vector2D();
}

//--------------------------------------------------------------------------------------------------
GameObject* Tank_c008101c::MostThreateningObstacle()
{
	GameObject* most = nullptr;

	for(GameObject* obstacle : ObstacleManager::Instance()->GetObstacles())
	{
		Rect2D rect = obstacle->GetAdjustedBoundingBox();
		rect.x -= 20;
		rect.y -= 20;
		rect.height += 20;
		rect.width += 20;
		bool collision = Collisions::Instance()->PointInBox(mAhead, rect)
			|| Collisions::Instance()->PointInBox(mAhead2, rect);

		Vector2D pos = GetPosition();
		if (collision && (most == nullptr || Vec2DDistance(pos, obstacle->GetCentralPosition()) < Vec2DDistance(pos, most->GetCentralPosition())))
		{
			most = obstacle;
		}
	}
	return most;
}