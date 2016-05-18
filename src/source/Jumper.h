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
#include "Bouncer.h"
#include "Block.h"


#ifndef JUMPER_H
#define JUMPER_H

class Jumper : public Bouncer
{

public:
	friend class JumperFactory;
	
	Jumper();
	~Jumper();
	
	virtual void Update( Player &Mario, Map2D &Map );
	
	void CollideOnMap( Map2D &Map );
	

};


//------------------------------------------------------------------------------
//	JumperFactory
//------------------------------------------------------------------------------
class JumperFactory : public EntityFactory 
{

public:
	JumperFactory();
	~JumperFactory();


	virtual void UpdateEntities( Player &Mario, Map2D &Map );
	virtual void DrawEntities( s32 z );
	virtual void DrawAABB( s32 z, int color );
	virtual void CollideWithPlayer( Player &Mario, Map2D &Map );
	virtual void CollideWithBlocks( BlockFactory &Blocks, Player &Mario );
	virtual void KillAllEntities();

	void Init( glImage* const Sprites );
	void Spawn( s32 sx, s32 sy, int direction = -1 );
	
protected:

	std::vector<Jumper> Jumpers;
	

};

#endif // JUMPER_H
