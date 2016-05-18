/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include "Vector2df32.h"
#include "UTIL.h"
#include "GlobalConstants.h"
#include "Map2D.h"


#ifndef CAMERA_H
#define CAMERA_H

class Vector3Df32
{
public:
	
	s32 x,y,z;
};



class Camera
{
public:
	
	Camera();
	~Camera();
	void Update( s32 x, s32 y, int signx, int signy, s32 &offx, s32 &offy, Map2D &Map );
	void Update( s32 x, s32 y, int signx, int signy, Map2D &Map );
	void UpdatePaused( s32 &x, s32 &y, s32 PlayerX, s32 PlayerY, Map2D &Map );
	void Look();
	void Zoom( s32 value );
	
	void SetMode( int v )
	{
		Mode = v;
	}
	
	const s32& GetEyeDistanceFromScreen() const 
	{
		return EyeDistanceFromScreen;
	}
	
private:
	
	void Follow( s32 x, s32 y );
	void Follow( s32 x, s32 y, Map2D &Map );
	void Follow( s32 x, s32 y, int signx, int signy, Map2D &Map );
	void Follow( s32 x, s32 y, int signx, int signy, int limit, Map2D &Map );
	void Follow( s32 x, s32 y, int signx, int signy, s32 &offx, s32 &offy, Map2D &Map );
	void FollowLimit( s32 &x, s32 &y, Map2D &Map );
	
	Vector3Df32 Position;
	Vector3Df32 Target;
	Vector3Df32 Up;
	
	s32 EyeDistanceFromScreen;
	int Mode;
	
	
};



#endif // CAMERA_H
