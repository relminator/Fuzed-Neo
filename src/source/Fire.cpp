/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Fire.h"

#define MAX_FIRES 24

Fire::Fire()
{
	
	Active = false;	
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 1;
	Speed = 0; 

	Width = (31 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (24 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 15;
	MaxFrame = 4;
	Score = 71;
}

Fire::~Fire()
{
}


void Fire::Init( s32 sx, s32 sy, int direction )
{
	Active = true;
	
	Speed *= direction;
	
	x = sx;
	y = sy;
	
	Frame = UTIL::RND() & 3;
	
	xv = 0; 
	yv = 1;
	
	ResolveAABBS();
	
}



void Fire::CollideOnMap( Map2D &Map )
{
	
	int TileY;
	
	
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


void Fire::Update( Player &Mario, Map2D &Map )
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
	
	aabb2.Init( x, y + Height/2, Width, Height/2 );
	
	
}

void Fire::Draw( const glImage *spr, s32 z )
{
	glSprite3D( x, y, z, Flipmode, &spr[BaseFrame+Frame] );
	glSprite3D( x + (10 * TILE_SIZE_3D)/TILE_SIZE, y, z, Flipmode, &spr[BaseFrame+(3-Frame)] );
}
	

void Fire::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
}


bool Fire::CollideWithPlayer( Player &Mario, Map2D &Map )
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
	
	
	if( !Mario.IsInvincible() )
	{
			if( aabb2.Intersects( Mario.GetAABB2() ) )
			{
				Mario.SetInvincible(true);
				Mario.HitAnimation();
			}
			
			return true;
	}
	
	
	
	return false;
}




//------------------------------------------------------------------------------
//	FireFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
FireFactory::FireFactory()
{
}
	
FireFactory::~FireFactory()
{
	Fires.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void FireFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	Fires.resize( MAX_FIRES );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void FireFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<Fire>::iterator iter;
	for( iter = Fires.begin(); iter != Fires.end(); ++iter )
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
void FireFactory::DrawEntities( s32 z )
{
	
	std::vector<Fire>::iterator iter;
	for( iter = Fires.begin(); iter != Fires.end(); ++iter )
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
void FireFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<Fire>::iterator iter;
	for( iter = Fires.begin(); iter != Fires.end(); ++iter )
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
void FireFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<Fire>::iterator iter;
	for( iter = Fires.begin(); iter != Fires.end(); ++iter )
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
void FireFactory::Spawn( s32 x, s32 y, int direction  )
{
	
	std::vector<Fire>::iterator iter;
	for( iter = Fires.begin(); iter != Fires.end(); ++iter )
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
void FireFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	std::vector<Fire>::iterator iter;
	for( iter = Fires.begin(); iter != Fires.end(); ++iter )
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
void FireFactory::KillAllEntities()
{
	std::vector<Fire>::iterator iter;
	for( iter = Fires.begin(); iter != Fires.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}



