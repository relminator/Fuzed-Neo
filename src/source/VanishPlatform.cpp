/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "VanishPlatform.h"

#define MAX_VANISH_COUNTER  60

VanishPlatform::VanishPlatform()
{
	Active = false;	
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 0;
	Speed = floattof32(0); 
	
	Width = (31 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (31 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 0;
	MaxFrame = 1;
	
	Vanishing = false;
	VanishCounter = MAX_VANISH_COUNTER;
}

VanishPlatform::~VanishPlatform()
{
}


void VanishPlatform::Init( s32 sx, s32 sy )
{
	Active = true;
	
	
	x = sx;  
	y = sy;
	
	Vanishing = false;
	VanishCounter = MAX_VANISH_COUNTER;
	
}





void VanishPlatform::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	if( Vanishing )
	{
		VanishCounter--;
	}
	if( VanishCounter < 0 )
	{
		SetTile( x + Width/2, y + Height/2, NONE, Map );
		Kill();
		return;
	}
	else
	{
		SetTile( x + Width/2, y + Height/2, INVISIBLE, Map );
	}
	// aabb
	ResolveAABBS();
	aabb.Init( x, y - Height/32, Width, Height );
	
}

void VanishPlatform::Draw( const glImage *spr, s32 z )
{
	if( !Vanishing )
	{
		glColor( RGB15(0,31,0));
		DrawCube( x + floattof32(0.5), y + floattof32(0.5), 0, &spr[BaseFrame+Frame] );			
		glColor( RGB15(31,31,31));
	}					
	else
	{
		int Fade = VanishCounter/2;
		if( Fade < 1 ) Fade = 1;
		glPolyFmt( POLY_ALPHA(Fade) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
		glColor( RGB15(0,31,0));
		DrawCube( x + floattof32(0.5), y + floattof32(0.5), 0, &spr[BaseFrame+Frame] );			
		glColor( RGB15(31,31,31));
		glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
	}
	
}
	

void VanishPlatform::DrawAABB( s32 z, int Color )
{
	z += TILE_SIZE_3D/2;
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
}

bool VanishPlatform::CollideWithPlayer( Player &Mario, Map2D &Map )
{

	if( aabb.Intersects( Mario.GetAABB() ) )
	{
		
			
		s32 cy = y + Height / 2;
		
		if( (Mario.GetY() + Mario.GetHeight()) < cy )   // only step on top if player foot is above enemy center
		{
			if( !Vanishing ) Sound::Instance()->PlaySFX(E_SFX_WATER_SPLASH);
			Vanishing = true;
			return true;
		}
		
	}
				
		
	return false;
}



//------------------------------------------------------------------------------
//	VanishPlatformFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
VanishPlatformFactory::VanishPlatformFactory()
{
}
	
VanishPlatformFactory::~VanishPlatformFactory()
{
	VanishPlatforms.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void VanishPlatformFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	VanishPlatforms.resize( MAX_VANISH_PLATFORMS );			// speed of iteration)
}


//--------------------------------------
//	
//--------------------------------------
const VanishPlatform VanishPlatformFactory::GetVanishPlatform( int i ) const 
{
	VanishPlatform B;
	B.SetActive(false);
	if( i < 0 ) return B;
	if( i >= MAX_VANISH_PLATFORMS ) return B;
	if( VanishPlatforms[i].IsActive() ) 
		return VanishPlatforms[i];
	
	return B;
}
	

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void VanishPlatformFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<VanishPlatform>::iterator iter;
	for( iter = VanishPlatforms.begin(); iter != VanishPlatforms.end(); ++iter )
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
void VanishPlatformFactory::DrawEntities( s32 z )
{
	
	std::vector<VanishPlatform>::iterator iter;
	for( iter = VanishPlatforms.begin(); iter != VanishPlatforms.end(); ++iter )
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
void VanishPlatformFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<VanishPlatform>::iterator iter;
	for( iter = VanishPlatforms.begin(); iter != VanishPlatforms.end(); ++iter )
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
void VanishPlatformFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<VanishPlatform>::iterator iter;
	for( iter = VanishPlatforms.begin(); iter != VanishPlatforms.end(); ++iter )
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
void VanishPlatformFactory::Spawn( s32 x, s32 y )
{
	
	std::vector<VanishPlatform>::iterator iter;
	for( iter = VanishPlatforms.begin(); iter != VanishPlatforms.end(); ++iter )
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
void VanishPlatformFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	
}

//--------------------------------------
//	
//--------------------------------------
void VanishPlatformFactory::KillAllEntities()
{
	std::vector<VanishPlatform>::iterator iter;
	for( iter = VanishPlatforms.begin(); iter != VanishPlatforms.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}



