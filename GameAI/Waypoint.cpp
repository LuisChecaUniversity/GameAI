#include "Waypoint.h"
#include <vector>

using namespace::std;
//---------------------------------------------------------------

Waypoint::Waypoint(int id, Vector2D startPosition, vector<int> connectingIDs)
{
	mID			= id;
	mPosition	= startPosition;

	mConnectedWaypointIDs = connectingIDs;
}

//---------------------------------------------------------------

Waypoint::~Waypoint()
{
	mConnectedWaypointIDs.clear();
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

