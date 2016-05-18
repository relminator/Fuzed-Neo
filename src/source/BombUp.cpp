/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "BombUp.h"

#define MAX_BOMB_UP 8


BombUp::BombUp()
{
	
	Active = false;	

	x = 0;
	y = 0; 
	xv = 0; 
	yv = 0;
	
	
	Width = (24 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (24 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 1;
	MaxFrame = 4;
	
	Stunned = false;
	
}

BombUp::~BombUp()
{
}


void BombUp::Init( s32 sx, s32 sy )
{
	Active = true;
	
	x = sx;
	y = sy;
	
		
}

void BombUp::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	ResolveStates();
	
	
	//SetTile( x + Width/2, y + Height/2, BLOCK_COLLIDE, Map );
	
	if( (Counter & 3) == 0 )
	{
		Frame = ( Frame + 1 ) % MaxFrame;
	}
	// aabb
	ResolveAABBS();
	
	
}

void BombUp::Draw( const glImage *spr, s32 z )
{
	int Scale = (1 << 12) + abs(sinLerp( Counter * 512 ));
	glSpriteRotateScale3D( x + Width/2, y + Height/2 + Height/4, z, 0, Scale, Scale, Flipmode, &spr[BaseFrame+Frame] );
}
	

void BombUp::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
}



bool BombUp::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	if( aabb2.Intersects( Mario.GetAABB2() ) )
	{
		Mario.IncBombsLeft();
		Vector2df32 Pos;
		Pos.x = x + Width/2;
		Pos.y = y + Height/2;
		Particle::Instance()->Spawn( Pos, 24 );
		Sound::Instance()->PlaySFX(E_SFX_POWER_UP);
		Kill();
		return true;
	}
	
	return false;
}
	
//------------------------------------------------------------------------------
//	BombUpFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
BombUpFactory::BombUpFactory()
{
}
	
BombUpFactory::~BombUpFactory()
{
	BombUps.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void BombUpFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	BombUps.resize( MAX_BOMB_UP );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void BombUpFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<BombUp>::iterator iter;
	for( iter = BombUps.begin(); iter != BombUps.end(); ++iter )
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
void BombUpFactory::DrawEntities( s32 z )
{
	
	std::vector<BombUp>::iterator iter;
	for( iter = BombUps.begin(); iter != BombUps.end(); ++iter )
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
void BombUpFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<BombUp>::iterator iter;
	for( iter = BombUps.begin(); iter != BombUps.end(); ++iter )
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
void BombUpFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<BombUp>::iterator iter;
	for( iter = BombUps.begin(); iter != BombUps.end(); ++iter )
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
void BombUpFactory::Spawn( s32 x, s32 y )
{
	
	std::vector<BombUp>::iterator iter;
	for( iter = BombUps.begin(); iter != BombUps.end(); ++iter )
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
void BombUpFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	
}

//--------------------------------------
//	
//--------------------------------------
void BombUpFactory::KillAllEntities()
{
	std::vector<BombUp>::iterator iter;
	for( iter = BombUps.begin(); iter != BombUps.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}




