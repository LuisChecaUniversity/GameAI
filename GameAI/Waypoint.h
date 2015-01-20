#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "Commons.h"
#include <vector>
using namespace::std;

//---------------------------------------------------------------
// A waypoint is a point on the map which information regarding where can be reached from this point.
// Used in pathfinding.

class Waypoint
{
	//---------------------------------------------------------------
public:
	Waypoint(int id, Vector2D startPosition, vector<int> connectingIDs);
	~Waypoint();

	int			GetID()										{return mID;}
	
	vector<int> GetConnectedWaypointIDs()					{return mConnectedWaypointIDs;}
	bool		IsConnectedTo(int waypointIDToCheck);

	Vector2D	GetPosition()								{return mPosition;}

	//---------------------------------------------------------------
private:
	int			mID;
	Vector2D	mPosition;

	vector<int>	 mConnectedWaypointIDs;			
};

//---------------------------------------------------------------

#endif //WAYPOINT_H