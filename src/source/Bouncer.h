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


#ifndef BOUNCER_H
#define BOUNCER_H

class Bouncer : public Entity 
{

public:
	friend class BouncerFactory;
	

	Bouncer();
	~Bouncer();

	virtual void Update( Player &Mario, Map2D &Map );
	virtual void Draw( const glImage *spr, s32 z );
	virtual void DrawAABB( s32 z, int Color ); 
	virtual bool CollideWithPlayer( Player &Mario, Map2D &Map );
	
	void Init( s32 sx, s32 sy, int direction = -1 );
	
	void CollideOnMap( Map2D &Map );

protected:

	s32 JumpHeight;
};


//------------------------------------------------------------------------------
//	BouncerFactory
//------------------------------------------------------------------------------
class BouncerFactory : public EntityFactory 
{

public:
	BouncerFactory();
	~BouncerFactory();


	virtual void UpdateEntities( Player &Mario, Map2D &Map );
	virtual void DrawEntities( s32 z );
	virtual void DrawAABB( s32 z, int color );
	virtual void CollideWithPlayer( Player &Mario, Map2D &Map );
	virtual void CollideWithBlocks( BlockFactory &Blocks, Player &Mario );
	virtual void KillAllEntities();

	void Init( glImage* const Sprites );
	void Spawn( s32 sx, s32 sy, int direction = -1 );
	
protected:

	std::vector<Bouncer> Bouncers;
	

};

#endif // BOUNCER_H
