/*
 * MovementManager.h
 *
 *  Created on: Jun 28, 2017
 *      Author: user
 */

#ifndef MOVEMENTMANAGER_H_
#define MOVEMENTMANAGER_H_

#include "Globals.h"
#include "Robot.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "HamsterAPIClientCPP/Hamster.h"
using namespace std;
using namespace HamsterAPI;

class MovementManager
{
private:
	HamsterAPI::Hamster * hamster;

public:
	double * deltaX, * deltaY, * deltaYaw, * yaw;
	MovementManager(HamsterAPI::Hamster * hamster);
	void MoveTo(Robot * robot, Location currLocation, Location * destination);
	void GetScansBetween(double min, double max, std::vector<double> & distances);
	void MoveForward();
	void TurnLeft() ;
	void TurnRight();
	void MoveBackwards();
	void StopMoving() ;
	virtual ~MovementManager();
};

#endif /* MOVEMENTMANAGER_H_ */