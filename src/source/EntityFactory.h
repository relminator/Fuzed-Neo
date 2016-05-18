/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <vector>

#include "Player.h"
#include "Map2D.h"
#include "Entity.h"


#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

class BlockFactory;

class EntityFactory 
{

public:
	EntityFactory();
	~EntityFactory();
	
	virtual void UpdateEntities( Player &Mario, Map2D &Map ) = 0;
	virtual void DrawEntities( s32 z ) = 0;
	virtual void DrawAABB( s32 z, int color ) = 0;
	virtual void CollideWithPlayer( Player &Mario, Map2D &Map ) = 0;
	virtual void CollideWithBlocks( BlockFactory &Blocks, Player &Mario ) = 0;
	virtual void KillAllEntities() = 0;
	
	int GetNumEntities() const 
	{
		return NumEntities;
	}
	
protected:

	glImage *Images;
	
	int NumEntities;
	
	
};

#endif // ENTITYFACTORY_H
