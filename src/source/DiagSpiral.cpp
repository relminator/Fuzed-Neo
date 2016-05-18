/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "DiagSpiral.h"

#define MAX_DIAGSPIRALS 16


DiagSpiral::DiagSpiral()
{
	
	Active = false;	
	Axis = 0;
	
	x = 0;
	y = 0; 
	
	Direction = -1;
	Rotation = 0;
	
	Speed = 0; 
	Accel = ACCEL * 10;
	
	Width = (32 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (32 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 23;
	MaxFrame = 2;
	
	yv = Speed;
	xv = Speed;
	Score = 75;
}

DiagSpiral::~DiagSpiral()
{
}


void DiagSpiral::Init( s32 sx, s32 sy, int axis )
{
	Active = true;
	
	x = sx;
	y = sy;
	
	Axis = axis;
	
	Direction = -1;
	Speed = 0;
	
	switch( axis )
	{
		case 0:
			yv = Speed;
			xv = Speed;
			sxv = 1;
			syv = 1;	
			break;
		case 1:
			yv = -Speed;
			xv = -Speed;
			sxv = -1;
			syv = -1;	
			break;
		case 2:
			yv = -Speed;
			xv = Speed;
			sxv = -1;
			syv = 1;	
			break;
		case 3:
			yv = Speed;
			xv = -Speed;
			sxv = 1;
			syv = -1;	
			break;
		default:
			yv = Speed;
			xv = Speed;
			sxv = 1;
			syv = 1;	
			
	}
	
	ResolveAABBS();
	
	
}

void DiagSpiral::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	ResolveStates();
		
	Rotation += Speed;
	
	Speed += ( Accel * Y_FRICTION ) >> 12;
	
	if( IsOnWater( x + Width/2, y + Height/2, Map ) )
	{
		Speed *= 7;
		Speed /= 8;
	}
	
	if( Stunned ) Speed = 0;
	
	
	yv = Speed * syv;
	xv = Speed * sxv;
	
	
	y += yv;
	x += xv;
		
	
	Speed -= ( Speed * Y_FRICTION ) >> 12;

	
	s32 ix = x + Width/2;
	s32 iy = y + Height/2;
	s32 offset = UTIL::Sign(xv) * Width/4;
	
	if( GetTile( ix + xv + offset, iy, Map ) >= BRICK )
	{
		sxv= -sxv;
		Speed = 0;
		Rotation = 0;
		x -= xv;
	}
	
	offset = UTIL::Sign(yv) * Height/4;
	if( GetTile( ix, iy + yv + offset, Map ) >= BRICK )
	{
		syv= -syv;
		Speed = 0;
		Rotation = 0;
		y -= yv;
	}
	
	
	if( (Counter & 3) == 0 )
	{
		Frame = ( Frame + 1 ) % MaxFrame;
	}
	
	SetTile( x + Width/2, y + Height/2, BLOCK_COLLIDE, Map );
	
	// aabb
	ResolveAABBS();
	
	
}


//------------------------------------------------------------------------------
//	DiagSpiralFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
DiagSpiralFactory::DiagSpiralFactory()
{
}
	
DiagSpiralFactory::~DiagSpiralFactory()
{
	DiagSpirals.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void DiagSpiralFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	DiagSpirals.resize( MAX_DIAGSPIRALS );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void DiagSpiralFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<DiagSpiral>::iterator iter;
	for( iter = DiagSpirals.begin(); iter != DiagSpirals.end(); ++iter )
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
void DiagSpiralFactory::DrawEntities( s32 z )
{
	
	std::vector<DiagSpiral>::iterator iter;
	for( iter = DiagSpirals.begin(); iter != DiagSpirals.end(); ++iter )
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
void DiagSpiralFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<DiagSpiral>::iterator iter;
	for( iter = DiagSpirals.begin(); iter != DiagSpirals.end(); ++iter )
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
void DiagSpiralFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<DiagSpiral>::iterator iter;
	for( iter = DiagSpirals.begin(); iter != DiagSpirals.end(); ++iter )
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
void DiagSpiralFactory::Spawn( s32 x, s32 y, int axis )
{
	
	std::vector<DiagSpiral>::iterator iter;
	for( iter = DiagSpirals.begin(); iter != DiagSpirals.end(); ++iter )
	{
	
		if( !iter->Active )		// insert on a free slot
		{
			iter->Init( x, y, axis );
			break;
		}
	
	}
	
}	

//--------------------------------------
//	
//--------------------------------------
void DiagSpiralFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	std::vector<DiagSpiral>::iterator iter;
	for( iter = DiagSpirals.begin(); iter != DiagSpirals.end(); ++iter )
	{
		if( iter->Active )
		{
			for( int i = 0; i < MAX_BLOCKS; i++ )
			{
				Block B = Blocks.GetBlock(i);
				if( B.IsActive())
				{
					AABB aabb1 = iter->GetAabb();
					AABB aabb2 = B.GetAabb2();
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
							if( ( iter->GetY() - B.GetY() ) > ( TILE_SIZE_3D / 2 ) )
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
	
}

//--------------------------------------
//	
//--------------------------------------
void DiagSpiralFactory::KillAllEntities()
{
	std::vector<DiagSpiral>::iterator iter;
	for( iter = DiagSpirals.begin(); iter != DiagSpirals.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}


