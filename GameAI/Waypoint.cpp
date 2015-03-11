#include "Waypoint.h"
#include "Texture2D.h"
#include <SDL.h>
#include <vector>

using namespace::std;
//---------------------------------------------------------------

Waypoint::Waypoint(SDL_Renderer* renderer, int id, Vector2D startPosition, vector<int> connectingIDs)
{
	mID			= id;
	mPosition	= startPosition;

	mTexture = new Texture2D(renderer);
	mTexture->LoadFromFile(kMkWaypointPath);

	mConnectedWaypointIDs = connectingIDs;
}

//---------------------------------------------------------------

Waypoint::~Waypoint()
{
	mConnectedWaypointIDs.clear();
}

//---------------------------------------------------------------

void Waypoint::Render()
{
	mTexture->Render(mPosition);
}

//---------------------------------------------------------------

bool Waypoint::IsConnectedTo(int waypointIDToCheck)
{
	//Check through the connect IDs to see if there is a match.
	for(unsigned int i = 0; i < mConnectedWaypointIDs.size(); i++)
	{
		if(mConnectedWaypointIDs[i] == waypointIDToCheck)
			return true;
	}

	//If we reach here there was no match.
	return false;
}

//---------------------------------------------------------------

