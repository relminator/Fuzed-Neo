/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Coin.h"

#define MAX_COINS 16

Coin::Coin()
{
	
	Active = false;	
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 1;
	Speed = 0; 
	
	Width = (16 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (16 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 69;
	MaxFrame = 4;
	Score = 100;
}

Coin::~Coin()
{
}


void Coin::Init( s32 sx, s32 sy )
{
	Active = true;
	
	x = sx;   // +xv so offset right to fix sticky bug when spawning on edges
	y = sy;
	
	xv = 0;
	yv = 1;
	
	ResolveAABBS();
	
}


bool Coin::CollideFloors( s32 ix, s32 iy, int &TileY, Map2D &Map )
{
	s32 TileXpixels = ix - (ix % TILE_SIZE_3D);
	s32 TestEnd = ix + Width;
	
	TileY = iy / TILE_SIZE_3D;
	
	int TileX = TileXpixels / TILE_SIZE_3D;
	
	while(TileXpixels <= TestEnd)
	{
		if( (Map(TileX,TileY) >= BRICK) || (Map(TileX,TileY) == TOP_LADDER) ) return true;	
		TileX++;
		TileXpixels += TILE_SIZE_3D;
	}	
	
	return false;
}

void Coin::CollideOnMap( Map2D &Map )
{
	
	int TileY;
	
	
		if( CollideFloors( x, y + yv + Height, TileY, Map ) )
		{
			y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
			yv = -JUMP_HEIGHT/2;
		}
		else
		{
			y += yv;
			yv += GRAVITY;
			if( yv > FALL_SPEED_THRESHOLD ) yv = FALL_SPEED_THRESHOLD;
		}
	
	

}


void Coin::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	ResolveStates();
	
	CollideOnMap( Map );
	
	if( (Counter & 3) == 0 )
	{
		Frame = ( Frame + 1 ) % MaxFrame;
	}

	// BLOCK_COLLIDE only collides with blocks and not with player
	SetTile( x + Width/2, y + Height/2, BLOCK_COLLIDE, Map );
	// aabb
	ResolveAABBS();
	
	
	
}

void Coin::Draw( const glImage *spr, s32 z )
{
	glSprite3D( x, y, z, Flipmode, &spr[BaseFrame+Frame] );
}
	

void Coin::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(31,31,31) );
}

bool Coin::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	
	
	
	if( aabb.Intersects( Mario.GetAABB() ) )
	{
		
		
		if( aabb2.Intersects( Mario.GetAABB2() ) )
		{
			Vector2df32 Pos;
			Pos.x = x + Width/2;
			Pos.y = y + Height/2;
			Particle::Instance()->Spawn( Pos, 24 );
			Sound::Instance()->PlaySFX(E_SFX_COIN_UP);
			Mario.AddToScore(Score);
			Kill();
			return true;
		}
		
			
		return true;
	}
	
	
		

	return false;
}



//------------------------------------------------------------------------------
//	CoinFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
CoinFactory::CoinFactory()
{
	
}
	
CoinFactory::~CoinFactory()
{
	Coins.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void CoinFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	Coins.resize( MAX_COINS );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void CoinFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	NumEntities = 0;
	std::vector<Coin>::iterator iter;
	for( iter = Coins.begin(); iter != Coins.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->Update( Mario, Map );
			NumEntities++;
		}
	}
	
	
}

//--------------------------------------
//	
//--------------------------------------
void CoinFactory::DrawEntities( s32 z )
{
	
	std::vector<Coin>::iterator iter;
	for( iter = Coins.begin(); iter != Coins.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->Draw( Images, z );
			z -= 4;
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void CoinFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<Coin>::iterator iter;
	for( iter = Coins.begin(); iter != Coins.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->DrawAABB( z, color );
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void CoinFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<Coin>::iterator iter;
	for( iter = Coins.begin(); iter != Coins.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->CollideWithPlayer( Mario, Map );
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void CoinFactory::Spawn( s32 x, s32 y )
{
	
	std::vector<Coin>::iterator iter;
	for( iter = Coins.begin(); iter != Coins.end(); ++iter )
	{
	
		if( !iter->Active )		// insert on a free slot
		{
			iter->Init( x, y );
			break;
		}
	
	}
	
}	

//--------------------------------------
//	
//--------------------------------------
void CoinFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	
}

//--------------------------------------
//	
//--------------------------------------
void CoinFactory::KillAllEntities()
{
	std::vector<Coin>::iterator iter;
	for( iter = Coins.begin(); iter != Coins.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}


