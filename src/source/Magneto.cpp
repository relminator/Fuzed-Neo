/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Magneto.h"

#define MAX_MAGNETOS 16

#define MAGNETO_SPEED floattof32(0.035)

Magneto::Magneto()
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
	
	Width = (20 * TILE_SIZE_3D) / TILE_SIZE;   // <= 20 cuz I was lazy and didn't use -+1 but -+height/width
	Height = (20 * TILE_SIZE_3D) / TILE_SIZE;  // in the movement ai 
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 5;
	MaxFrame = 9;
	
	Stunned = false;
	Score = 75;
}

Magneto::~Magneto()
{
}


void Magneto::Init( s32 sx, s32 sy, int axis )
{
	Active = true;
	
	x = sx;
	y = sy;
	
	Axis = axis;
	
	Direction = -1;  // on air
	
	x = sx;
	y = sy;
	xv = 0 ;
	yv = 0;;
	
	Direction = -1;
	Speed = 0;
	
	ResolveAABBS();
	
	
}

void Magneto::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	ResolveStates();
	
	Rotation = 0;
	
	if( Axis > 0	)
	{
		UpdateMagnetoRight( Map );
		switch( Direction )
		{
			case -1:
				Rotation = 0;
				Flipmode = GL_FLIP_NONE;
				break;
			case 0:		// right
				Rotation = 0;
				Flipmode = GL_FLIP_NONE;
				break;
			case 1:		// up
				Rotation = (BRAD_PI/2) * 3;
				Flipmode = GL_FLIP_NONE;
				break;
			case 2:		// down
				Rotation = (BRAD_PI/2);
				Flipmode = GL_FLIP_NONE;
				break;
			case 3:		// left
				Rotation = (BRAD_PI);
				Flipmode = GL_FLIP_NONE;
				break;
		}
	
	}
	else
	{
		UpdateMagnetoLeft( Map );
		switch( Direction )
		{
			case -1:
				Rotation = 0;
				Flipmode = GL_FLIP_H;
				break;
			case 0:		// right
				Rotation = BRAD_PI;
				Flipmode = GL_FLIP_H;
				break;
			case 1:		// up
				Rotation = (BRAD_PI/2);
				Flipmode = GL_FLIP_H;
				break;
			case 2:		// down
				Rotation = (BRAD_PI/2)*3;
				Flipmode = GL_FLIP_H;
				break;
			case 3:		// left
			Rotation = 0;
				Flipmode = GL_FLIP_H;
				break;
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

void Magneto::Draw( const glImage *spr, s32 z )
{
	glSpriteRotate3D( x + Width/2, y + Height/2, z, Rotation, Flipmode, &spr[BaseFrame+Frame] );
}
	

void Magneto::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
}



bool Magneto::CollideWithPlayer( Player &Mario, Map2D &Map )
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
	
	if( Mario.CollideAttack( aabb ) )
	{
		if( !Stunned ) Sound::Instance()->PlaySFX(E_SFX_ATTACK_HIT);
		StunMe( 20 );
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
	
	
	return false;
}


void Magneto::UpdateMagnetoRight( Map2D &Map )
{
	s32 xoffset = Width/2;
	s32 yoffset = Height/2;
	
	s32 sx = x + xoffset;
	s32 sy = y + yoffset;
	
	s32 Dx = xv;
	s32 Dy = yv;
	
	switch( Direction )
	{
		
		case -1:
			if( GetTile( sx, sy + Dy + (Height/2), Map ) >= BRICK )  // Tile Below
			{
				Dx = (MAGNETO_SPEED);
				Dy = 0;
				Direction = 0;
			}
			break;
		
		case 0:			// right
			if( GetTile( sx + xoffset, sy, Map ) >= BRICK ) // Tile right
			{
				Direction = 1;		//up
			}
			else	// Check below
			{
				if( GetTile( sx - xoffset , sy + Height, Map ) < BRICK )   // no tile below
				{
					if( GetTile( sx - Width, sy + Height, Map ) >= BRICK )
					{
						Direction = 2;   // Down
					}
				}
			}
			break;
		
		case 1:			// up
		
			if( GetTile( sx, sy - yoffset, Map ) >= BRICK ) // Tile above
			{
				Direction = 3;		// left
			}
			else  // No Tile above
			{
				// Check right
				if( GetTile( sx + Width, sy + yoffset, Map ) < BRICK )   // no tile right
				{
					if( GetTile( sx + Width, sy + Height, Map ) >= BRICK )
					{
					Direction = 0;   // right
					}
				}
			}
			break;
		
		case 2:			// down
		
			if( GetTile( sx, sy + yoffset, Map ) >= BRICK ) // Tile down
			{
				Direction = 0;		// right
			}
			else  // No Tile below
			{
				// Check left
				if( GetTile( sx - Width, sy - yoffset, Map ) < BRICK )   // no tile left
				{
					if( GetTile( sx - Width, sy - Height, Map ) >= BRICK )
					{
						Direction = 3;   // left
					}
				}
			}
			break;
		
		
		case 3:			// left
				
			if( GetTile( sx - xoffset, sy, Map ) >= BRICK ) // Tile left
			{
				Direction = 2;		//down
			}
			else  // No Tile left
			{
				// Check below
				if( GetTile( sx + xoffset, sy - Height, Map ) < BRICK )   // no tile below
				{
					if( GetTile( sx + Width, sy - Height, Map ) >= BRICK )
					{
						Direction = 1;   // up
					}
				}
			}
			break;

	}
	
	switch( Direction )
	{
		case -1:
			break;
		case 0:		// right
			Dx = (MAGNETO_SPEED);
			Dy = 0;
			break;
		case 1:		// up
			Dx = 0;
			Dy = -(MAGNETO_SPEED);
			break;
		case 2:		// down
			Dx = 0;
			Dy = (MAGNETO_SPEED);
			break;
		case 3:		// left
			Dx = -(MAGNETO_SPEED);
			Dy = 0;
			break;
		
		
		
	}

		
	x = sx - xoffset;
	y = sy - yoffset;
	
	xv = Dx;
	yv = Dy;
	
	if( IsOnWater( x + Width/2, y + Height/2, Map ) )
	{
		xv /= 2;
		yv /= 2;
	}
	
	if( Stunned ) 
	{
		xv = 0;
		yv = 0;
	}
	
	x += xv;
	y += yv;
	
	if( Direction == -1 ) yv += GRAVITY;
			
}

void Magneto::UpdateMagnetoLeft( Map2D &Map )
{
	s32 xoffset = Width/2;
	s32 yoffset = Height/2;
	
	s32 sx = x + xoffset;
	s32 sy = y + yoffset;
	
	s32 Dx = xv;
	s32 Dy = yv;
	
	switch( Direction )
	{
		
		case -1:
			if( GetTile( sx, sy + Dy + (Height/2), Map ) >= BRICK )  // Tile Below
			{
				Dx = -(MAGNETO_SPEED);
				Dy = 0;
				Direction = 3;
			}
			break;
		
		case 0:			// right
			if( GetTile( sx + xoffset, sy, Map ) >= BRICK ) // Tile right
			{
				Direction = 2;		//up
			}
			else	// Check below
			{
				if( GetTile( sx - xoffset , sy - Height, Map ) < BRICK )   // no tile below
				{
					if( GetTile( sx - Width, sy - Height, Map ) >= BRICK )
					{
						Direction = 1;   // Down
					}
				}
			}
			break;
		
		case 1:			// up
		
			if( GetTile( sx, sy - yoffset, Map ) >= BRICK ) // Tile above
			{
				Direction = 0;		// right
			}
			else  // No Tile above
			{
				// Check left
				if( GetTile( sx - Width, sy + yoffset, Map ) < BRICK )   // no tile right
				{
					if( GetTile( sx - Width, sy + Height, Map ) >= BRICK )
					{
					Direction = 3;   // left
					}
				}
			}
			break;
		
		case 2:			// down
		
			if( GetTile( sx, sy + yoffset, Map ) >= BRICK ) // Tile down
			{
				Direction = 3;		// right
			}
			else  // No Tile below
			{
				// Check right
				if( GetTile( sx + Width, sy - yoffset, Map ) < BRICK )   // no tile right
				{
					if( GetTile( sx + Width, sy - Height, Map ) >= BRICK )
					{
						Direction = 0;   // left
					}
				}
			}
			break;
		
		
		case 3:			// left
				
			if( GetTile( sx - xoffset, sy, Map ) >= BRICK ) // Tile left
			{
				Direction = 1;		//up
			}
			else  // No Tile left
			{
				// Check above
				if( GetTile( sx + xoffset, sy + Height, Map ) < BRICK )   // no tile above
				{
					if( GetTile( sx + Width, sy + Height, Map ) >= BRICK )
					{
						Direction = 2;   // down
					}
				}
			}
			break;

	}
	
	switch( Direction )
	{
		case -1:
			break;
		case 0:		// right
			Dx = (MAGNETO_SPEED);
			Dy = 0;
			break;
		case 1:		// up
			Dx = 0;
			Dy = -(MAGNETO_SPEED);
			break;
		case 2:		// down
			Dx = 0;
			Dy = (MAGNETO_SPEED);
			break;
		case 3:		// left
			Dx = -(MAGNETO_SPEED);
			Dy = 0;
			break;
		
		
		
	}
	
	
	
	x = sx - xoffset;
	y = sy - yoffset;
	
	xv = Dx;
	yv = Dy;
	
	if( IsOnWater( x + Width/2, y + Height/2, Map ) )
	{
		xv /= 2;
		yv /= 2;
	}
	
	if( Stunned ) 
	{
		xv = 0;
		yv = 0;
	}
	
	x += xv;
	y += yv;

	if( Direction == -1 ) yv += GRAVITY;
			
}


//------------------------------------------------------------------------------
//	MagnetoFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
MagnetoFactory::MagnetoFactory()
{
}
	
MagnetoFactory::~MagnetoFactory()
{
	Magnetos.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void MagnetoFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	Magnetos.resize( MAX_MAGNETOS );			// speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void MagnetoFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<Magneto>::iterator iter;
	for( iter = Magnetos.begin(); iter != Magnetos.end(); ++iter )
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
void MagnetoFactory::DrawEntities( s32 z )
{
	
	std::vector<Magneto>::iterator iter;
	for( iter = Magnetos.begin(); iter != Magnetos.end(); ++iter )
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
void MagnetoFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<Magneto>::iterator iter;
	for( iter = Magnetos.begin(); iter != Magnetos.end(); ++iter )
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
void MagnetoFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<Magneto>::iterator iter;
	for( iter = Magnetos.begin(); iter != Magnetos.end(); ++iter )
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
void MagnetoFactory::Spawn( s32 x, s32 y, int axis )
{
	
	std::vector<Magneto>::iterator iter;
	for( iter = Magnetos.begin(); iter != Magnetos.end(); ++iter )
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
void MagnetoFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	std::vector<Magneto>::iterator iter;
	for( iter = Magnetos.begin(); iter != Magnetos.end(); ++iter )
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
void MagnetoFactory::KillAllEntities()
{
	std::vector<Magneto>::iterator iter;
	for( iter = Magnetos.begin(); iter != Magnetos.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}




