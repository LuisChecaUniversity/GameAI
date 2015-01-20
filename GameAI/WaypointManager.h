#ifndef _WAYPOINTMANAGER_H
#define _WAYPOINTMANAGER_H

//--------------------------------------------------------------------------------------------------
// Waypoint Manager is a singleton that keeps hold of all the waypoints in the scene.
// It collects its information from the XML file and can give useful information on request.

#include <vector>
using namespace::std;

class Waypoint;

//--------------------------------------------------------------------------------------------------

class WaypointManager
{
	//---------------------------------------------------------------
public:
	~WaypointManager();

	static WaypointManager* Instance();

	Waypoint* GetWaypointWithID(int id);

	//---------------------------------------------------------------
private:
	WaypointManager();

	void LoadWaypoints();

	//---------------------------------------------------------------
private:
	static WaypointManager* mInstance;

	vector<Waypoint*> mWaypoints;
};

//--------------------------------------------------------------------------------------------------

#endif //_WAYPOINTMANAGER_H