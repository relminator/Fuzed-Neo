/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Jumper.h"

#define MAX_JUMPERS 16

Jumper::Jumper()
{
	
	Active = false;	
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 1;
	Speed = floattof32(0.03); 

	Width = (24 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (24 * TILE_SIZE_3D) / TILE_SIZE;   
	
	JumpHeight = JUMP_HEIGHT;
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 46;
	MaxFrame = 6;
	Score = 43;
}

Jumper::~Jumper()
{
}


void Jumper::Update( Player &Mario, Map2D &Map )
{
	Counter++;

	
	ResolveStates();
	if( !Stunned )
	{
		CollideOnMap( Map );
			
		xv = 0;
		//if( GetPlayerDistanceX( Mario ) > 5 )
		//{
		//	if( Mario.GetX() <  x )
		//		xv = -Speed;
		//	else
		//		xv = Speed;
		//}
		//else
		{
			xv = Speed;
		}
		
	}	
	if( (Counter & 3) == 0 )
	{
		Frame = ( Frame + 1 ) % MaxFrame;
	}
	
	SetTile( x + Width/2, y + Height/2, BLOCK_COLLIDE, Map );
	
	// aabb
	ResolveAABBS();
	
	
}


void Jumper::CollideOnMap( Map2D &Map )
{
	
	int TileX, TileY;
	
	if( IsOnWater( x + Width/2, y + Height/2, Map ) )
	{
		xv /= 2;
		if( yv > 3 ) yv /= 2;
	}
	
	
	if( xv > 0 )
	{
		Flipmode = GL_FLIP_NONE;
	
		if( CollideWalls( x + xv + Width, y, TileX, Map ) )
		{
			x = (TileX * TILE_SIZE_3D) - Width - ONE;
			Speed = -Speed;
		}
		else
		{
			x += xv;
		}
		
	}		
	else if( xv < 0 )
	{
		
		Flipmode = GL_FLIP_H;
		
		if( CollideWalls( x + xv, y, TileX, Map ) )
		{
			x = ( (TileX + 1 ) * TILE_SIZE_3D) + ONE;
			Speed = -Speed;
		}
		else
		{
			x += xv;
		}
		
	}
	
	if( yv < 0 )
	{
		
		if( CollideFloors( x, y + yv, TileY, Map ) )   // hit the roof
		{
			y = ( (TileY + 1) * TILE_SIZE_3D) + ONE ;
			yv = 0;
		}
		else
		{
			y += yv;
			yv += GRAVITY;
		}
		
	}		
	else
	{
		if( CollideFloors( x, y + yv + Height, TileY, Map ) )
		{
			y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
			yv = -JumpHeight;									// Jump again
			
		}
		else
		{
			y += yv;
			yv += GRAVITY;
			if( yv > FALL_SPEED_THRESHOLD ) yv = FALL_SPEED_THRESHOLD;
		}
	
	}


}



//------------------------------------------------------------------------------
//	JumperFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
JumperFactory::JumperFactory()
{
}
	
JumperFactory::~JumperFactory()
{
	Jumpers.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void JumperFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	Jumpers.resize( MAX_JUMPERS );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void JumperFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<Jumper>::iterator iter;
	for( iter = Jumpers.begin(); iter != Jumpers.end(); ++iter )
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
void JumperFactory::DrawEntities( s32 z )
{
	
	std::vector<Jumper>::iterator iter;
	for( iter = Jumpers.begin(); iter != Jumpers.end(); ++iter )
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
void JumperFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<Jumper>::iterator iter;
	for( iter = Jumpers.begin(); iter != Jumpers.end(); ++iter )
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
void JumperFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<Jumper>::iterator iter;
	for( iter = Jumpers.begin(); iter != Jumpers.end(); ++iter )
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
void JumperFactory::Spawn( s32 x, s32 y, int direction  )
{
	
	std::vector<Jumper>::iterator iter;
	for( iter = Jumpers.begin(); iter != Jumpers.end(); ++iter )
	{
	
		if( !iter->Active )		// insert on a free slot
		{
			iter->Init( x, y, direction );
			break;
		}
	
	}
	
}	

//--------------------------------------
//	
//--------------------------------------
void JumperFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	std::vector<Jumper>::iterator iter;
	for( iter = Jumpers.begin(); iter != Jumpers.end(); ++iter )
	{
		if( iter->Active )
		{
			for( int i = 0; i < MAX_BLOCKS; i++ )
			{
				Block B = Blocks.GetBlock(i);
				if( B.IsActive())
				{
					AABB aabb1 = iter->GetAabb();
					AABB aabb2 = B.GetAabb();
					if( aabb1.Intersects(aabb2))
					{
						if( B.GetDirection() != 0 )  // if not falling move idiots out of the way
						{
							if( !B.IsThrown() )
							{
								iter->x = B.GetCx() + B.GetWidth()/2 * B.GetDirection();
							}
							else
							{
								Vector2df32 Pos;
								Pos.x = iter->GetX() + iter->GetWidth()/2;
								Pos.y = iter->GetY() + iter->GetHeight()/2;
								Particle::Instance()->Spawn( Pos, 24 );
				
								Explosion::Instance()->Spawn( Pos.x,
															  Pos.y,
															  0,
															  0,
															  Explode::MEDIUM_4,
															  Explode::SMALL_2 );
			
								Sound::Instance()->PlaySFX(E_SFX_EXPLODE);
								Mario.AddToScore(iter->Score);
								iter->Kill();
							}
						}
						else	// Falling so keel the idiot
						{
							Vector2df32 Pos;
							Pos.x = iter->GetX() + iter->GetWidth()/2;
							Pos.y = iter->GetY() + iter->GetHeight()/2;
							Particle::Instance()->Spawn( Pos, 24 );
			
							Explosion::Instance()->Spawn( Pos.x,
														  Pos.y,
														  0,
														  0,
														  Explode::MEDIUM_4,
														  Explode::SMALL_2 );
							Sound::Instance()->PlaySFX(E_SFX_EXPLODE);
							Mario.AddToScore(iter->Score);
							iter->Kill();
						}
					}
				}
			}
	
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void JumperFactory::KillAllEntities()
{
	std::vector<Jumper>::iterator iter;
	for( iter = Jumpers.begin(); iter != Jumpers.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}


