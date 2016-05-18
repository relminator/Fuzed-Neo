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

#ifndef MAGNETO_H
#define MAGNETO_H

class Magneto : public Entity 
{

public:

	friend class MagnetoFactory;
	
	Magneto();
	~Magneto();

	virtual void Update( Player &Mario, Map2D &Map );
	virtual void Draw( const glImage *spr, s32 z );
	virtual void DrawAABB( s32 z, int Color ); 
	virtual bool CollideWithPlayer( Player &Mario, Map2D &Map );
	
	void Init( s32 sx, s32 sy, int axis = 0 );
	
	
	
protected:
	void UpdateMagnetoRight( Map2D &Map );
	void UpdateMagnetoLeft( Map2D &Map );
	
	int Axis;
	int Direction;
	s32 Rotation;
	
};



//------------------------------------------------------------------------------
//	MagnetoFactory
//------------------------------------------------------------------------------
class MagnetoFactory : public EntityFactory 
{

public:
	MagnetoFactory();
	~MagnetoFactory();


	virtual void UpdateEntities( Player &Mario, Map2D &Map );
	virtual void DrawEntities( s32 z );
	virtual void DrawAABB( s32 z, int color );
	virtual void CollideWithPlayer( Player &Mario, Map2D &Map );
	virtual void CollideWithBlocks( BlockFactory &Blocks, Player &Mario );
	virtual void KillAllEntities();

	void Init( glImage* const Sprites );
	void Spawn( s32 x, s32 y, int axis = 0 );
	
private:

	std::vector<Magneto> Magnetos;
	

};

#endif // MAGNETO_H
