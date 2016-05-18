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
#ifndef BOMB_H
#define BOMB_H

class Bomb 
{

public:
	Bomb();
	~Bomb();
	
	void Init( s32 sx, s32 sy, int Dir );
	void Update( MapContainer &Map );
	void Explode();
	void Kill();
	void Draw( const glImage *spr ) const;
	int CollideFloors( s32 ix, s32 iy, int &TileX, int &TileY, Map2D &Map );
	
	const s32& GetX() const 
	{
		return x;
	}
	
	const s32& GetY() const 
	{
		return y;
	}
	
	const s32& GetTileX() const 
	{
		return TileX;
	}
	
	const s32& GetTileY() const 
	{
		return TileY;
	}
	
	bool IsActive() const 
	{
		return Active;
	}
	
	bool IsKaBoom() const 
	{
		return KaBoom;
	}
	
	int GetDirection() const 
	{
		return Direction;
	}
	
	const s32& GetWidth() const 
	{
		return Width;
	}
	
	const s32& GetHeight() const 
	{
		return Height;
	}
	
	int GetTimer() const 
	{
		return Timer;
	}
	
	const AABB& GetAabb() const 
	{
		return Aabb;
	}

	const AABB& GetAabb2() const 
	{
		return Aabb2;
	}
	
	const AABB& GetAabb3() const 
	{
		return Aabb3;
	}
	
	const AABB& GetAabb4() const 
	{
		return Aabb4;
	}
	
	int GetTileYBelow() const 
	{
		return (y + Height*2) / TILE_SIZE_3D;
	}
	
	int GetOnTop() const
	{
		return OnTop;
	}
private:

	int 	Counter;
	int 	Timer;
	bool 	Active;
	
	s32		x;
	s32		y;
	s32		xv;
	s32		yv;
	
	int		Direction;     	// -1 left, 1 right
	bool	KaBoom;			// State to kill() the bomb  
	int 	Frame;
	int 	NumFrames;
	int 	BaseFrame;
	
	bool    OnTop;
	int 	TileX;
	int 	TileY;
	
	s32 	Width;
	s32		Height;
	AABB	Aabb;			// For moveable blocks and soft brick
	AABB	Aabb2;			// For enemies
	AABB	Aabb3;			// For moveable blocks  when falling more than 1/2 tile or something 
	AABB	Aabb4;			// Always on the front 
	bool 	ResetAABB3;
};

#endif // BOMB_H
