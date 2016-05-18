/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <stdio.h>

#include "gl2d.h"
#include "Vector2df32.h"
#include "UTIL.h"

#include "GlobalConstants.h"
#include "Map2D.h"
#include "Renderer3D.h"
#include "AABB.h"
#include "Entity.h"
#include "EntityFactory.h"

#ifndef BLOCK_H
#define BLOCK_H

#define MAX_BLOCKS 32

class Block : public Entity 
{

public:
	friend class BlockFactory;
	
	enum 
	{
		SOFT = 0,
		HARD
	};
	
	Block();
	~Block();

	virtual void Update( Player &Mario, Map2D &Map );
	virtual void Draw( const glImage *spr, s32 z );
	virtual void DrawAABB( s32 z, int Color ); 
	virtual bool CollideWithPlayer( Player &Mario, Map2D &Map );
		
	void Init( s32 sx, s32 sy, int type, int direction = -1 );
	bool CollideFloors( s32 ix, s32 iy, int &TileY, Map2D &Map );
	bool CollideWalls( s32 ix, s32 iy, int &TileX, Map2D &Map );	
	void CollideOnMap( Map2D &Map );
	bool CollideWithBlock( Block &B );
	
	int GetID() const
	{
		return ID;
	}
	
	int GetDirection() const 
	{
		return Direction;
	}
	
	bool IsThrown() const
	{
		if( ID == HARD ) 
		{
			if( Thrown )
			{
				return true;
			}
		}
		return false;
	}
	
	
	const s32 GetCx() const 
	{
		return cx;
	}
	
	const s32 GetCy() const 
	{
		return cy;
	}
	
private:

	int ID;
	bool AutoControl;
	int AutoControlDistance;
	int Direction;
	bool Thrown;
	
	s32 cx;
	s32 cy;

};


//------------------------------------------------------------------------------
//	GreenTurtleFactory
//------------------------------------------------------------------------------
class BlockFactory : public EntityFactory 
{

public:
	BlockFactory();
	~BlockFactory();


	virtual void UpdateEntities( Player &Mario, Map2D &Map );
	virtual void DrawEntities( s32 z );
	virtual void DrawAABB( s32 z, int color );
	virtual void CollideWithPlayer( Player &Mario, Map2D &Map );
	virtual void CollideWithBlocks( BlockFactory &Blocks, Player &Mario );
	virtual void KillAllEntities();

	void Init( glImage* const Sprites );
	void Spawn( s32 sx, s32 sy, int type, int direction = -1 );
	void SortAll();
	
	const Block GetBlock( int i) const;
	
private:
	
	
	void CollideWithBlocks();
	
	std::vector<Block> Blocks;
};



#endif // BLOCK_H
