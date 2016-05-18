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


#ifndef VANISHPLATFORM_H
#define VANISHPLATFORM_H

#define MAX_VANISH_PLATFORMS 16

class VanishPlatform : public Entity 
{

public:
	friend class VanishPlatformFactory;
	
	
	
	VanishPlatform();
	~VanishPlatform();

	virtual void Update( Player &Mario, Map2D &Map );
	virtual void Draw( const glImage *spr, s32 z );
	virtual void DrawAABB( s32 z, int Color ); 
	virtual bool CollideWithPlayer( Player &Mario, Map2D &Map );
		
	void Init( s32 sx, s32 sy );
	bool CollideFloors( s32 ix, s32 iy, int &TileY, Map2D &Map );
	bool CollideWalls( s32 ix, s32 iy, int &TileX, Map2D &Map );	
	void CollideOnMap( Map2D &Map );
	
	bool IsVanishing() const
	{
		return Vanishing;
	}
	
private:

	int VanishCounter;
	bool Vanishing;
	

};


//------------------------------------------------------------------------------
//	GreenTurtleFactory
//------------------------------------------------------------------------------
class VanishPlatformFactory : public EntityFactory 
{

public:
	VanishPlatformFactory();
	~VanishPlatformFactory();


	virtual void UpdateEntities( Player &Mario, Map2D &Map );
	virtual void DrawEntities( s32 z );
	virtual void DrawAABB( s32 z, int color );
	virtual void CollideWithPlayer( Player &Mario, Map2D &Map );
	virtual void CollideWithBlocks( BlockFactory &Blocks, Player &Mario );
	virtual void KillAllEntities();

	void Init( glImage* const Sprites );
	void Spawn( s32 sx, s32 sy );
	
	const VanishPlatform GetVanishPlatform(int i) const;
	
private:
	
	
	void CollideWithVanishPlatforms();
	
	std::vector<VanishPlatform> VanishPlatforms;
};


#endif // VANISHPLATFORM_H
