/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Bouncer.h"

#define MAX_BOUNCERS 16

Bouncer::Bouncer()
{
	
	Active = false;	
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 1;
	Speed = floattof32(0.03); 

	Width = (31 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (31 * TILE_SIZE_3D) / TILE_SIZE;   
	
	JumpHeight = JUMP_HEIGHT;
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 19;
	MaxFrame = 4;
	Score = 44;
}

Bouncer::~Bouncer()
{
}


void Bouncer::Init( s32 sx, s32 sy, int direction )
{
	Active = true;
	Counter = 0;
	Speed *= direction;
	
	x = sx;
	y = sy;
	
	xv = 0; 
	yv = 1;
	
	JumpHeight = JUMP_HEIGHT;
	
	ResolveAABBS();
	
}


void Bouncer::CollideOnMap( Map2D &Map )
{
	
	int TileX, TileY;
	
	xv = 0;
	xv = Speed;
	
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
			JumpHeight -=  (JumpHeight/8);						// Dampen the height
			if( JumpHeight <= 10 ) JumpHeight = JUMP_HEIGHT;	// Reset height
			
		}
		else
		{
			y += yv;
			yv += GRAVITY;
			if( yv > FALL_SPEED_THRESHOLD ) yv = FALL_SPEED_THRESHOLD;
		}
	
	}


}


void Bouncer::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	ResolveStates();
	if( !Stunned )
	{
		CollideOnMap( Map );
	}	
	if( (Counter & 3) == 0 )
	{
		Frame = ( Frame + 1 ) % MaxFrame;
	}
	
	SetTile( x + Width/2, y + Height/2, BLOCK_COLLIDE, Map );
	
	// aabb
	ResolveAABBS();
	
}

void Bouncer::Draw( const glImage *spr, s32 z )
{
	glSprite3D( x, y, z, Flipmode, &spr[BaseFrame+Frame] );
}
	

void Bouncer::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
}


bool Bouncer::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	if( Mario.CollideShots( aabb ) )
	{
		Explosion::Instance()->Spawn( x + Width/2,
									  y + Height/2,
									  0,
									  0,
									  Explode::MEDIUM_1 );
		Sound::Instance()->PlaySFX(E_SFX_EXPLODE);
		Mario.AddToScore(Score);
		Kill();
		return true;
		
	}
	
	if( Mario.CollideBombs( aabb ) )
	{
		Explosion::Instance()->Spawn( x + Width/2,
									  y + Height/2,
									  0,
									  0,
									  Explode::MEDIUM_4 );
		Sound::Instance()->PlaySFX(E_SFX_EXPLODE);
		Mario.AddToScore(Score);
		Kill();
		return true;
	}
	
	if( Mario.CollideAttack( aabb ) && Mario.GetCanHit() )
	{
		int Tx = ( x + Width / 2 ) / TILE_SIZE_3D;
		int Ty = ( y + Height / 2 ) / TILE_SIZE_3D;		
		Map( Tx, Ty ) = NONE;      // set as 0 when free falling
		
		Vector2df32 Pos;
		Pos.x = x + Width/2;
		Pos.y = y + Height/2;
		Particle::Instance()->Spawn( Pos, 24 );
		
		Explosion::Instance()->Spawn( Pos.x,
									  Pos.y,
									  0,
									  0,
									  Explode::MEDIUM_4 );
	
			
		Sound::Instance()->PlaySFX(E_SFX_EXPLODE);
		Mario.AddToScore(Score);
		Kill();
		return true;
	}	
	
	
	if( !Mario.IsInvincible() )
	{
		if( aabb.Intersects( Mario.GetAABB() ) )
		{
			if( aabb2.Intersects( Mario.GetAABB2() ) )
			{
				Mario.SetInvincible(true);
				Mario.HitAnimation();
			}
			
			return true;
		}
	}
	
	
	
	return false;
}




//------------------------------------------------------------------------------
//	BouncerFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
BouncerFactory::BouncerFactory()
{
}
	
BouncerFactory::~BouncerFactory()
{
	Bouncers.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void BouncerFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	Bouncers.resize( MAX_BOUNCERS );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void BouncerFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	NumEntities = 0;
	std::vector<Bouncer>::iterator iter;
	for( iter = Bouncers.begin(); iter != Bouncers.end(); ++iter )
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
void BouncerFactory::DrawEntities( s32 z )
{
	
	std::vector<Bouncer>::iterator iter;
	for( iter = Bouncers.begin(); iter != Bouncers.end(); ++iter )
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
void BouncerFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<Bouncer>::iterator iter;
	for( iter = Bouncers.begin(); iter != Bouncers.end(); ++iter )
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
void BouncerFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<Bouncer>::iterator iter;
	for( iter = Bouncers.begin(); iter != Bouncers.end(); ++iter )
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
void BouncerFactory::Spawn( s32 x, s32 y, int direction  )
{
	
	std::vector<Bouncer>::iterator iter;
	for( iter = Bouncers.begin(); iter != Bouncers.end(); ++iter )
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
void BouncerFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	std::vector<Bouncer>::iterator iter;
	for( iter = Bouncers.begin(); iter != Bouncers.end(); ++iter )
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
								iter->Speed = -iter->Speed;
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
void BouncerFactory::KillAllEntities()
{
	std::vector<Bouncer>::iterator iter;
	for( iter = Bouncers.begin(); iter != Bouncers.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}



