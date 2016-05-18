/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Spiral.h"

#define MAX_SPIRALS 16


Spiral::Spiral()
{
	
	Active = false;	
	Axis = 0;
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 0;
	
	Direction = -1;
	Rotation = 0;
	
	Speed = 0; 
	Accel = ACCEL * 20;
	
	Width = (32 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (32 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 23;
	MaxFrame = 2;
	
	Stunned = false;
	
	Score = 67;
	
}

Spiral::~Spiral()
{
}


void Spiral::Init( s32 sx, s32 sy, int axis )
{
	Active = true;
	
	x = sx;
	y = sy;
	
	Axis = axis;
	
	Direction = -1;
	Speed = 0;
	
	if( Axis == 0 )   // bounce on Yaxis
	{
		xv = 0;
		yv = 0;
	}
	else
	{
		xv = 0;
		yv = 0;
	}
	
	ResolveAABBS();
	
}

void Spiral::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	ResolveStates();
	
	//Rotation += Speed;
	Rotation += 128;
	
	if( Axis == 0 )
	{
		Speed += ( Accel * Y_FRICTION ) >> 12;
		if( IsOnWater( x + Width/2, y + Height/2, Map ) )
		{
			Speed *= 3;
			Speed /= 4;
		}
		
		if( Stunned )
		{
			Speed = 0;
			Rotation -= 128;
		}

		yv = Speed * Direction;
		y += yv;
		Speed -= ( Speed * Y_FRICTION ) >> 12;
	
		s32 iy;
		if( yv < 0 )
			iy = y;
		else
			iy = y + TILE_SIZE_3D;
		
		if( Map.Collide( x, iy + yv ) >= BRICK )
		{
			Direction= -Direction;
			Speed = 0;
			Rotation -= 128;			
		}
		
		
	}
	else
	{
		
		Speed += ( Accel * X_FRICTION ) >> 12;
		if( IsOnWater( x + Width/2, y + Height/2, Map ) )
		{
			Speed *= 3;
			Speed /= 4;
		}	
		
		if( Stunned )
		{
			Speed = 0;
			Rotation -= 128;			
		}
	
		xv = Speed * Direction;
		x += xv;
		Speed -= ( Speed * X_FRICTION ) >> 12;
	
		
			
		s32 ix;
		if( xv < 0 )
			ix = x;
		else
			ix = x + TILE_SIZE_3D;
		
		if( Map.Collide( ix + xv, y ) >= BRICK )
		{
			Direction= -Direction;
			Speed = 0;
			Rotation -= 128;
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

void Spiral::Draw( const glImage *spr, s32 z )
{
	glSpriteRotate3D( x + Width/2, y + Height/2, z, Rotation * 5 , Flipmode, &spr[BaseFrame+Frame] );
}
	

void Spiral::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
}



bool Spiral::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	if( Mario.CollideShots( aabb ) )
	{
		Explosion::Instance()->Spawn( x + Width/2,
									  y + Height/2,
									  0,
									  0,
									  Explode::MEDIUM_5 );
		Sound::Instance()->PlaySFX(E_SFX_EXPLODE);
		Mario.AddToScore(Score);
		Kill();
		return true;
		
	}
	
	if( !Mario.IsInvincible() )
	{
		if( aabb2.Intersects( Mario.GetAABB2() ) )
		{
			Sound::Instance()->PlaySFX(E_SFX_HURT);
			Mario.SetInvincible(true);
			Mario.HitAnimation();
			return true;
		}
	}
	
	if( Mario.CollideAttack( aabb ) )
	{
		if( !Stunned ) Sound::Instance()->PlaySFX(E_SFX_ATTACK_HIT);
		StunMe( 20 );
	}	
	
	return false;
}
	
//------------------------------------------------------------------------------
//	SpiralFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
SpiralFactory::SpiralFactory()
{
}
	
SpiralFactory::~SpiralFactory()
{
	Spirals.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void SpiralFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	Spirals.resize( MAX_SPIRALS );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void SpiralFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<Spiral>::iterator iter;
	for( iter = Spirals.begin(); iter != Spirals.end(); ++iter )
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
void SpiralFactory::DrawEntities( s32 z )
{
	
	std::vector<Spiral>::iterator iter;
	for( iter = Spirals.begin(); iter != Spirals.end(); ++iter )
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
void SpiralFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<Spiral>::iterator iter;
	for( iter = Spirals.begin(); iter != Spirals.end(); ++iter )
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
void SpiralFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<Spiral>::iterator iter;
	for( iter = Spirals.begin(); iter != Spirals.end(); ++iter )
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
void SpiralFactory::Spawn( s32 x, s32 y, int axis )
{
	
	std::vector<Spiral>::iterator iter;
	for( iter = Spirals.begin(); iter != Spirals.end(); ++iter )
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
void SpiralFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	std::vector<Spiral>::iterator iter;
	for( iter = Spirals.begin(); iter != Spirals.end(); ++iter )
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
void SpiralFactory::KillAllEntities()
{
	std::vector<Spiral>::iterator iter;
	for( iter = Spirals.begin(); iter != Spirals.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}




