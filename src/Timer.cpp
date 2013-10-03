#include "Timer.h"


Timer::Timer(){
}
Timer::~Timer(){
}
//---------------------------------


void Timer::intialize(int timePeriod){
	period = timePeriod;
	done   = false;	
	timeNow = 0;
	timeLast = 0;
	percentage = 0;
}
//---------------------------------


void	Timer::update()
{
	if (done == false)
	{		
		timeNow = ofGetElapsedTimeMillis();
		
		percentage = (timeNow - timeLast) / float(period);
		if (timeNow - timeLast >= period)
		{
			done = true;
		}
		
	}	
		
}
//---------------------------------

bool	Timer::isDone()
{
	if (done == true)
	{
		return true;
	}
	else {
		return false;
	}
}
//---------------------------------

void	Timer::start()
{
	done = false;
	timeNow = ofGetElapsedTimeMillis();
	timeLast = timeNow;
	percentage = 0;
}
//---------------------------------

float	Timer::getPercent()
{
	return percentage;
}
//---------------------------------

int	Timer::getTimeNow()
{
	return timeNow;
}
//---------------------------------


float	Timer::getTimeEllapsed()
{
	return (timeNow - timeLast)/1000.0;
}
//---------------------------------



