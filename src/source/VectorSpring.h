/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <maxmod9.h>

#include <stdio.h>

#include "gl2d.h"
#include "Vector2df32.h"
#include "UTIL.h"
#include "Sound.h"

#include "GlobalConstants.h"
#include "Map2D.h"
#include "Renderer3D.h"
#include "AABB.h"
#include "Explosion.h"
#include "Particles.h"

#ifndef VECTORSPRING_H
#define VECTORSPRING_H

class VectorSpring {

public:
	VectorSpring();
	~VectorSpring();
	
	void Update();
	
	const s32 &GetX() const 
	{
		return x;
	}
	
	const s32 &GetY() const 
	{
		return y;
	}
	
	const s32 &GetXv() const 
	{
		return xv;
	}
	
	const s32 &GetYv() const 
	{
		return yv;
	}
	
	void SetOx( s32 v )
	{
		ox = v;
	}
	
	void SetOy( s32 v )
	{
		oy = v;
	}
	
	void SetX( s32 v )
	{
		x = v;
	}
	
	void SetY( s32 v )
	{
		y = v;
	}
	
	void SetXv( s32 v )
	{
		xv = v;
	}
	
	void SetYv( s32 v )
	{
		yv = v;
	}
	
	void SetParameters( int mass, s32 force, s32 friction, s32 magnitude )
	{
		Force = force;
		Mass = mass;
		PullAccel = Force / Mass;
		Friction = friction;
		Magnitude = magnitude;
	}
	
private:

	s32 	x;
	s32		y;
	s32		xv;
	s32		yv;
	s32 	ox;
	s32		oy;
	s32		PullAccel;
	int		Mass;
	s32		Force;
	s32 	Friction;
	s32  	Magnitude;

};

#endif // VECTORSPRING_H
