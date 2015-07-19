#include "Robot.h"
#include "../Utils/PositionUtils.h"

using namespace Utils;
Robot::Robot(char* ip, int port, Location* startLocation)
{
	_pc = new PlayerClient(ip,port);
	_pp = new Position2dProxy(_pc);
	_lp = new LaserProxy(_pc);

	// TODO: Change
	_location  = new Location(startLocation->getX(),startLocation->getY(),startLocation->getYaw());

	int i;

	//Fix the player bug
	for(i=0;i<15;i++)
	{
		_pp->SetOdometry(startLocation->getX(),startLocation->getY(),startLocation->getYaw());
		_pc->Read();
	}

	_pp->SetMotorEnable(true);
}

void Robot::read()
{
	_pc->Read();
	this->_location->setX(this->_pp->GetXPos());
	this->_location->setY(this->_pp->GetYPos());
	this->_location->setYaw(this->_pp->GetYaw());
	this->_location->setYaw(Helper::KeepYawInRange(this->_location->getYaw()));
}

void Robot::getDelta(double &dX,double &dY,double &dYaw)
{
	dX = _pp->GetXPos() - _location->getX();
	dY = _pp->GetYPos() - _location->getY();
	dYaw = _pp->GetYaw() - _location->getYaw();

	//_location->setX(dX+ _location->getX());
	//_location->setY(dY+ _location->getY());

	PositionUtils::FixRad(dYaw);
}
void Robot::UpdateLocation(double x, double y, double yaw)//, bool isSetOdo)
{
	/*if (isSetOdo)
	{
		while (_pp->GetXPos() != x || _pp->GetYPos() != y ||_pp->GetYPos() != yaw)
		{
			_pp->SetOdometry(x,y,yaw);
		}
	}*/
	//_pp->SetOdometry(x,y,yaw);
}
void Robot::setSpeed(float speed, float angularSpeed)
{
	_pp->SetSpeed(speed,angularSpeed);
}

float Robot::getLaserDistance(int index)
{
	return _lp->GetRange(index);
}

Robot::~Robot()
{
	delete _lp;
	delete _pp;
	delete _pc;
}
