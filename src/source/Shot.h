/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <vector>
#include "Aabb.h"
#include "Explosion.h"
#include "Particles.h"
#include "GlobalConstants.h"
#include "Map2D.h"

#ifndef SHOT_H
#define SHOT_H

#define PLAYER_SHOT_SPEED floattof32( 0.20 )

class Shot 
{

public:

	enum ID
	{
		NONE = 0,
		WAVE,
		NORMAL,
		BOUNCE,
		THROW,
		WATER,
		MAGNETO
	};
	
	
	Shot();
	~Shot();
	
	
	void InitNormal( s32 sx, s32 sy, int frame, int level, int Direction, int SpriteSize );
	void InitWave( s32 sx, s32 sy, s32 angle, int frame, int level, int Direction, int SpriteSize );
	void InitBounce( s32 sx, s32 sy, s32 angle, int frame, int level, int Direction, int SpriteSize );
	void InitThrow( s32 sx, s32 sy, int frame, int level, int Direction, int SpriteSize );
	void InitWater( s32 sx, s32 sy, int frame, int level, int Direction, int SpriteSize );
	void InitMagneto( s32 sx, s32 sy, int frame, int level, int xDirection, int SpriteSize );
	void Update( Map2D &Map );
	void UpdateMagnetoRight( Map2D &Map );
	void UpdateMagnetoLeft( Map2D &Map );
	void Kill();

	inline bool IsActive() const
	{
		return Active;
	}
	
	inline ID GetID() const
	{
		return ShotID;
	}
	
	inline s32 GetX() const
	{
		return x;
	}
	
	inline s32 GetY() const
	{
		return y;
	}
	
	inline s32 GetDx() const
	{
		return Dx;
	}
	
	inline s32 GetDy() const
	{
		return Dy;
	}
	
	inline s32 GetWidth() const
	{
		return Width;
	}
	
	inline s32 GetHeight() const
	{
		return Height;
	}
	
	
	inline int GetDrawFrame() const
	{
		return BaseFrame + Frame;
	}
	
	inline AABB GetAABB() const
	{
		return Aabb;
	}
	
	int GetTile( s32 ix, s32 iy, Map2D &Map )
	{
		int TileX = ix / TILE_SIZE_3D;
		int TileY = iy / TILE_SIZE_3D;
		return Map(TileX,TileY);
		
	}
	
	
private:

	int 	Counter;
	bool 	Active;
	ID 		ShotID;
	
	s32		x;
	s32		y;
	s32		Dx;
	s32		Dy;
	s32		Angle;		// 20.12
	s32		Radius;		// 20.12
	int		Direction;     // -1 on air, 0 floor, 1 right wall, 2 left floor, 3 ceiling
	
	int 	Energy;
	int 	Life;
	
	int 	Frame;
	int 	NumFrames;
	int 	BaseFrame;
	
	s32 	Width;
	s32		Height;
	AABB	Aabb;
	
};

#endif // SHOT_H
