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
#include "Spiral.h"
#include "Block.h"

#ifndef DIAGSPIRAL_H
#define DIAGSPIRAL_H

class DiagSpiral : public Spiral
{

public:
	friend class DiagSpiralFactory;
	
	DiagSpiral();
	~DiagSpiral();

	virtual void Update( Player &Mario, Map2D &Map );
	
	void Init( s32 sx, s32 sy, int axis = 0 );
	void CollideOnMap( Map2D &Map );
	
private:
	int sxv;
	int syv;

};

//------------------------------------------------------------------------------
//	DiagDiagSpiralFactory
//------------------------------------------------------------------------------
class DiagSpiralFactory : public EntityFactory 
{

public:
	DiagSpiralFactory();
	~DiagSpiralFactory();


	virtual void UpdateEntities( Player &Mario, Map2D &Map );
	virtual void DrawEntities( s32 z );
	virtual void DrawAABB( s32 z, int color );
	virtual void CollideWithPlayer( Player &Mario, Map2D &Map );
	virtual void CollideWithBlocks( BlockFactory &Blocks, Player &Mario );
	virtual void KillAllEntities();

	void Init( glImage* const Sprites );
	void Spawn( s32 x, s32 y, int axis = 0 );
	
private:

	std::vector<DiagSpiral> DiagSpirals;
	

};


#endif // DIAGSPIRAL_H
