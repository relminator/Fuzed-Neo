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
#include "Block.h"

#ifndef SPIRAL_H
#define SPIRAL_H

class Spiral : public Entity 
{

public:

	friend class SpiralFactory;
	
	Spiral();
	~Spiral();

	virtual void Update( Player &Mario, Map2D &Map );
	virtual void Draw( const glImage *spr, s32 z );
	virtual void DrawAABB( s32 z, int Color ); 
	virtual bool CollideWithPlayer( Player &Mario, Map2D &Map );
	
	void Init( s32 sx, s32 sy, int axis = 0 );
	
	
	
protected:

	int Axis;
	s32 Accel;
	int Direction;
	s32 Rotation;
	
};



//------------------------------------------------------------------------------
//	SpiralFactory
//------------------------------------------------------------------------------
class SpiralFactory : public EntityFactory 
{

public:
	SpiralFactory();
	~SpiralFactory();


	virtual void UpdateEntities( Player &Mario, Map2D &Map );
	virtual void DrawEntities( s32 z );
	virtual void DrawAABB( s32 z, int color );
	virtual void CollideWithPlayer( Player &Mario, Map2D &Map );
	virtual void CollideWithBlocks( BlockFactory &Blocks, Player &Mario );
	virtual void KillAllEntities();

	void Init( glImage* const Sprites );
	void Spawn( s32 x, s32 y, int axis = 0 );
	
private:

	std::vector<Spiral> Spirals;
	

};


#endif // SPIRAL_H
