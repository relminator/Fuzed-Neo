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
#include "Player.h"

#ifndef ENTITY_H
#define ENTITY_H


class Entity 
{

public:
	Entity();
	~Entity();

	virtual void Update( Player &Mario, Map2D &Map ) = 0;
	virtual void Draw( const glImage *spr, s32 z ) = 0;
	virtual void DrawAABB( s32 z, int Color ) = 0;
	virtual bool CollideWithPlayer( Player &Mario, Map2D &Map ) = 0;
	
	bool IsOnWater( s32 ix, s32 iy, Map2D &Map );
	bool CollideFloors( s32 ix, s32 iy, int &TileY, Map2D &Map );
	bool CollideWalls( s32 ix, s32 iy, int &TileX, Map2D &Map );
	bool SeePlayerHorizontal( Player &Mario, Map2D &Map, int TileDistance );
	int GetPlayerDistanceX( Player &Mario );
	int GetPlayerDistanceY( Player &Mario );
	void ResolveAABBS();
	void StunMe( int delay );
	void ResolveStates();
	void Kill();
	
	void SetActive( bool Active ) 
	{
		this->Active = Active;
	}
	
	void SetHP(int HP) 
	{
		this->HP = HP;
	}
	
	bool IsActive() const 
	{
		return Active;
	}

	const s32 &GetHeight() const 
	{
		return Height;
	}

	const s32 &GetWidth() const 
	{
		return Width;
	}

	const AABB &GetAabb() const 
	{
		return aabb;
	}

	const AABB &GetAabb2() const 
	{
		return aabb2;
	}
	
	const s32 &GetX() const 
	{
		return x;
	}

	const s32 &GetY() const 
	{
		return y;
	}

	int GetTileX() const
	{
		return x / TILE_SIZE_3D;
	}
	
	int GetTileY() const
	{
		return y / TILE_SIZE_3D;
	}
	
	int GetHP() const 
	{
		return HP;
	}
	
	int GetScore() const 
	{
		return Score;
	}
	
	int GetTile( s32 ix, s32 iy, Map2D &Map )
	{
		int TileX = ix / TILE_SIZE_3D;
		int TileY = iy / TILE_SIZE_3D;
		return Map(TileX,TileY);
		
	}
	
	void SetTile( s32 ix, s32 iy, int v, Map2D &Map )
	{
		int TileX = ix / TILE_SIZE_3D;
		int TileY = iy / TILE_SIZE_3D;
		Map(TileX,TileY) = v;
	}
	
protected:

	bool Active;
	int HP;
	int MaxHP;
	int Score;

	s32 x;
	s32 y;
	s32 xv;
	s32 yv;
	s32 Speed;

	s32 Width;
	s32 Height;

	int Counter;
	int Flipmode;
	int Frame;
	int BaseFrame;
	int MaxFrame;

	int StunCoolDown;
	bool Stunned;
	
	AABB aabb;
	AABB aabb2;
	
};

#endif // ENTITY_H
