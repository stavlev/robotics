#include "math.h"
#include "ConfigurationManager.h"
#include "PathPlanner.h"
#include "Map.h"
#include "WaypointsManager.h"
#include "DisplayManager.h"
#include "Robot.h"
#include "MovementManager.h"

int main()
{
	try
	{
		Hamster * hamster = new HamsterAPI::Hamster(1);

		sleep(3);
		OccupancyGrid occupancyGrid = hamster->getSLAMMap();

		sleep(1);
		double mapHeight = occupancyGrid.getHeight();
		double mapWidth = occupancyGrid.getWidth();
		double mapResolution = occupancyGrid.getResolution();

		ConfigurationManager configurationManager(mapHeight, mapWidth);
		Location startLocation = configurationManager.GetStartLocation();
		Location goalLocation = configurationManager.GetGoalLocation();
		int robotRadiusInCm = configurationManager.GetRobotRadiusInCm();

		Map map = Map(&occupancyGrid, robotRadiusInCm, startLocation, goalLocation, mapHeight, mapWidth);
		Grid grid = map.grid;

		LocalizationManager localizationManager(hamster, occupancyGrid, mapHeight, mapWidth, mapResolution);
		Robot robot(hamster, &localizationManager, map.inflationRadius);

		PathPlanner pathPlanner = PathPlanner(&grid);
		string plannedRoute = pathPlanner.plannedRoute;

		WayPointsManager waypointsManager;

		int numOfWaypoints = waypointsManager.CalculateWaypoints(plannedRoute, startLocation, goalLocation);
		vector<Location> waypoints = waypointsManager.waypoints;

		// Print the map including the planned route and chosen waypoints
		DisplayManager displayManager = DisplayManager(&grid, plannedRoute, &waypoints, numOfWaypoints);
		//displayManager.PrintRouteCvMat();
		displayManager.PrintWaypoints(true);

		MovementManager movementManager(hamster);

		Location hamsterStartLocation = configurationManager.GetHamsterStartLocation();
		robot.Initialize(hamsterStartLocation);

		int waypointIndex = 0;
		Location currWaypoint;
		double deltaX = 0, deltaY = 0, deltaYaw = 0;

		while (hamster->isConnected())
		{
			try
			{
				Location sheker = {.x = 50.0, .y = 50.0, .yaw = 0};
				movementManager.MoveTo(&robot, &sheker);
				robot.UpdateLocation();

				/*displayManager.PrintRouteCvMat();
				sleep(10);*/

				while (waypointIndex < numOfWaypoints)
				{
					currWaypoint = waypoints.at(waypointIndex);

					// Convert cv::Mat location to HamsterAPI::Hamster location
					currWaypoint.x -= (mapWidth / 2);
					currWaypoint.y -= (mapHeight / 2);

					movementManager.MoveTo(&robot, &currWaypoint);
					waypointIndex++;

					robot.UpdateLocation();

					/*deltaX = robot.GetDeltaX();
					deltaY = robot.GetDeltaY();
					deltaYaw = robot.GetDeltaYaw();

					cout << "Real values:" << " deltaX : " << deltaX << " deltaY: " << deltaY << " deltaYaw : " << deltaYaw << endl;

					localizationManager.UpdateParticles(deltaX, deltaY, deltaYaw);
					displayManager.PrintRouteCvMat(localizationManager.GetParticles());
					localizationManager.PrintParticles();*/
				}

				movementManager.StopMoving();
				cout << "Hamster has reached its destination!" << endl;

				return 0;
			}
			catch (const HamsterAPI::HamsterError & message_error)
			{
				HamsterAPI::Log::i("Client", message_error.what());
			}
		}
	}
	catch (const HamsterAPI::HamsterError & connection_error)
	{
		HamsterAPI::Log::i("Client", connection_error.what());
	}

	return 0;
}
