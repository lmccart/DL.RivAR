/*
 *  timer.h
 *  AR-Kiosk
 *
 *  Created by Dennis Rosenfeld on 11-06-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


//Simple timer Class

#ifndef _TIMER
#define _TIMER

#include "ofMain.h"

#define DEFAULT_COUNT_START 0
#define DEFAULT_COUNT_END	0
#define DEFAULT_COUNT_DELAY	1000

class Timer{
protected:
	
	int timeNow;
	int timeLast;
	int period;
	float percentage;

	bool done;
	
public:
	
	Timer();
	~Timer();
	void intialize(int timePeriod);
	
	void	update();
	bool	isDone();
	
	
	int		getTimeNow();
	int		getTimeLast();
	float	getTimeEllapsed();
	int		getPeriod();
	float	getPercent();
	
	void	setPeriod();
	void	reset();
	
	void	start();
	void	stop();
	void	pause();
	
	
	
};
#endif