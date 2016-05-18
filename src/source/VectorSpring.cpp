/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "VectorSpring.h"

static const s32 FORCE = floattof32(1);    // Arbitrary force constant
static const s32 FRICTION = floattof32(0.949996);	// Damping for spring
static const int MAGNITUDE = 5;		// Arbirtarry distance kinda work like dt(deltatime) increase = faster springs

// Force should have been F = -K * X or F = -K * ( Length - Restlength)
// But meh, this is faster and works just as well

VectorSpring::VectorSpring()
{

	x = 0;
	y = 0;
	xv = 0;
	yv = 0;
	ox = 0;
	oy = 0;
	Force = FORCE;
	Friction = FRICTION;
	Magnitude = MAGNITUDE;
	Mass = 400;
	PullAccel = Force / Mass;
	
}

VectorSpring::~VectorSpring()
{
}

void VectorSpring::Update()
{
	
	s32 Dx = x - ox;
	s32 Dy = y - oy;
	s32 PullForce = PullAccel * Magnitude;		// Change in velocity
    xv -= (Dx * PullForce) >> 12;	// Lessen the distance between the 2 points by subtracting 
    yv -= (Dy * PullForce) >> 12;	// Dist * change in velocity
    
    xv *= Friction;
    yv *= Friction;
  	
  	xv = xv >> 12;
  	yv = yv >> 12;
  	
  	x += xv;
  	y += yv;
	
	
}
