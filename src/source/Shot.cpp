/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Shot.h"

Shot::Shot()
{

	Active = false;
	ShotID = NORMAL; 
	Frame = 0;
	NumFrames = 1;
	BaseFrame = 0;
	Width = 2;
	Height = 2;
	Life = 60 * 3;
	
	Direction = -1; // -1 on air, 0 floor, 1 right wall, 2 left floor, 3 ceiling
	

}

Shot::~Shot()
{

}


void Shot::InitNormal( s32 sx, s32 sy, int frame, int level, int Direction, int SpriteSize )
{
	
	Active = true;
	ShotID = NORMAL;
	
	Counter = 0;
	Life = 60 * 2;
	
	x = sx;
	y = sy;
	Dx = (PLAYER_SHOT_SPEED/2) * Direction;
	Dy = 0;
	
	Frame = frame;
	NumFrames = 5;
	BaseFrame = 12;
	Width = (16 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (16 * TILE_SIZE_3D) / TILE_SIZE;
	
	// Reset AABB
	Aabb.Init( x - (Width/2), y - (Height/2), Width, Height );
	
}


void Shot::InitWave( s32 sx, s32 sy, s32 angle, int frame, int level, int Direction, int SpriteSize )
{
	
	Active = true;
	ShotID = WAVE;
	Counter = 0;
	Life = 60 * 2;
	
	x = sx;
	y = sy;
	Dx = (PLAYER_SHOT_SPEED/2) * Direction ;
	Dy = sy;
	Angle = angle;
	Radius = 0;
	
	Frame = frame;
	NumFrames = 6;
	BaseFrame = 6;
	
	Width = (16 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (16 * TILE_SIZE_3D) / TILE_SIZE;
	
	// Reset AABB
	Aabb.Init( x, y, Width, Height );
	
}

void Shot::InitBounce( s32 sx, s32 sy, s32 angle, int frame, int level, int Direction, int SpriteSize )
{
	
	Active = true;
	ShotID = BOUNCE;
	Counter = 0;
	Life = 60 * 4;
	
	x = sx;
	y = sy;
	Dx = (PLAYER_SHOT_SPEED/2) * Direction ;
	Dy = PLAYER_SHOT_SPEED;
	Angle = angle;
	Radius = 0;
	
	Frame = frame;
	NumFrames = 2;
	BaseFrame = 4;
	
	Width = (20 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (20 * TILE_SIZE_3D) / TILE_SIZE;
	
	// Reset AABB
	Aabb.Init( x, y, Width, Height );
	
}	

void Shot::InitThrow( s32 sx, s32 sy, int frame, int level, int Direction, int SpriteSize )
{
	
	Active = true;
	ShotID = THROW;
	Counter = 0;
	Life = 60 * 4;
	
	x = sx;
	y = sy;
	Dx = (PLAYER_SHOT_SPEED/2) * Direction ;
	Dy = -JUMP_HEIGHT/2;
	Radius = 0;
	
	Frame = frame;
	NumFrames = 1;
	BaseFrame = 17;
	
	Width = (20 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (20 * TILE_SIZE_3D) / TILE_SIZE;
	
	// Reset AABB
	Aabb.Init( x, y, Width, Height );
	
}	

void Shot::InitWater( s32 sx, s32 sy, int frame, int level, int Direction, int SpriteSize )
{
	
	Active = true;
	ShotID = WATER;
	Counter = 0;
	Life = 60 * 4;
	
	
	x = sx;
	y = sy;
	Dx = (PLAYER_SHOT_SPEED/4) * Direction ;
	Dy = -JUMP_HEIGHT/4;;
	Radius = Direction;   // used for left and right movement
	
	Frame = frame;
	NumFrames = 13;
	BaseFrame = 0;
	
	Width = (20 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (20 * TILE_SIZE_3D) / TILE_SIZE;
	
	// Reset AABB
	Aabb.Init( x, y, Width, Height );
	
}

void Shot::InitMagneto( s32 sx, s32 sy, int frame, int level, int xDirection, int SpriteSize )
{
	
	Active = true;
	ShotID = MAGNETO;
	Counter = 0;
	Life = 60 * 4;
	Direction = -1;  // on air
	
	x = sx - xDirection * Width;
	y = sy;
	Dx = 0 ;
	Dy = 0;
	Radius = xDirection;   // base direction
	
	Frame = frame;
	NumFrames = 13;
	BaseFrame = 0;
	
	Width = (20 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (20 * TILE_SIZE_3D) / TILE_SIZE;
	
	// Reset AABB
	Aabb.Init( x, y, Width, Height );
	
}

void Shot::Update( Map2D &Map )
{
	
	Counter++;
	s32 sy = 0;
	int offset;
	switch( ShotID )
	{
		case WAVE:
			Angle += 600;
			Radius += 50;
			sy = ( sinLerp(Angle) ) * ( Radius );
			x += Dx;
			y = Dy + (sy >> 12);         
			break;
		
		case NORMAL:
			x += Dx;
			y += Dy;
			break;
		
		case BOUNCE:
			if( GetTile( x + Dx, y, Map ) >= BRICK )
			{
				Dx = -Dx;
			}
			
			if( GetTile( x, y + Dy, Map ) >= BRICK )
			{
				Dy = -Dy;
			}
			
			x += Dx;
			y += Dy;
			break;
			
		case THROW:
			if( GetTile( x + Dx, y, Map ) >= BRICK )
			{
				Dx = -Dx;
			}
			
			if( GetTile( x, y + Dy, Map ) >= BRICK )
			{
				if( Dy > 0 ) Dy = -JUMP_HEIGHT; 
				else  Dy = -Dy;
			}
			
			x += Dx;
			y += Dy;
			Dy += GRAVITY;
			break;
			
		case WATER:
			// Midpoint +- HalfWidth +- Dx
			offset = Radius * (Width/2) + Dx;
			if( GetTile( x + offset, y, Map ) >= BRICK ) // Tile Left or right
 			{
				Dx = -Dx;
				Radius = -Radius;
			}
			else if( GetTile( x, y + Dy + (Height/2), Map ) >= BRICK )  // Tile Below
			{
				Dx = (PLAYER_SHOT_SPEED/2) * Radius;
				Dy = 0;
			}
			else  // No Tile below, left and right
			{
				// offset a little to left and right
				if( GetTile( x - offset, y + Height, Map ) < BRICK ) Dx = 0;
			}
			
			x += Dx;
			y += Dy;
			Dy += GRAVITY;
			break;
		
		case MAGNETO:
			if( Radius == 1 )
				UpdateMagnetoRight( Map );
			else
				UpdateMagnetoLeft( Map );
			break;
		default:
			break;
		
	}
	
	if( (Counter & 1) == 0 )
	{
		Frame = ( Frame + 1 ) % NumFrames;
	}
	
	if( (Counter & 7) == 0 )
	{
		Sound::Instance()->PlaySFX(E_SFX_MAGIC_ACTIVE);
	}
	
	Life--;
	if( Life  < 0 )
	{
		Explosion::Instance()->Spawn( x,
									  y,
									  0,
									  0,
									  Explode::SMALL_4 );
	
		Kill();
	}
	
	// Reset AABB
	Aabb.Init( x - (Width/2), y - (Height/2), Width, Height );
	
}

void Shot::Kill()
{
	Active = false;
	x = -300 << 12;
	Aabb.Init( x - Width/2, y - Height /2, Width, Height );
}


void Shot::UpdateMagnetoRight( Map2D &Map )
{
	s32 xoffset = Width/2;
	s32 yoffset = Height/2;
	
	switch( Direction )
	{
		
		case -1:
			if( GetTile( x + xoffset, y, Map ) >= BRICK )  // Tile right
			{
				Direction = 1;    // up
			}
			break;
		
		case 0:			// right
			if( GetTile( x + xoffset, y, Map ) >= BRICK ) // Tile right
			{
				Direction = 1;		//up
			}
			else	// Check below
			{
				if( GetTile( x - xoffset , y + Height, Map ) < BRICK )   // no tile below
				{
					if( GetTile( x - Width, y + Height, Map ) >= BRICK )
					{
						Direction = 2;   // Down
					}
				}
			}
			break;
		
		case 1:			// up
		
			if( GetTile( x, y - yoffset, Map ) >= BRICK ) // Tile above
			{
				Direction = 3;		// left
			}
			else  // No Tile above
			{
				// Check right
				if( GetTile( x + Width, y + yoffset, Map ) < BRICK )   // no tile right
				{
					if( GetTile( x + Width, y + Height, Map ) >= BRICK )
					{
					Direction = 0;   // right
					}
				}
			}
			break;
		
		case 2:			// down
		
			if( GetTile( x, y + yoffset, Map ) >= BRICK ) // Tile down
			{
				Direction = 0;		// right
			}
			else  // No Tile below
			{
				// Check left
				if( GetTile( x - Width, y - yoffset, Map ) < BRICK )   // no tile left
				{
					if( GetTile( x - Width, y - Height, Map ) >= BRICK )
					{
						Direction = 3;   // left
					}
				}
			}
			break;
		
		
		case 3:			// left
				
			if( GetTile( x - xoffset, y, Map ) >= BRICK ) // Tile left
			{
				Direction = 2;		//down
			}
			else  // No Tile left
			{
				// Check below
				if( GetTile( x + xoffset, y - Height, Map ) < BRICK )   // no tile below
				{
					if( GetTile( x + Width, y - Height, Map ) >= BRICK )
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
			Dx = (PLAYER_SHOT_SPEED/2);
			Dy = 0;
			break;
		case 0:		// right
			Dx = (PLAYER_SHOT_SPEED/2);
			Dy = 0;
			break;
		case 1:		// up
			Dx = 0;
			Dy = -(PLAYER_SHOT_SPEED/2);
			break;
		case 2:		// down
			Dx = 0;
			Dy = (PLAYER_SHOT_SPEED/2);
			break;
		case 3:		// left
			Dx = -(PLAYER_SHOT_SPEED/2);
			Dy = 0;
			break;
		
		
		
	}

	x += Dx;
	y += Dy;

}

void Shot::UpdateMagnetoLeft( Map2D &Map )
{
	s32 xoffset = Width/2;
	s32 yoffset = Height/2;
	
	switch( Direction )
	{
		
		case -1:
			if( GetTile( x - xoffset, y, Map ) >= BRICK )  // Tile left
			{
				Direction = 1;    // up
			}
			break;
		
		case 0:			// right
			if( GetTile( x + xoffset, y, Map ) >= BRICK ) // Tile right
			{
				Direction = 2;		//up
			}
			else	// Check below
			{
				if( GetTile( x - xoffset , y - Height, Map ) < BRICK )   // no tile below
				{
					if( GetTile( x - Width, y - Height, Map ) >= BRICK )
					{
						Direction = 1;   // Down
					}
				}
			}
			break;
		
		case 1:			// up
		
			if( GetTile( x, y - yoffset, Map ) >= BRICK ) // Tile above
			{
				Direction = 0;		// right
			}
			else  // No Tile above
			{
				// Check left
				if( GetTile( x - Width, y + yoffset, Map ) < BRICK )   // no tile right
				{
					if( GetTile( x - Width, y + Height, Map ) >= BRICK )
					{
					Direction = 3;   // left
					}
				}
			}
			break;
		
		case 2:			// down
		
			if( GetTile( x, y + yoffset, Map ) >= BRICK ) // Tile down
			{
				Direction = 3;		// right
			}
			else  // No Tile below
			{
				// Check right
				if( GetTile( x + Width, y - yoffset, Map ) < BRICK )   // no tile right
				{
					if( GetTile( x + Width, y - Height, Map ) >= BRICK )
					{
						Direction = 0;   // left
					}
				}
			}
			break;
		
		
		case 3:			// left
				
			if( GetTile( x - xoffset, y, Map ) >= BRICK ) // Tile left
			{
				Direction = 1;		//up
			}
			else  // No Tile left
			{
				// Check above
				if( GetTile( x + xoffset, y + Height, Map ) < BRICK )   // no tile above
				{
					if( GetTile( x + Width, y + Height, Map ) >= BRICK )
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
			Dx = -(PLAYER_SHOT_SPEED/2);
			Dy = 0;
			break;
		case 0:		// right
			Dx = (PLAYER_SHOT_SPEED/2);
			Dy = 0;
			break;
		case 1:		// up
			Dx = 0;
			Dy = -(PLAYER_SHOT_SPEED/2);
			break;
		case 2:		// down
			Dx = 0;
			Dy = (PLAYER_SHOT_SPEED/2);
			break;
		case 3:		// left
			Dx = -(PLAYER_SHOT_SPEED/2);
			Dy = 0;
			break;
		
		
		
	}

	x += Dx;
	y += Dy;

}
