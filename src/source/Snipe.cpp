/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Snipe.h"

#define MAX_SNIPES 16

Snipe::Snipe()
{
	
	Active = false;	
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 1;
	Speed = 0; 
	
	Width = (20 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (28 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 63;
	MaxFrame = 6;
	
	Direction = RIGHT;
	Score = 200;
	
}

Snipe::~Snipe()
{
}


void Snipe::Init( s32 sx, s32 sy, int direction )
{
	Active = true;
	
	Speed *= direction;
	
	x = sx + Width / 4;   // +xv so offset right to fix sticky bug when spawning on edges
	y = sy;
	
}


bool Snipe::CollideFloors( s32 ix, s32 iy, int &TileY, Map2D &Map )
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

void Snipe::CollideOnMap( Map2D &Map )
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


void Snipe::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	ResolveStates();
	if( !Stunned )
	{
		//Speed = floattof32(0.03) * UTIL::Sign( Speed );
			
		if( SeePlayerHorizontal( Mario, Map, 5 ) )
		{
			if( (Mario.GetX() - x ) < 0 ) // Enemy right
			{
					Direction = LEFT;
					Flipmode = GL_FLIP_H;
			}
			else
			{
					Direction = RIGHT;
					Flipmode = GL_FLIP_NONE;
			}
		}
		
		CollideOnMap( Map );
		
	}
	
	if( (Counter & 7) == 0 )
	{
		Frame = ( Frame + 1 ) % MaxFrame;
	}

	// BLOCK_COLLIDE only collides with blocks and not with player
	SetTile( x + Width/2, y + Height/2, BLOCK_COLLIDE, Map );
	// aabb
	ResolveAABBS();
	
	
	
}

void Snipe::Draw( const glImage *spr, s32 z )
{
	glSprite3D( x, y, z, Flipmode, &spr[BaseFrame+Frame] );
}
	

void Snipe::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(31,31,31) );
}

bool Snipe::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	
	
	
	if( aabb.Intersects( Mario.GetAABB() ) )
	{
		
		
		if( aabb2.Intersects( Mario.GetAABB2() ) )
		{
			Vector2df32 Pos;
			Pos.x = x + Width/2;
			Pos.y = y + Height/2;
			Particle::Instance()->Spawn( Pos, 24 );
			Sound::Instance()->PlaySFX(E_SFX_SNIPE_UP);
			Mario.AddToScore(Score);
			Kill();
			return true;
		}
		
			
		return true;
	}
	
	
		

	return false;
}



//------------------------------------------------------------------------------
//	SnipeFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
SnipeFactory::SnipeFactory()
{
	
	SnipeKilled = false;
	AllRescued = false;
}
	
SnipeFactory::~SnipeFactory()
{
	Snipes.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void SnipeFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	Snipes.resize( MAX_SNIPES );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void SnipeFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	NumEntities = 0;
	std::vector<Snipe>::iterator iter;
	for( iter = Snipes.begin(); iter != Snipes.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->Update( Mario, Map );
			NumEntities++;
		}
	}
	
	if( NumEntities == 0 )
	{
		if( !AllRescued )
		{
			Sound::Instance()->PlaySFX(E_SFX_SNIPE_COMPLETE);
			AllRescued = true;
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void SnipeFactory::DrawEntities( s32 z )
{
	
	std::vector<Snipe>::iterator iter;
	for( iter = Snipes.begin(); iter != Snipes.end(); ++iter )
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
void SnipeFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<Snipe>::iterator iter;
	for( iter = Snipes.begin(); iter != Snipes.end(); ++iter )
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
void SnipeFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<Snipe>::iterator iter;
	for( iter = Snipes.begin(); iter != Snipes.end(); ++iter )
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
void SnipeFactory::Spawn( s32 x, s32 y, int direction  )
{
	
	std::vector<Snipe>::iterator iter;
	for( iter = Snipes.begin(); iter != Snipes.end(); ++iter )
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
void SnipeFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	std::vector<Snipe>::iterator iter;
	for( iter = Snipes.begin(); iter != Snipes.end(); ++iter )
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
								SnipeKilled = true;
								Mario.Die();
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
							SnipeKilled = true;
							Mario.Die();
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
void SnipeFactory::KillAllEntities()
{
	std::vector<Snipe>::iterator iter;
	for( iter = Snipes.begin(); iter != Snipes.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}


