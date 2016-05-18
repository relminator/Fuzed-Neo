/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Bomb.h"

#define MAX_TIMER ( 60 * 3 )

Bomb::Bomb()
{
	Active = false;
	KaBoom = false;
	
	Counter = 0;
	
	x = 0;
	y = 0;
	xv = 0;
	yv = 0;
	
	Timer = MAX_TIMER;
	
	Width = (16 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (16 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Frame= 0;
	BaseFrame = 0;
	NumFrames = 4;
	
	OnTop = false;
	
	Direction = 1;
	ResetAABB3 = false;
}

Bomb::~Bomb()
{
}

void Bomb::Init( s32 sx, s32 sy, int Dir )
{
	Active = true;
	KaBoom = false;
	
	Counter = 0;
	Direction = Dir;
	x = sx;
	y = sy;
	xv = 0;
	yv = 0;
	
	Timer = MAX_TIMER;
	
	Width = (16 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (16 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Frame= 0;
	BaseFrame = 0;
	NumFrames = 4;
	ResetAABB3 = false;
	OnTop = false;
	
}

void Bomb::Update( MapContainer &Map )
{
	
	if( KaBoom )
	{
		Kill();
		return;
	}
	
	Counter++;
	
	if( (Counter & 3) == 0 )
	{
		Frame = ( Frame + 1 ) % NumFrames;
	}
	
	
	s32 iy = y - Height/2;
	s32 ix = x - Width/2;
		 
	bool Collided = CollideFloors( ix, iy + yv + Height, TileX, TileY, Map.Collision );
	
	if( Collided )
	{
		iy = ( TileY * TILE_SIZE_3D ) - Height - ONE;
		yv = 0;									
		OnTop = true;
	}
	else
	{
		iy += yv;
		yv += GRAVITY;
		if( yv > FALL_SPEED_THRESHOLD ) yv = FALL_SPEED_THRESHOLD;
		OnTop = false;
	}

	y = iy + Height/2;
	
	// Reset AABB
	s32 offset = Direction * (Width/2);
	//Aabb.Init( x - Width/2, y - Height/2, Width, Height*2 );
	Aabb.Init( x - offset, y - Height/2, 1, Height*2 );
	Aabb2.Init( x - Width, y - Height, Width*2, Height*2 );
	Aabb3.Init( x - offset, y - Height/2, 1, Height*2 );
	Aabb4.Init( x + offset, y - Height/2, 1, Height*2 );
	
	if( yv > 512 )
	{
		ResetAABB3 = true;
	}
	
	if( ResetAABB3 )   // move to center when falling
	{
		Aabb3.Init( x, y - Height/2, 1, Height*2 );		
	}
	
	
	Timer--;
	if( Timer < 0 )
	{
		int Atx =  (x - offset) / TILE_SIZE_3D;
		int Aty =  GetTileYBelow();
		
		// Center
		if( Map.Collision( Atx, Aty ) == SOFT_BRICK )
		{
			AABB Taabb;
			Taabb.Init( Atx * TILE_SIZE_3D, Aty * TILE_SIZE_3D, TILE_SIZE_3D, TILE_SIZE_3D );
			if( Taabb.Intersects(Aabb) )
			{
				Map.Collision( Atx, Aty ) = NONE;
				Map.Base( Atx, Aty ) = NONE;
				
				Explosion::Instance()->Spawn( Atx * TILE_SIZE_3D + TILE_SIZE_3D/2,
											  Aty * TILE_SIZE_3D + TILE_SIZE_3D/2,
											  0,
											  0,
											  Explode::MEDIUM_4,
											  Explode::SMALL_2 );
				Explode();
			}
		}

		if( !KaBoom )
		{
			if( Collided )
			{
				if( Map.Collision( TileX, TileY ) == SOFT_BRICK )
				{
					Map.Collision( TileX, TileY ) = NONE;
					Map.Base( TileX, TileY ) = NONE;
					
					Explosion::Instance()->Spawn( TileX * TILE_SIZE_3D + TILE_SIZE_3D/2,
												  TileY * TILE_SIZE_3D + TILE_SIZE_3D/2,
												  0,
												  0,
												  Explode::MEDIUM_4,
												  Explode::SMALL_2 );
				}
			}
		}
		
		Explode();
		
	}
	
	
}

void Bomb::Explode()
{
	Vector2df32 Pos;
	Pos.x = x;
	Pos.y = y;
	Particle::Instance()->Spawn( Pos, 24 );
	
	Explosion::Instance()->Spawn( Pos.x,
								  Pos.y,
								  0,
								  0,
								  Explode::MEDIUM_4 );
	
	Sound::Instance()->PlaySFX(E_SFX_EXPLODE);
	
	KaBoom = true;
}

void Bomb::Kill()
{
	Active = false;
	KaBoom = false;
	x = -300 << 12;
	y = -300 << 12;
	Aabb.Init( x - Width/2, y - Height /2, Width, Height );
	Aabb2.Init( x - Width/2, y - Height /2, Width, Height );
	Aabb3.Init( x - Width/2, y - Height /2, Width, Height );
	Aabb4.Init( x - Width/2, y - Height /2, Width, Height );
}

void Bomb::Draw( const glImage *spr ) const
{
	glSprite3D( x - Width/2, y - Height/2, 8, GL_FLIP_NONE, &spr[BaseFrame + Frame] );
}


int Bomb::CollideFloors( s32 ix, s32 iy, int &TileX, int &TileY, Map2D &Map )
{
	s32 TileXpixels = ix - (ix % TILE_SIZE_3D);
	s32 TestEnd = ix + Width;
	
	TileY = iy / TILE_SIZE_3D;
	
	TileX = TileXpixels / TILE_SIZE_3D;
	
	while(TileXpixels <= TestEnd)
	{
		int Tile = Map(TileX,TileY);
		if(  Tile >= BRICK  ) return Tile;	
		TileX++;
		TileXpixels += TILE_SIZE_3D;
	}	
	
	return false;
}
