#ifndef DISPLAYMANAGER_H_
#define DISPLAYMANAGER_H_

#include "Globals.h"
#include "Grid.h"
#include "Particle.h"
#include <string>
#include <vector>
#include <queue>
#include <HamsterAPIClientCPP/Hamster.h>
using namespace std;
using namespace HamsterAPI;

class DisplayManager
{
private:
	Grid grid;
	int startRow;
	int startCol;
	int goalRow;
	int goalCol;
	vector<vector<bool> > occupationMap;
	int height;
	int width;
	vector<vector<int> > mapFromPlannedRoute;
	cv::Mat_<cv::Vec3b> routeCvMat;
	string plannedRoute;
	vector<Location> waypoints;
	void InitMapWithRoute();
	void InitMapWithParticles(vector<Particle *> particles);
	void ColorPixelByRoute(int currentCellValue, int i, int j);
	void ColorPixelByParticles(int currentCellValue, int i, int j);
public:
	DisplayManager(Grid * grid, string plannedRoute, vector<Location> * waypoints);
	void PrintRouteCvMat();
	void PrintRouteCvMat(vector<Particle *> particles);
	virtual ~DisplayManager();
};

#endif /* DISPLAYMANAGER_H_ */
