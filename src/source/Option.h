/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#ifndef OPTION_H
#define OPTION_H

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

#define MAX_SHADOWS 8

class Option 
{

public:
	Option();
	~Option();
	
	void Update();
	void Draw( const glImage *spr ) const;
	
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
	
	void SetFlipmode( int v )
	{
		Flipmode = v;
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
	
	int Counter;
	int Flipmode;
	int SpriteFrame;
	int BaseFrame;
	int MaxFrame;

	Part Shadows[MAX_SHADOWS];
};

#endif // OPTION_H
