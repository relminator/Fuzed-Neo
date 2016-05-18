/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "RedTurtle.h"

#define MAX_RED_TURTLES 16

RedTurtle::RedTurtle()
{
	
	Active = false;	
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 1;
	Speed = floattof32(0.03); 
	
	Width = (20 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (30 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 25;
	MaxFrame = 8;
	Score = 76;
	
}

RedTurtle::~RedTurtle()
{
}


void RedTurtle::Init( s32 sx, s32 sy, int direction )
{
	Active = true;
	
	Speed *= direction;
	
	x = sx + Width / 4;   // +xv so offset right to fix sticky bug when spawning on edges
	y = sy;
	
	xv = 0; 
	yv = 1;
	
	ResolveAABBS();
	
	
}


bool RedTurtle::CollideFloors( s32 ix, s32 iy, int &TileY, Map2D &Map )
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

void RedTurtle::CollideOnMap( Map2D &Map )
{
	
	int TileX, TileY;
	
	xv = 0;
	
	if( IsOnWater( x + Width/2, y + Height/2, Map ) )
	{
		if( yv > 3 ) yv /= 2;
	}
	
	
	if( yv >= 0 )
	{
		if( CollideFloors( x, y + yv + Height, TileY, Map ) )
		{
			y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
			yv = ONE;
			xv = Speed;
		}
		else
		{
			y += yv;
			yv += GRAVITY;
			if( yv > FALL_SPEED_THRESHOLD ) yv = FALL_SPEED_THRESHOLD;
		}
	
	}

	if( IsOnWater( x + Width/2, y + Height/2, Map ) )
	{
		xv /= 2;
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
			if( Map.Collide( x + Width + xv, y + Height + Height/2 ) == NONE )
			{
				Speed = -Speed;
				xv = Speed;
			}
	
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
			if( Map.Collide( x + xv, y + Height + Height/2 ) == NONE )
			{
				Speed = -Speed;
				xv = Speed;
			}
	
			x += xv;
		}
		
	}
	

}


void RedTurtle::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	ResolveStates();
	if( !Stunned )
	{
		Speed = floattof32(0.03) * UTIL::Sign( Speed );
			
		if( SeePlayerHorizontal( Mario, Map, 5 ) )
		{
			if( (Mario.GetX() - x ) < 0 ) // Enemy right
			{
				if( xv < 0 )
				{
					Speed = -floattof32(0.10);
				}
					
			}
			else
			{
				if( xv > 0 )
				{
					Speed = floattof32(0.10);
				}
				
			}
		}
		
		CollideOnMap( Map );
		
	}
	
	if( (Counter & 3) == 0 )
	{
		Frame = ( Frame + 1 ) % MaxFrame;
	}

	// BLOCK_COLLIDE only collides with blocks and not with player
	SetTile( x + Width/2, y + Height/2, BLOCK_COLLIDE, Map );
	// aabb
	ResolveAABBS();
	
	
	
}

void RedTurtle::Draw( const glImage *spr, s32 z )
{
	glSprite3D( x - ((6 * TILE_SIZE_3D) / TILE_SIZE), y, z, Flipmode, &spr[BaseFrame+Frame] );
}
	

void RedTurtle::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(31,31,31) );
}

bool RedTurtle::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	
	if( Mario.CollideShots( aabb ) )
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

	if( Mario.CollideAttack( aabb )  && Mario.GetCanHit() )
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
				return true;
			}
			
				
			s32 cy = y + Height / 4;
			
			if( (Mario.GetY() + Mario.GetHeight()) < cy )   // only step on top if player foot is above enemy center
			{
				Mario.SetY((y - Mario.GetHeight())+1);	// Snap to head offset a little down since our sprite is smaller than aabb height
				Mario.SetYV(0);						// Don't bounce
				Mario.SetXV(xv);					// Follow
				Mario.SetJumping(false);			// Can jump from head
				Mario.SetFalling(false);			// So that animation resets
				Mario.SetClimbing(false);
				//if( !( Mario.GetKeyH() & KEY_A ) ) Mario.SetCanJump(true);
				Mario.SetCanJump(true);
				Mario.CollideOnMap( Map ); 			// Do some collision
			}
			
			
			return true;
		}
		
	}
	
	
		

	return false;
}



//------------------------------------------------------------------------------
//	RedTurtleFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
RedTurtleFactory::RedTurtleFactory()
{
}
	
RedTurtleFactory::~RedTurtleFactory()
{
	RedTurtles.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void RedTurtleFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	RedTurtles.resize( MAX_RED_TURTLES );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void RedTurtleFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	NumEntities = 0;
	std::vector<RedTurtle>::iterator iter;
	for( iter = RedTurtles.begin(); iter != RedTurtles.end(); ++iter )
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
void RedTurtleFactory::DrawEntities( s32 z )
{
	
	std::vector<RedTurtle>::iterator iter;
	for( iter = RedTurtles.begin(); iter != RedTurtles.end(); ++iter )
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
void RedTurtleFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<RedTurtle>::iterator iter;
	for( iter = RedTurtles.begin(); iter != RedTurtles.end(); ++iter )
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
void RedTurtleFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<RedTurtle>::iterator iter;
	for( iter = RedTurtles.begin(); iter != RedTurtles.end(); ++iter )
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
void RedTurtleFactory::Spawn( s32 x, s32 y, int direction  )
{
	
	std::vector<RedTurtle>::iterator iter;
	for( iter = RedTurtles.begin(); iter != RedTurtles.end(); ++iter )
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
void RedTurtleFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	std::vector<RedTurtle>::iterator iter;
	for( iter = RedTurtles.begin(); iter != RedTurtles.end(); ++iter )
	{
		if( iter->Active )
		{
			for( int i = 0; i < MAX_BLOCKS; i++ )
			{
				Block B = Blocks.GetBlock(i);
				if( B.IsActive() )
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
void RedTurtleFactory::KillAllEntities()
{
	std::vector<RedTurtle>::iterator iter;
	for( iter = RedTurtles.begin(); iter != RedTurtles.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}


