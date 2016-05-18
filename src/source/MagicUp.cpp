/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "MagicUp.h"

#define MAX_MAGIC_UP 8


MagicUp::MagicUp()
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
	BaseFrame = 33;
	MaxFrame = 8;
	
	Stunned = false;
	Score = 150;
}

MagicUp::~MagicUp()
{
}


void MagicUp::Init( s32 sx, s32 sy )
{
	Active = true;
	
	x = sx;
	y = sy;
	
		
}

void MagicUp::Update( Player &Mario, Map2D &Map )
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

void MagicUp::Draw( const glImage *spr, s32 z )
{
	int Scale = (1 << 12) + abs(sinLerp( Counter * 512 ))/2;
	glSpriteRotateScale3D( x + Width/2, y + Height/2, z, 0, Scale, Scale, Flipmode, &spr[BaseFrame+Frame] );
}
	

void MagicUp::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
}



bool MagicUp::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	if( aabb2.Intersects( Mario.GetAABB2() ) )
	{
		Mario.IncShotsLeft();
		Vector2df32 Pos;
		Pos.x = x + Width/2;
		Pos.y = y + Height/2;
		Particle::Instance()->Spawn( Pos, 24 );
		Sound::Instance()->PlaySFX(E_SFX_POWER_UP);
		Mario.AddToScore(Score);
		Kill();
		return true;
	}
	
	return false;
}
	
//------------------------------------------------------------------------------
//	MagicUpFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
MagicUpFactory::MagicUpFactory()
{
}
	
MagicUpFactory::~MagicUpFactory()
{
	MagicUps.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void MagicUpFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	MagicUps.resize( MAX_MAGIC_UP );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void MagicUpFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<MagicUp>::iterator iter;
	for( iter = MagicUps.begin(); iter != MagicUps.end(); ++iter )
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
void MagicUpFactory::DrawEntities( s32 z )
{
	
	std::vector<MagicUp>::iterator iter;
	for( iter = MagicUps.begin(); iter != MagicUps.end(); ++iter )
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
void MagicUpFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<MagicUp>::iterator iter;
	for( iter = MagicUps.begin(); iter != MagicUps.end(); ++iter )
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
void MagicUpFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<MagicUp>::iterator iter;
	for( iter = MagicUps.begin(); iter != MagicUps.end(); ++iter )
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
void MagicUpFactory::Spawn( s32 x, s32 y )
{
	
	std::vector<MagicUp>::iterator iter;
	for( iter = MagicUps.begin(); iter != MagicUps.end(); ++iter )
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
void MagicUpFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	
}

//--------------------------------------
//	
//--------------------------------------
void MagicUpFactory::KillAllEntities()
{
	std::vector<MagicUp>::iterator iter;
	for( iter = MagicUps.begin(); iter != MagicUps.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}




