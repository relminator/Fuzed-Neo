/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "GreenTurtle.h"

#define MAX_GREEN_TURTLES 16

GreenTurtle::GreenTurtle()
{
	
	Active = false;	
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 1;
	Speed = floattof32(0.03); 

	Width = (28 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (28 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 41;
	MaxFrame = 4;
	Score = 51;
}

GreenTurtle::~GreenTurtle()
{
}


void GreenTurtle::Init( s32 sx, s32 sy, int direction )
{
	Active = true;
	
	Speed *= direction;
	
	x = sx;
	y = sy;
	
	xv = 0; 
	yv = 1;
	
	ResolveAABBS();
	
}



void GreenTurtle::CollideOnMap( Map2D &Map )
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
	
	if( yv >= 0 )
	{
		if( CollideFloors( x, y + yv + Height, TileY, Map ) )
		{
			y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
			yv = ONE;
		}
		else
		{
			y += yv;
			yv += GRAVITY;
			if( yv > FALL_SPEED_THRESHOLD ) yv = FALL_SPEED_THRESHOLD;
		}
	
	}


}


void GreenTurtle::Update( Player &Mario, Map2D &Map )
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
	
	SetTile( x + (Width/2), y + Height/2, BLOCK_COLLIDE, Map );
	
	// aabb
	ResolveAABBS();
	
	
}

void GreenTurtle::Draw( const glImage *spr, s32 z )
{
	glSprite3D( x, y, z, Flipmode, &spr[BaseFrame+Frame] );
}
	

void GreenTurtle::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
}


bool GreenTurtle::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	if( Mario.CollideShots( aabb ) )
	{
		Explosion::Instance()->Spawn( x + Width/2,
									  y + Height/2,
									  0,
									  0,
									  Explode::MEDIUM_2 );
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
			s32 cy = y + Height / 2;
			
			if( Mario.GetCY() < cy )
			{
				Sound::Instance()->PlaySFX(E_SFX_BOUNCE);
				Mario.SetYV(-JUMP_HEIGHT);
			}
			
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
//	GreenTurtleFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
GreenTurtleFactory::GreenTurtleFactory()
{
}
	
GreenTurtleFactory::~GreenTurtleFactory()
{
	GreenTurtles.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void GreenTurtleFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	GreenTurtles.resize( MAX_GREEN_TURTLES );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void GreenTurtleFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<GreenTurtle>::iterator iter;
	for( iter = GreenTurtles.begin(); iter != GreenTurtles.end(); ++iter )
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
void GreenTurtleFactory::DrawEntities( s32 z )
{
	
	std::vector<GreenTurtle>::iterator iter;
	for( iter = GreenTurtles.begin(); iter != GreenTurtles.end(); ++iter )
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
void GreenTurtleFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<GreenTurtle>::iterator iter;
	for( iter = GreenTurtles.begin(); iter != GreenTurtles.end(); ++iter )
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
void GreenTurtleFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<GreenTurtle>::iterator iter;
	for( iter = GreenTurtles.begin(); iter != GreenTurtles.end(); ++iter )
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
void GreenTurtleFactory::Spawn( s32 x, s32 y, int direction  )
{
	
	std::vector<GreenTurtle>::iterator iter;
	for( iter = GreenTurtles.begin(); iter != GreenTurtles.end(); ++iter )
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
void GreenTurtleFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	std::vector<GreenTurtle>::iterator iter;
	for( iter = GreenTurtles.begin(); iter != GreenTurtles.end(); ++iter )
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
void GreenTurtleFactory::KillAllEntities()
{
	std::vector<GreenTurtle>::iterator iter;
	for( iter = GreenTurtles.begin(); iter != GreenTurtles.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}


