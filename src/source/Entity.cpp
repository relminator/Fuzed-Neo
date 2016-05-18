/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Entity.h"

Entity::Entity()
{
	
	Active = false;	
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 0;
	Speed = floattof32(0.03); 
	
	Width = (32 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (32 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 0;
	MaxFrame = 1;
	
	HP = 1;
	MaxHP = 1;
	Score = 0;
	Stunned = false;
	StunCoolDown = 30;
	
}

Entity::~Entity()
{
}

bool Entity::IsOnWater( s32 ix, s32 iy, Map2D &Map )
{
	int TileX = ix / TILE_SIZE_3D;
	int TileY = iy / TILE_SIZE_3D;
	if( (Map(TileX,TileY) == WATER ) || (Map(TileX,TileY) == TOP_WATER ) ) return true;
	
	return false;
}	

bool Entity::CollideFloors( s32 ix, s32 iy, int &TileY, Map2D &Map )
{
	s32 TileXpixels = ix - (ix % TILE_SIZE_3D);
	s32 TestEnd = ix + Width;
	
	TileY = iy / TILE_SIZE_3D;
	
	int TileX = TileXpixels / TILE_SIZE_3D;
	
	while(TileXpixels <= TestEnd)
	{
		if( (Map(TileX,TileY) >= BRICK ) ) return true;	
		TileX++;
		TileXpixels += TILE_SIZE_3D;
	}	
	
	return false;
}


bool Entity::CollideWalls( s32 ix, s32 iy, int &TileX, Map2D &Map )
{
	s32 TileYpixels = iy - (iy % TILE_SIZE_3D);
	s32 TestEnd = iy + Height;
	
	TileX = ix / TILE_SIZE_3D;
	
	int TileY = TileYpixels / TILE_SIZE_3D;
	
	while(TileYpixels <= TestEnd)
	{
		if( Map(TileX,TileY) >= BRICK ) return true;	
		TileY++;
		TileYpixels += TILE_SIZE_3D;
	}	

	return false;
}


int Entity::GetPlayerDistanceX( Player &Mario )
{
	
	int Tx = x / TILE_SIZE_3D;
	
	int PlayerTx = Mario.GetTileX();
	
	return abs( PlayerTx - Tx );
	
}

int Entity::GetPlayerDistanceY( Player &Mario )
{
	
	int Ty = y / TILE_SIZE_3D;
	
	int PlayerTy = Mario.GetTileY();
	
	return abs( PlayerTy - Ty );
	
}

bool Entity::SeePlayerHorizontal( Player &Mario, Map2D &Map, int TileDistance )
{
	
	int Tx = ( x + Width / 2 ) / TILE_SIZE_3D;
	int Ty = y / TILE_SIZE_3D;
	
	int PlayerTx = Mario.GetTileX();
	int PlayerTy = Mario.GetTileY();
	
	int dist = ( PlayerTx - Tx );
	
	if( Ty == PlayerTy )
	{
		if( abs(dist) < TileDistance )
		{
			// Check to see if there is any block blocking the view
			if( dist > 0 )
			{
				for( int i = 0; i < dist; i++ )
				{
					if( Map( Tx + i, Ty ) >= BRICK ) return false;
				}
			}
			else
			{
				for( int i = 0; i < dist; i++ )
				{
					if( Map( Tx - i, Ty ) >= BRICK ) return false;
				}
			}
			
			return true;
		}
	}
	
	return false;
}

void Entity::ResolveAABBS()
{
	
	aabb.Init( x, y, Width, Height );
	aabb2.Init( x, y, Width, Height );
	aabb2.Resize( Width/4, Height/4 );
	
}
	
void Entity::StunMe( int delay )
{
	StunCoolDown = delay;
	Stunned = true;
}

void Entity::ResolveStates()
{
	if( Stunned )
	{
		StunCoolDown--;
		if( StunCoolDown < 0 )
		{
			Stunned = false;
		}
	}
	
}


void Entity::Kill()
{

				
	Active = false;
	
	x = -100 << 12;
	y = -100 << 12;
	
	
	ResolveAABBS();
	
}

