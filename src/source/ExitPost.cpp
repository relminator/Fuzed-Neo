/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "ExitPost.h"

#define MAX_EXIT_POST 1


ExitPost::ExitPost()
{
	
	Active = false;	

	x = 0;
	y = 0; 
	xv = 0; 
	yv = 0;
	
	
	Width = (46 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (52 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 14;
	MaxFrame = 1;
	
	Stunned = false;
	
}

ExitPost::~ExitPost()
{
}


void ExitPost::Init( s32 sx, s32 sy )
{
	Active = true;
	
	x = sx;
	y = sy;
	
}

void ExitPost::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	ResolveStates();
	
	
	//SetTile( x + Width/2, y + Height/2, BLOCK_COLLIDE, Map );
	
	// aabb
	ResolveAABBS();
	
	
}

void ExitPost::Draw( const glImage *spr, s32 z )
{
	
	glSprite3D( x , (y - Height/2) + Height/8 , z - TILE_SIZE_3D / 8, Flipmode, &spr[BaseFrame+Frame] );
}
	

void ExitPost::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
}



bool ExitPost::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	Mario.SetInExitPost(false);
	
	if( aabb2.Intersects( Mario.GetAABB2() ) )
	{
		Mario.SetInExitPost(true);
	
		return true;
	}
	
	return false;
}
	
//------------------------------------------------------------------------------
//	ExitPostFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
ExitPostFactory::ExitPostFactory()
{
	Open = false;
}
	
ExitPostFactory::~ExitPostFactory()
{
	ExitPosts.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void ExitPostFactory::Init( glImage* const Sprites )
{
	Open = false;
	Images = Sprites;
	ExitPosts.resize( MAX_EXIT_POST );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void ExitPostFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<ExitPost>::iterator iter;
	for( iter = ExitPosts.begin(); iter != ExitPosts.end(); ++iter )
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
void ExitPostFactory::DrawEntities( s32 z )
{
	static int Counter = 0;
	Counter++;
	int color = 16 + (abs(sinLerp(Counter * 512) * 15 ) >> 12);
	
	std::vector<ExitPost>::iterator iter;
	for( iter = ExitPosts.begin(); iter != ExitPosts.end(); ++iter )
	{
		if( iter->Active )
		{
			if( Open )
			{
				glColor( RGB15( color, color, color) );
				iter->Draw( Images, z );
			}
			else
			{
				glColor( RGB15(31,31,31));
				iter->Draw( Images, z );
			}
			z -= 4;
		}
	}
	
	glColor( RGB15(31,31,31));
	
}

//--------------------------------------
//	
//--------------------------------------
void ExitPostFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<ExitPost>::iterator iter;
	for( iter = ExitPosts.begin(); iter != ExitPosts.end(); ++iter )
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
void ExitPostFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<ExitPost>::iterator iter;
	for( iter = ExitPosts.begin(); iter != ExitPosts.end(); ++iter )
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
void ExitPostFactory::Spawn( s32 x, s32 y )
{
	
	std::vector<ExitPost>::iterator iter;
	for( iter = ExitPosts.begin(); iter != ExitPosts.end(); ++iter )
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
void ExitPostFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	
}

//--------------------------------------
//	
//--------------------------------------
void ExitPostFactory::KillAllEntities()
{
	std::vector<ExitPost>::iterator iter;
	for( iter = ExitPosts.begin(); iter != ExitPosts.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}




